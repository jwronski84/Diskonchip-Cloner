#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "app_config.h"

/*
 * TrueFFS integration boundary.
 *
 * This project requires legally obtained TrueFFS/BDK/SDK or compatible
 * DiskOnChip command support before real low-level target formatting and
 * full ECC-corrected logical block access can be claimed.
 */

typedef enum {
    TRUEFFS_OK = 0,
    TRUEFFS_ERR_NOT_AVAILABLE,
    TRUEFFS_ERR_IO,
    TRUEFFS_ERR_FORMAT,
    TRUEFFS_ERR_ECC
} trueffs_status_t;

bool trueffs_available(void);
trueffs_status_t trueffs_identify(doc_socket_t socket, uint32_t *block_size, uint32_t *block_count);
trueffs_status_t trueffs_format_target(uint8_t *percent_complete);
trueffs_status_t trueffs_read_block(doc_socket_t socket, uint32_t block, uint8_t *out512);
trueffs_status_t trueffs_write_block(doc_socket_t socket, uint32_t block, const uint8_t *in512);
bool trueffs_edc_ecc_active(void);
