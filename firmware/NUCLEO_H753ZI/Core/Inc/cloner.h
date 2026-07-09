#pragma once
#include <stdint.h>
#include <stdbool.h>

typedef enum {
    CLONER_OK = 0,
    CLONER_ERR_IDENTIFY,
    CLONER_ERR_GEOMETRY,
    CLONER_ERR_READ,
    CLONER_ERR_WRITE,
    CLONER_ERR_VERIFY
} cloner_status_t;

cloner_status_t cloner_clone_source_to_target(uint32_t *failed_block);
cloner_status_t cloner_verify_source_target(uint32_t *failed_block);
