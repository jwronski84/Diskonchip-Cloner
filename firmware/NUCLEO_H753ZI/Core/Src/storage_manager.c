#include "storage_manager.h"
#include <string.h>

/*
 * Two-SD-card storage layer.
 *
 * SD1 system card: logs, config, diagnostics, update packages.
 * SD2 image card: .docimg raw images and verification metadata.
 *
 * Final implementation should use FatFs with two mounted logical drives,
 * for example "0:/" for SD1 and "1:/" for SD2.
 */

static doc_image_info_t active_image;
static bool image_write_open = false;
static bool image_read_open = false;

bool storage_init(void) {
    /* TODO: initialize SDMMC/SPI and mount both FatFs volumes. */
    memset(&active_image, 0, sizeof(active_image));
    image_write_open = false;
    image_read_open = false;
    return false;
}

bool storage_system_log(const char *message) {
    (void)message;
    /* TODO: append to SD1 /logs/current.log */
    return false;
}

bool storage_image_begin_write(doc_image_info_t *info) {
    if (!info) return false;
    active_image = *info;
    image_write_open = true;
    /* TODO: create SD2 /images/name.docimg and metadata files. */
    return false;
}

bool storage_image_write_block(uint32_t block, const uint8_t *data, uint32_t len) {
    if (!image_write_open || !data || len == 0) return false;
    (void)block; (void)data; (void)len;
    /* TODO: seek to block*block_size and write exactly one logical block. */
    return false;
}

bool storage_image_finish_write(doc_image_info_t *info) {
    if (!image_write_open) return false;
    image_write_open = false;
    if (info) *info = active_image;
    /* TODO: flush file, write .json sidecar and .sha256 checksum. */
    return false;
}

bool storage_image_begin_read(const char *image_name, doc_image_info_t *info) {
    if (!image_name || !info) return false;
    memset(&active_image, 0, sizeof(active_image));
    strncpy(active_image.image_name, image_name, sizeof(active_image.image_name)-1);
    image_read_open = true;
    /* TODO: open SD2 image and read metadata. */
    *info = active_image;
    return false;
}

bool storage_image_read_block(uint32_t block, uint8_t *data, uint32_t len) {
    if (!image_read_open || !data || len == 0) return false;
    (void)block; (void)data; (void)len;
    /* TODO: seek to block*block_size and read exactly one logical block. */
    return false;
}

bool storage_image_finish_read(void) {
    image_read_open = false;
    /* TODO: close file. */
    return false;
}
