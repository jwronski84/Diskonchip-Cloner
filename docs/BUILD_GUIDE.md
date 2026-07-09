# MD2202-D16 Cloner Prototype Build Guide

## What this package is

This is a prototype package for a low-cost MD2202-D16 logical-block cloner. It includes printable enclosure STL files, a component BOM, a safe simulator cloner, and hardware skeletons for the final electronics.

It is not yet a validated commercial DiskOnChip programmer. Before writing real MD2202-D16 parts, verify the DiskOnChip register protocol, voltage level, timing, write-enable behavior, and bad-block/TrueFFS handling on sacrificial devices.

## Recommended controller choice

Use the STM32H743 Nucleo-144 path first. The STM32H743 has a Flexible Memory Controller that can interface with asynchronous SRAM/NOR/NAND-style parallel devices, which is a much better match for the MD2202-D16 bus than bit-banging GPIO from Linux.

Second choice: BeagleBone Black plus CPLD/PRU. This is better if you want Linux on the appliance, but it adds bus-interface complexity.

Third choice: Teensy 4.1. This is a good low-cost bare-metal option with enough GPIO and microSD, but it lacks a true external memory controller.

## System block diagram

```text
+----------------------+       +------------------------+
|  Front panel controls |       |  TFT / OLED display    |
|  Clone Verify Read    |       |  Progress + status     |
|  Write + encoder      |       +-----------+------------+
+-----------+----------+                   |
            |                              |
            v                              v
+------------------------------------------------------+
| STM32H743 Nucleo-144 preferred controller            |
| FMC async memory interface + GPIO control/status     |
+-----------+-----------------------------+------------+
            |                             |
            v                             v
+--------------------------+   +-------------------------+
| SOURCE 32-pin ZIF socket |   | TARGET 32-pin ZIF socket|
| MD2202-D16               |   | MD2202-D16              |
+--------------------------+   +-------------------------+
```

## Print settings

- Material: PETG or PLA+
- Nozzle: 0.4 mm
- Layer height: 0.20 mm
- Walls/perimeters: 4
- Infill: 25%
- Supports: off for bottom tray; minimal support may be needed under bezels depending on slicer
- Heat-set inserts: M3

## STL files

- `stl/MD2202_cloner_bottom_tray.stl`
- `stl/MD2202_cloner_top_panel.stl`
- `stl/button_cap_clone.stl`
- `stl/button_cap_verify.stl`
- `stl/button_cap_read.stl`
- `stl/button_cap_write.stl`
- `stl/rotary_encoder_knob.stl`

## Assembly overview

```text
1. Print bottom tray and top panel.
2. Install M3 heat-set inserts into tray posts.
3. Mount controller with M3 screws and nylon washers.
4. Mount adapter/bus board over the controller.
5. Mount two 32-pin ZIF sockets: SOURCE left, TARGET right.
6. Install TFT display in the front-panel opening.
7. Install rotary encoder and four buttons.
8. Wire buttons, display, sockets, bus buffers, and power.
9. Check all rails and logic levels before inserting MD2202-D16 devices.
10. Run simulator before enabling real write routines.
```

## Electrical warnings

- Confirm whether your exact MD2202-D16 parts are 5 V or 3.3 V before inserting them.
- Do not directly connect unknown 5 V devices to 3.3 V MCU pins.
- Use 74LVC245 or equivalent bus transceivers for the 8-bit data bus.
- Add series resistors, decoupling, and power-current limiting during first bring-up.
- Never insert/remove DiskOnChip devices while powered.

## Wiring concept

Each socket needs:

- A0-A12
- D0-D7
- CE#
- OE#
- WE#
- RESET#
- VCC and GND

For the STM32H7 design, route source and target sockets through the FMC-capable pins and select each chip using separate chip-select lines. Use buffers so only the active socket drives the shared data bus.

## Cloning procedure

1. Power off the cloner.
2. Insert the original MD2202-D16 in SOURCE.
3. Insert the destination MD2202-D16 in TARGET.
4. Close both ZIF levers.
5. Power on.
6. Run **Detect** or self-test.
7. Confirm both devices identify as the same capacity.
8. Select **Clone**.
9. The cloner reads one logical block from SOURCE, writes it to TARGET, then reads the TARGET block back.
10. Any mismatch stops the job and displays the failing block number.
11. After clone completes, run **Verify** for a full source-to-target compare.
12. Power off before removing chips.

## Simulator test

```bash
cd src/python_cloner
python3 md2202_clone.py --backend sim clone
python3 md2202_clone.py --backend sim verify
```

## Real hardware bring-up sequence

1. Test the enclosure and dummy socket fit.
2. Test all button/display wiring.
3. Test address/control line toggling with no MD2202 installed.
4. Test voltage rails with dummy load.
5. Insert one sacrificial MD2202 in SOURCE only.
6. Implement and test read-only `identify`.
7. Implement read-only sector dump.
8. Add TARGET socket detection.
9. Only after read works reliably, implement writes on sacrificial parts.
10. Keep automatic verify-after-write enabled permanently.
