# STM32H7 option

The STM32H743 Nucleo-144 is the preferred microcontroller option for this cloner because the STM32H7 family includes an FMC external memory controller. That makes it a better match for the MD2202-D16 asynchronous memory-style bus than bit-banging GPIO from Linux.

## Intended approach

- Configure FMC for 8-bit asynchronous SRAM/NOR-style access.
- Use separate chip-select or enable logic for SOURCE and TARGET sockets.
- Buffer the bidirectional data bus with 74LVC245-style transceivers.
- Implement read-only device identification first.
- Implement logical block reads before writes.
- Keep verify-after-write mandatory.

## Status

This directory currently contains a skeleton, not a completed MD2202-D16 hardware driver.
