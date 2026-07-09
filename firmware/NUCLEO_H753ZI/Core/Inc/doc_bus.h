#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "app_config.h"

bool doc_bus_init(void);
void doc_bus_select(doc_socket_t socket);
void doc_bus_deselect_all(void);
uint8_t doc_bus_read8(doc_socket_t socket, uint32_t offset);
void doc_bus_write8(doc_socket_t socket, uint32_t offset, uint8_t value);
void doc_bus_set_write_enabled(doc_socket_t socket, bool enabled);
bool doc_bus_is_write_enabled(doc_socket_t socket);
