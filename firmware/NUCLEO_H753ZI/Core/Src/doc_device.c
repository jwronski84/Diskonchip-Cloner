#include "doc_device.h"
#include "doc_bus.h"
#include <string.h>

/*
 * DiskOnChip logical-device layer.
 *
 * MD2202-D16 support requires the exact DiskOnChip register/command sequence.
 * This layer is where the official identify/read/write routines belong after
 * hardware read-only bus validation.
 */

bool doc_device_identify(doc_socket_t socket, doc_info_t *info) {
    if (!info) return false;
    memset(info, 0, sizeof(*info));
    doc_bus_select(socket);

    /* TODO: replace with real DiskOnChip ID register access. */
    info->present = false;
    info->block_size = DOC_BLOCK_SIZE;
    info->block_count = DOC_BLOCKS_16M;
    strncpy(info->name, "MD2202-D16 TODO", sizeof(info->name)-1);

    doc_bus_deselect_all();
    return info->present;
}

bool doc_device_read_block(doc_socket_t socket, uint32_t block, uint8_t *out512) {
    if (!out512 || block >= DOC_BLOCKS_16M) return false;
    doc_bus_select(socket);

    /* TODO: implement logical block read through DiskOnChip command/register protocol. */
    memset(out512, 0xFF, DOC_BLOCK_SIZE);

    doc_bus_deselect_all();
    return false;
}

bool doc_device_write_block(doc_socket_t socket, uint32_t block, const uint8_t *in512) {
    if (!in512 || block >= DOC_BLOCKS_16M) return false;
    if (socket == DOC_SOCKET_SOURCE) return false;
#if DOC_ENABLE_WRITES
    doc_bus_set_write_enabled(socket, true);
    doc_bus_select(socket);

    /* TODO: implement logical block write through DiskOnChip command/register protocol. */
    (void)in512;

    doc_bus_deselect_all();
    doc_bus_set_write_enabled(socket, false);
    return false;
#else
    (void)socket; (void)block; (void)in512;
    return false;
#endif
}
