#include "doc_bus.h"
#include "cloner.h"
#include "ui.h"

int main(void) {
    /* TODO: HAL_Init(); SystemClock_Config(); MX_GPIO_Init(); MX_FMC_Init(); MX_SPI_Init(); */
    ui_init();
    doc_bus_init();
    ui_status("MD2202 CLONER READY");

    for (;;) {
        ui_command_t cmd = ui_poll_command();
        uint32_t failed = 0;
        cloner_status_t st = CLONER_OK;

        if (cmd == UI_CMD_CLONE) {
            st = cloner_clone_source_to_target(&failed);
        } else if (cmd == UI_CMD_VERIFY) {
            st = cloner_verify_source_target(&failed);
        } else if (cmd == UI_CMD_READ) {
            ui_status("READ IMAGE TODO");
        } else if (cmd == UI_CMD_WRITE_IMAGE) {
            ui_status("WRITE IMAGE TODO");
        }

        if (st != CLONER_OK) {
            (void)failed;
            ui_status("ERROR - SEE LOG");
        }
    }
}
