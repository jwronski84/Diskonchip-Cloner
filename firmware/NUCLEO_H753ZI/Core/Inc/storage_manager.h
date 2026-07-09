#pragma once
#include <stdint.h>
#include <stdbool.h>

#define IMAGE_NAME_MAX 96

typedef enum {
    STORAGE_SD_SYSTEM = 0,
    STORAGE_SD_IMAGE  = 1
} storage_slot_t;

typedef struct {
    char image_name[IMAGE_NAME_MAX];
    uint32_t block_size;
    uint32_t block_count;
    uint32_t image_bytes;
    char sha256_hex[65];
} doc_image_info_t;

bool storage_init(void);
bool storage_system_log(const char *message);
bool storage_image_begin_write(doc_image_info_t *info);
bool storage_image_write_block(uint32_t block, const uint8_t *data, uint32_t len);
bool storage_image_finish_write(doc_image_info_t *info);
bool storage_image_begin_read(const char *image_name, doc_image_info_t *info);
bool storage_image_read_block(uint32_t block, uint8_t *data, uint32_t len);
bool storage_image_finish_read(void);
