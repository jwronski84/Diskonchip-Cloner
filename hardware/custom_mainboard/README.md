# Custom single-PCB mainboard concept

This directory defines the production-oriented replacement for the NUCLEO-H753ZI prototype stack.

## Goal

Replace the NUCLEO development board, adapter PCB, loose wiring, and SD breakout boards with one integrated mainboard that mounts directly inside the 118 mm tall enclosure concept.

## Mainboard features

- STM32H753 microcontroller
- Dual 32-pin premium ZIF sockets for SOURCE and TARGET MD2202-D16 devices
- Buffered/level-translated MD2202 8-bit bus interface
- Hardware source write-protect
- Target write-enable gate and safety latch
- 5.0 inch touchscreen connector
- Dual microSD card sockets
  - SD1: system/log/update card
  - SD2: image/archive card
- USB-C service/programming/debug connector
- SWD programming header
- 5 V DC power input
- 3.3 V logic regulator
- selectable/validated DiskOnChip VCC rail support
- fan connector with PWM/tach support
- Clone and Cancel/Back button connectors
- Power/Busy/Pass/Fail LEDs
- Test points for address/data/control lines
- Mounting holes aligned to the 118 mm enclosure

## Why this is better than the NUCLEO prototype

- Far less wiring
- Better signal integrity
- Cleaner front/rear panel integration
- Easier assembly
- Lower final unit cost after prototype validation
- More professional internal layout

## Development strategy

1. Keep NUCLEO-H753ZI as the bring-up/prototype firmware platform.
2. Design the custom board around the same STM32H753 family and firmware interfaces.
3. Keep the DiskOnChip bus abstraction identical so firmware can move from NUCLEO to custom PCB.
4. Use the 118 mm tall enclosure as the fixed mechanical target.

## Board stack recommendation

A 4-layer PCB is recommended:

1. Signal / components
2. Ground plane
3. Power plane / controlled rails
4. Signal / low-speed routing

The MD2202 bus is not extremely high speed, but the edge-sensitive CE#, OE#, and WE# lines require clean routing, good grounding, and proper series termination.
