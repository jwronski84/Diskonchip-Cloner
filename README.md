# DiskOnChip MD2202-D16 Cloner

Prototype open-hardware/software project for a low-cost MD2202-D16 DiskOnChip cloner.

> **Important:** This is an early prototype package. The MD2202-D16 is an M-Systems DiskOnChip device with controller/TrueFFS behavior, not a simple raw flash chip. The current software includes a safe simulation backend and hardware-interface skeletons that must be validated against real hardware before any production use.

## Contents

- `docs/BUILD_GUIDE.md` - assembly and operation guide
- `bom/BOM_component_sources.csv` - component list and supplier websites
- `src/python_cloner/` - Python clone/verify control utility with simulation backend
- `src/stm32h7_option/` - custom STM32 FMC controller-board skeleton
- `mechanical/openscad/` - parametric 3D-printable enclosure model

## Recommended Hardware Paths

### Path A: Custom STM32 board with FMC-capable MCU
Recommended primary direction. Use a currently available STM32 with FMC/external-memory support and enough exposed pins for an 8-bit asynchronous bus, socket selection, controls, display, and storage. This avoids depending on obsolete Nucleo-144 boards.

### Path B: BeagleBone Black + external bus interface
Useful for Linux UI, logging, SD-card imaging, and PRU/CPLD-style deterministic bus timing.

### Path C: Teensy 4.1 prototype
Useful for early low-cost GPIO timing experiments, but less ideal than a true external-memory-controller design.

## Basic workflow

1. Assemble the enclosure and electronics.
2. Test with simulation mode first.
3. Validate source/target detection on real MD2202-D16 parts.
4. Read source image.
5. Write target image.
6. Verify target against source or saved image.

## Safety

Do not attempt to write to a source chip. Confirm orientation, voltage, ZIF socket pin 1, bus transceiver direction, and power sequencing before inserting DiskOnChip parts.
