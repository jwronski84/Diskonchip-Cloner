#include "doc_bus.h"

/*
 * Low-level bus layer.
 *
 * For the final NUCLEO-H753ZI adapter board, configure FMC/GPIO in STM32CubeMX.
 * This file intentionally contains safe placeholders so the clone engine can be
 * developed without accidentally asserting write-enable on real hardware.
 */

#define DOC_SOURCE_BASE ((volatile uint8_t *)0x60000000u)
#define DOC_TARGET_BASE ((volatile uint8_t *)0x64000000u)

static bool write_enable_source = false;
static bool write_enable_target = false;

bool doc_bus_init(void) {
    doc_bus_deselect_all();
    write_enable_source = false;
    write_enable_target = false;
    return true;
}

void doc_bus_select(doc_socket_t socket) {
    (void)socket;
    /* TODO: assert only selected socket CE#. */
}

void doc_bus_deselect_all(void) {
    /* TODO: deassert both CE#, OE#, and WE#. */
}

uint8_t doc_bus_read8(doc_socket_t socket, uint32_t offset) {
    volatile uint8_t *base = (socket == DOC_SOCKET_SOURCE) ? DOC_SOURCE_BASE : DOC_TARGET_BASE;
    /* TODO: validate FMC timing and address window. */
    return base[offset];
}

void doc_bus_write8(doc_socket_t socket, uint32_t offset, uint8_t value) {
#if DOC_ENABLE_WRITES
    if (!doc_bus_is_write_enabled(socket)) return;
    if (socket == DOC_SOCKET_SOURCE) return; /* never write source during clone */
    volatile uint8_t *base = DOC_TARGET_BASE;
    base[offset] = value;
#else
    (void)socket; (void)offset; (void)value;
#endif
}

void doc_bus_set_write_enabled(doc_socket_t socket, bool enabled) {
    if (socket == DOC_SOCKET_SOURCE) {
        write_enable_source = false;
        return;
    }
    write_enable_target = enabled;
}

bool doc_bus_is_write_enabled(doc_socket_t socket) {
    return (socket == DOC_SOCKET_SOURCE) ? write_enable_source : write_enable_target;
}
