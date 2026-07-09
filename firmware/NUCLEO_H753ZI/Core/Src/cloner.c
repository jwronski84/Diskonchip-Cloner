#include "cloner.h"
#include "doc_device.h"
#include "ui.h"
#include <string.h>

static uint8_t src_buf[DOC_BLOCK_SIZE];
static uint8_t tgt_buf[DOC_BLOCK_SIZE];

static bool geometry_matches(const doc_info_t *a, const doc_info_t *b) {
    return a->present && b->present && a->block_size == b->block_size && a->block_count == b->block_count;
}

cloner_status_t cloner_clone_source_to_target(uint32_t *failed_block) {
    doc_info_t src, tgt;
    if (!doc_device_identify(DOC_SOCKET_SOURCE, &src) || !doc_device_identify(DOC_SOCKET_TARGET, &tgt))
        return CLONER_ERR_IDENTIFY;
    if (!geometry_matches(&src, &tgt)) return CLONER_ERR_GEOMETRY;

    ui_status("CLONE START");
    for (uint32_t b=0; b<src.block_count; ++b) {
        if (!doc_device_read_block(DOC_SOCKET_SOURCE, b, src_buf)) { if (failed_block) *failed_block=b; return CLONER_ERR_READ; }
        if (!doc_device_write_block(DOC_SOCKET_TARGET, b, src_buf)) { if (failed_block) *failed_block=b; return CLONER_ERR_WRITE; }
        if (!doc_device_read_block(DOC_SOCKET_TARGET, b, tgt_buf)) { if (failed_block) *failed_block=b; return CLONER_ERR_VERIFY; }
        if (memcmp(src_buf, tgt_buf, src.block_size) != 0) { if (failed_block) *failed_block=b; return CLONER_ERR_VERIFY; }
        ui_progress(b+1, src.block_count);
    }
    ui_status("CLONE PASS");
    return CLONER_OK;
}

cloner_status_t cloner_verify_source_target(uint32_t *failed_block) {
    doc_info_t src, tgt;
    if (!doc_device_identify(DOC_SOCKET_SOURCE, &src) || !doc_device_identify(DOC_SOCKET_TARGET, &tgt))
        return CLONER_ERR_IDENTIFY;
    if (!geometry_matches(&src, &tgt)) return CLONER_ERR_GEOMETRY;

    ui_status("VERIFY START");
    for (uint32_t b=0; b<src.block_count; ++b) {
        if (!doc_device_read_block(DOC_SOCKET_SOURCE, b, src_buf)) { if (failed_block) *failed_block=b; return CLONER_ERR_READ; }
        if (!doc_device_read_block(DOC_SOCKET_TARGET, b, tgt_buf)) { if (failed_block) *failed_block=b; return CLONER_ERR_READ; }
        if (memcmp(src_buf, tgt_buf, src.block_size) != 0) { if (failed_block) *failed_block=b; return CLONER_ERR_VERIFY; }
        ui_progress(b+1, src.block_count);
    }
    ui_status("VERIFY PASS");
    return CLONER_OK;
}
