# NUCLEO-H753ZI firmware

Firmware target for the STM32 NUCLEO-H753ZI-based MD2202-D16 DiskOnChip cloner.

## Status

This is a compilable-style firmware architecture skeleton, not a finished production programmer. The clone engine and safety structure are implemented at the C source level, while the low-level DiskOnChip command/register routines remain isolated TODOs pending hardware validation.

## Layers

- `Core/Inc/app_config.h` - configuration constants
- `Core/Inc/doc_bus.h` / `Core/Src/doc_bus.c` - low-level memory bus abstraction
- `Core/Inc/doc_device.h` / `Core/Src/doc_device.c` - MD2202 logical block access layer
- `Core/Inc/cloner.h` / `Core/Src/cloner.c` - clone and verify engine
- `Core/Inc/ui.h` / `Core/Src/ui.c` - display/buttons/logging interface stubs
- `Core/Src/main.c` - top-level application loop

## Build path

Create a STM32CubeIDE project for NUCLEO-H753ZI, then copy this `Core` folder into the generated project. Configure FMC/GPIO/SPI in STM32CubeMX, then fill in the HAL handles and pin macros in `app_config.h` and `doc_bus.c`.

## First bring-up order

1. Build with `DOC_ENABLE_WRITES` set to `0`.
2. Test UI and button input.
3. Test control-line idle states with no MD2202 installed.
4. Test address/data toggling into a logic analyzer.
5. Implement `doc_bus_read8()` only.
6. Implement and confirm `doc_device_identify()`.
7. Implement logical block reads.
8. Only then enable target writes on sacrificial devices.
