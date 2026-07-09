#pragma once
#include <stdint.h>

#define DOC_BLOCK_SIZE       512u
#define DOC_BLOCKS_16M       32768u
#define DOC_IMAGE_BYTES      (DOC_BLOCK_SIZE * DOC_BLOCKS_16M)

/* Keep writes disabled until read-only identify/read is proven on sacrificial hardware. */
#define DOC_ENABLE_WRITES    0

/* Socket selection. */
typedef enum {
    DOC_SOCKET_SOURCE = 0,
    DOC_SOCKET_TARGET = 1
} doc_socket_t;

/* Board-specific GPIO/FMC pin macros go here after CubeMX pin assignment. */
#define DOC_SOURCE_WRITE_PROTECT_HARDWARE_REQUIRED 1
