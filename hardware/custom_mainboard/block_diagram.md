# Custom mainboard block diagram

```mermaid
flowchart TB
    PWR[5V DC / USB-C Power Input]
    REG33[3.3V Logic Regulator]
    REGDOC[Selectable DiskOnChip VCC Rail\n3.3V or verified 5V]
    MCU[STM32H753 MCU]
    XTAL[HSE/LSE Clock Sources]
    SWD[SWD Programming Header]
    USB[USB-C Service Port]
    TFT[5.0in Touchscreen Connector]
    SD1[SD1 System microSD]
    SD2[SD2 Image microSD]
    BUSBUF[Address/Data/Control Buffers\n74LVC / LVC8T Series]
    SRC[Source 32-pin ZIF\nMD2202-D16]
    TGT[Target 32-pin ZIF\nMD2202-D16]
    WPSRC[Source Hardware Write Protect]
    WETGT[Target Write Enable Gate]
    BTN[Clone + Cancel/Back Buttons]
    LED[Power/Busy/Pass/Fail LEDs]
    FAN[40mm Fan Connector\nPWM/Tach]
    TP[Test Points\nA/D/CE/OE/WE]

    PWR --> REG33
    PWR --> REGDOC
    REG33 --> MCU
    REG33 --> TFT
    REG33 --> SD1
    REG33 --> SD2
    REG33 --> BUSBUF
    REGDOC --> SRC
    REGDOC --> TGT
    MCU --- XTAL
    MCU --- SWD
    MCU --- USB
    MCU --> TFT
    MCU --> SD1
    MCU --> SD2
    MCU <--> BUSBUF
    BUSBUF <--> SRC
    BUSBUF <--> TGT
    WPSRC --> SRC
    WETGT --> TGT
    MCU --> WETGT
    BTN --> MCU
    MCU --> LED
    MCU --> FAN
    TP --- BUSBUF
```

## Bus philosophy

- Shared address bus to both sockets.
- Shared data bus through bidirectional transceiver.
- Separate CE# for source and target.
- SOURCE WE# physically held inactive except during special diagnostic mode that should not be populated by default.
- TARGET WE# gated through firmware-controlled and hardware-latched write-enable logic.

## Storage philosophy

- SD1 is system/config/log/update storage.
- SD2 is image/archive storage.
- Clone workflow always uses `SOURCE -> SD2 -> TARGET -> VERIFY`.
