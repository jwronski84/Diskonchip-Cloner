# Professional enclosure for NUCLEO-H753ZI MD2202-D16 cloner

This enclosure is designed for a printer with a maximum build volume of **240 x 240 x 220 mm**.

Default enclosure size:

- Width: **220 mm**
- Depth: **165 mm**
- Bottom tray height: **38 mm**
- Sloped top-panel height: **18-45 mm**

The model is intentionally split into printable parts:

- Bottom tray
- Sloped top panel
- Button caps
- Rotary encoder knob

## Access cutouts

The top panel includes access openings for:

- SOURCE 32-pin ZIF socket
- TARGET 32-pin ZIF socket
- TFT display
- Rotary encoder
- READ / CLONE / VERIFY / WRITE buttons
- Front status LEDs

The rear wall includes access openings for:

- NUCLEO USB/ST-LINK connector area
- Optional 5 V power input
- microSD card access
- Optional debug/UART header slot

## Internal mounting

The bottom tray includes standoffs for:

- NUCLEO-H753ZI board
- MD2202 adapter/buffer board
- Display support posts
- Cable tie anchors
- Top-panel screw bosses

## Exporting STL files

Install OpenSCAD, then run:

```powershell
cd mechanical\enclosure
.\export_enclosure.ps1
```

Generated STL files are written to:

```text
mechanical/stl/generated/
```

## Important fit note

NUCLEO-H753ZI mounting-hole locations and final adapter board mounting must be checked against the exact board revision and final PCB. The current design uses a practical Nucleo-144 clearance pocket and adjustable standoff coordinates in the OpenSCAD file.
