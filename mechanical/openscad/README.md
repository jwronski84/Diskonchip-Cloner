# OpenSCAD professional enclosure

This folder contains a parametric OpenSCAD enclosure for the MD2202-D16 cloner.

Your printer build volume is **240 x 240 x 220 mm**, so the default enclosure footprint is set to **220 x 160 mm** with a height under 70 mm. This leaves margin on the print bed and allows the bottom tray and top panel to be printed as separate parts.

## Parts

- `MD2202_enclosure.scad` - main enclosure model
- `export_stl.ps1` - Windows PowerShell STL export helper
- `export_stl.sh` - Linux/macOS STL export helper

## Export

Install OpenSCAD, then run:

```powershell
cd mechanical\openscad
.\export_stl.ps1
```

or:

```bash
cd mechanical/openscad
bash export_stl.sh
```

Generated files go to `mechanical/stl/generated/`.
