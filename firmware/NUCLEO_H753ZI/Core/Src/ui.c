#include "ui.h"

void ui_init(void) {
    /* TODO: initialize TFT, LEDs, buttons, encoder, and logging. */
}

void ui_status(const char *msg) {
    (void)msg;
    /* TODO: print to TFT and optional UART. */
}

void ui_progress(uint32_t done, uint32_t total) {
    (void)done; (void)total;
    /* TODO: update TFT progress bar and busy LED. */
}

ui_command_t ui_poll_command(void) {
    /* TODO: read buttons/encoder. */
    return UI_CMD_NONE;
}
