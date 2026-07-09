#include "workflow.h"
#include "doc_device.h"
#include "storage_manager.h"
#include "ui.h"
#include <string.h>

static uint8_t block_buf[DOC_BLOCK_SIZE];
static doc_image_info_t active_image;

static uint8_t pct(uint32_t done, uint32_t total) {
    if (total == 0) return 0;
    if (done >= total) return 100;
    return (uint8_t)((done * 100u) / total);
}

static void poll_media(workflow_context_t *ctx) {
    doc_info_t src, tgt;
    ctx->source_present = doc_device_identify(DOC_SOCKET_SOURCE, &src);
    ctx->target_present = doc_device_identify(DOC_SOCKET_TARGET, &tgt);
    ctx->source_target_compatible = ctx->source_present && ctx->target_present &&
        src.block_size == tgt.block_size && src.block_count == tgt.block_count;
    ctx->sd1_present = true; /* TODO: query storage manager */
    ctx->sd2_present = true; /* TODO: query storage manager */
    ctx->trueffs_available = false; /* TODO: set true when licensed/integrated TrueFFS layer is active */
    ctx->edc_ecc_available = ctx->trueffs_available; /* DiskOnChip EDC + TrueFFS ECC requirement */
    ctx->total_blocks = DOC_BLOCKS_16M;
}

void workflow_init(workflow_context_t *ctx) {
    memset(ctx, 0, sizeof(*ctx));
    ctx->state = WF_WAIT_FOR_MEDIA;
    storage_init();
}

void workflow_tick(workflow_context_t *ctx) {
    switch (ctx->state) {
    case WF_WAIT_FOR_MEDIA:
        poll_media(ctx);
        ui_show_media_status(ctx->source_present, ctx->target_present, ctx->sd1_present, ctx->sd2_present, ctx->trueffs_available, ctx->edc_ecc_available);
        if (ctx->source_target_compatible && ctx->sd1_present && ctx->sd2_present) ctx->state = WF_READY_TO_FORMAT;
        break;

    case WF_READY_TO_FORMAT:
        ui_show_ready_to_format();
        break;

    case WF_FORMAT_TARGET_CONFIRM:
        ui_show_format_confirm();
        break;

    case WF_FORMAT_TARGET_RUNNING:
        /* TODO: replace with actual TrueFFS low-level format call once obtained. */
        ctx->stage_percent = 0;
        ui_show_progress("Formatting target", ctx->stage_percent, ctx->overall_percent);
        if (!ctx->trueffs_available) {
            ctx->state = WF_ERROR;
            ctx->failed_block = 0;
            ui_status("TrueFFS format support required");
        }
        break;

    case WF_READY_TO_CLONE:
        ui_show_ready_to_clone();
        break;

    case WF_BACKUP_SOURCE_TO_SD2:
        memset(&active_image, 0, sizeof(active_image));
        strncpy(active_image.image_name, "MD2202D16_ACTIVE_BACKUP.docimg", sizeof(active_image.image_name)-1);
        active_image.block_size = DOC_BLOCK_SIZE;
        active_image.block_count = DOC_BLOCKS_16M;
        active_image.image_bytes = DOC_BLOCK_SIZE * DOC_BLOCKS_16M;
        if (!storage_image_begin_write(&active_image)) { ctx->state = WF_ERROR; break; }
        for (uint32_t b=0; b<DOC_BLOCKS_16M; ++b) {
            if (!doc_device_read_block(DOC_SOCKET_SOURCE, b, block_buf)) { ctx->failed_block=b; ctx->state=WF_ERROR; break; }
            if (!storage_image_write_block(b, block_buf, DOC_BLOCK_SIZE)) { ctx->failed_block=b; ctx->state=WF_ERROR; break; }
            ctx->current_block = b + 1;
            ctx->stage_percent = pct(ctx->current_block, DOC_BLOCKS_16M);
            ctx->overall_percent = pct(ctx->current_block, DOC_BLOCKS_16M * 3u);
            ui_show_progress("Backing up source to SD2", ctx->stage_percent, ctx->overall_percent);
        }
        storage_image_finish_write(&active_image);
        if (ctx->state != WF_ERROR) ctx->state = WF_CLONE_SD2_TO_TARGET;
        break;

    case WF_CLONE_SD2_TO_TARGET:
        if (!storage_image_begin_read(active_image.image_name, &active_image)) { ctx->state = WF_ERROR; break; }
        for (uint32_t b=0; b<DOC_BLOCKS_16M; ++b) {
            if (!storage_image_read_block(b, block_buf, DOC_BLOCK_SIZE)) { ctx->failed_block=b; ctx->state=WF_ERROR; break; }
            if (!doc_device_write_block(DOC_SOCKET_TARGET, b, block_buf)) { ctx->failed_block=b; ctx->state=WF_ERROR; break; }
            ctx->current_block = b + 1;
            ctx->stage_percent = pct(ctx->current_block, DOC_BLOCKS_16M);
            ctx->overall_percent = pct(DOC_BLOCKS_16M + ctx->current_block, DOC_BLOCKS_16M * 3u);
            ui_show_progress("Cloning SD2 image to target", ctx->stage_percent, ctx->overall_percent);
        }
        storage_image_finish_read();
        if (ctx->state != WF_ERROR) ctx->state = WF_VERIFY_TARGET;
        break;

    case WF_VERIFY_TARGET:
        if (!storage_image_begin_read(active_image.image_name, &active_image)) { ctx->state = WF_ERROR; break; }
        for (uint32_t b=0; b<DOC_BLOCKS_16M; ++b) {
            uint8_t target_buf[DOC_BLOCK_SIZE];
            if (!storage_image_read_block(b, block_buf, DOC_BLOCK_SIZE)) { ctx->failed_block=b; ctx->state=WF_ERROR; break; }
            if (!doc_device_read_block(DOC_SOCKET_TARGET, b, target_buf)) { ctx->failed_block=b; ctx->state=WF_ERROR; break; }
            if (memcmp(block_buf, target_buf, DOC_BLOCK_SIZE) != 0) { ctx->failed_block=b; ctx->state=WF_ERROR; break; }
            ctx->current_block = b + 1;
            ctx->stage_percent = pct(ctx->current_block, DOC_BLOCKS_16M);
            ctx->overall_percent = pct((DOC_BLOCKS_16M * 2u) + ctx->current_block, DOC_BLOCKS_16M * 3u);
            ui_show_progress("Verifying target", ctx->stage_percent, ctx->overall_percent);
        }
        storage_image_finish_read();
        if (ctx->state != WF_ERROR) ctx->state = WF_COMPLETE;
        break;

    case WF_COMPLETE:
        ui_show_complete(active_image.image_name);
        break;

    case WF_ERROR:
        ui_show_error(ctx->failed_block);
        break;
    }
}

void workflow_request_format(workflow_context_t *ctx) {
    if (ctx->state == WF_READY_TO_FORMAT) ctx->state = WF_FORMAT_TARGET_CONFIRM;
}

void workflow_confirm_format(workflow_context_t *ctx) {
    if (ctx->state == WF_FORMAT_TARGET_CONFIRM) ctx->state = WF_FORMAT_TARGET_RUNNING;
}

void workflow_request_clone(workflow_context_t *ctx) {
    if (ctx->state == WF_READY_TO_CLONE) ctx->state = WF_BACKUP_SOURCE_TO_SD2;
}
