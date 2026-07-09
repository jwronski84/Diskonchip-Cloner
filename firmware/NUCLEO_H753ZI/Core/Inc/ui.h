#pragma once
#include <stdint.h>
#include <stdbool.h>

typedef enum {
    UI_CMD_NONE = 0,
    UI_CMD_READ,
    UI_CMD_CLONE,
    UI_CMD_VERIFY,
    UI_CMD_WRITE_IMAGE
} ui_command_t;

void ui_init(void);
void ui_status(const char *msg);
void ui_progress(uint32_t done, uint32_t total);
ui_command_t ui_poll_command(void);
