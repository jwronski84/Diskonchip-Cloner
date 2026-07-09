# 5-inch touchscreen selection

The enclosure and UI are now designed around a **5.0-inch TFT touchscreen**.

## Recommended display class

- Size: 5.0 inch
- Resolution: 800 x 480 preferred
- Interface: SPI or FSMC/8080 parallel depending on chosen module
- Touch: resistive preferred for easiest STM32 integration, capacitive acceptable if I2C controller is documented
- Typical active/display module size: about 120 x 75 mm
- Recommended enclosure cutout allowance: 122 x 77 mm
- Recommended bezel outside size: 132 x 87 mm

## Why 5 inch

The enclosure footprint is 220 x 165 mm to fit a 240 x 240 x 220 mm printer. A 5-inch display gives a professional-looking front panel while still leaving room for:

- SOURCE and TARGET ZIF socket access
- Two physical safety buttons
- Optional rotary encoder
- Status LEDs
- Internal NUCLEO-H753ZI and adapter board standoffs

## Control layout change

Because the display is now touch capable, the front panel should use fewer physical controls:

- Touchscreen: all menus, read/write/verify selections, progress, logs
- Physical CLONE button: quick-start clone action
- Physical CANCEL/BACK button: emergency stop/menu back
- Optional rotary encoder: retained only if desired for non-touch operation
- Status LEDs: Power, Busy, Pass, Fail

## Electrical notes

For lowest firmware complexity, select a display module with:

- Separate SPI for display and touch, or
- SPI display + XPT2046 resistive touch controller, or
- 8080/FSMC display interface + I2C/SPI touch controller

Avoid undocumented RGB-only panels unless a matching driver board is included.
