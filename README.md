# DiskOnChip MD2202-D16 Cloner

Prototype open-hardware/software project for a low-cost MD2202-D16 DiskOnChip cloner.

> **Important:** This is an early prototype package. The MD2202-D16 is an M-Systems DiskOnChip device with controller/TrueFFS behavior, not a simple raw flash chip. The current software includes a safe simulation backend and hardware-interface skeletons that must be validated against real hardware before any production use.

## Contents

- `docs/BUILD_GUIDE.md` - assembly and operation guide
- `docs/images/` - generated guide diagrams
- `bom/BOM_component_sources.csv` - component list and supplier websites
- `src/python_cloner/` - Python clone/verify control utility with simulation backend
- `src/stm32h7_option/` - STM32H7/FMC microcontroller option skeleton
- `stl/` - 3D-printable ASCII STL enclosure/control parts

## Recommended Hardware Paths

### Path A: BeagleBone Black + external bus interface
Useful for Linux UI, logging, SD-card imaging, and PRU/CPLD-style deterministic bus timing.

### Path B: STM32H743 Nucleo-144 option
Recommended as the easier microcontroller path because STM32H7 parts include FMC external-memory controller support, which better matches the asynchronous memory-bus style of the DiskOnChip interface.

## Basic workflow

1. Assemble the enclosure and electronics.
2. Test with simulation mode first.
3. Validate source/target detection on real MD2202-D16 parts.
4. Read source image.
5. Write target image.
6. Verify target against source or saved image.

## Safety

Do not attempt to write to a source chip. Confirm orientation, voltage, ZIF socket pin 1, bus transceiver direction, and power sequencing before inserting DiskOnChip parts.
