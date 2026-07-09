#include "trueffs_port.h"

/*
 * Placeholder for legally obtained TrueFFS SDK/BDK or compatible code.
 * Do not replace this with copied proprietary code unless you have the right
 * to use and redistribute it.
 */

bool trueffs_available(void) {
    return false;
}

trueffs_status_t trueffs_identify(doc_socket_t socket, uint32_t *block_size, uint32_t *block_count) {
    (void)socket;
    if (block_size) *block_size = 0;
    if (block_count) *block_count = 0;
    return TRUEFFS_ERR_NOT_AVAILABLE;
}

trueffs_status_t trueffs_format_target(uint8_t *percent_complete) {
    if (percent_complete) *percent_complete = 0;
    return TRUEFFS_ERR_NOT_AVAILABLE;
}

trueffs_status_t trueffs_read_block(doc_socket_t socket, uint32_t block, uint8_t *out512) {
    (void)socket; (void)block; (void)out512;
    return TRUEFFS_ERR_NOT_AVAILABLE;
}

trueffs_status_t trueffs_write_block(doc_socket_t socket, uint32_t block, const uint8_t *in512) {
    (void)socket; (void)block; (void)in512;
    return TRUEFFS_ERR_NOT_AVAILABLE;
}

bool trueffs_edc_ecc_active(void) {
    return false;
}
