#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "app_config.h"

typedef struct {
    bool present;
    uint32_t block_size;
    uint32_t block_count;
    char name[32];
} doc_info_t;

bool doc_device_identify(doc_socket_t socket, doc_info_t *info);
bool doc_device_read_block(doc_socket_t socket, uint32_t block, uint8_t *out512);
bool doc_device_write_block(doc_socket_t socket, uint32_t block, const uint8_t *in512);
