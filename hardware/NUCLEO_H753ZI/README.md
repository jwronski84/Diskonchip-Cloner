# NUCLEO-H753ZI hardware target

This is the primary prototype hardware target for the MD2202-D16 DiskOnChip cloner.

ST describes the NUCLEO-H753ZI as an STM32 Nucleo-144 board with the STM32H753ZI MCU, Arduino/ST Zio expansion, ST morpho connectivity, and integrated ST-LINK debugger/programmer. The morpho headers expose the MCU I/O needed for an adapter board.

## Electrical architecture

The NUCLEO-H753ZI does **not** plug directly into the MD2202-D16. Use an adapter board containing:

- 2x 32-pin DIP ZIF sockets, SOURCE and TARGET
- 8-bit bidirectional data bus buffer/transceiver
- Address/control buffers as needed
- Optional voltage translation if your DiskOnChip parts require it
- Decoupling capacitors near each socket
- Series damping resistors on fast bus lines
- Jumperable power/voltage selection only after verifying exact MD2202 variant

## Design direction

Firmware is structured so the FMC/GPIO low-level layer is isolated from the DiskOnChip logical-block clone engine. The first hardware milestone is safe read-only identification. Writes should remain disabled until read timing and command/register behavior are proven on sacrificial parts.
