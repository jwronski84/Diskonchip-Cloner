# Standalone operation requirement

The cloner is intended to be a complete standalone appliance after initial firmware programming.

No PC should be required for normal use.

## Primary standalone workflows

### 1. Source-to-target clone with archive

1. Insert known-good MD2202-D16 into SOURCE.
2. Insert destination MD2202-D16 into TARGET.
3. Insert SD2 image/archive card.
4. Power on.
5. Touchscreen shows detected SOURCE and TARGET.
6. User selects **Clone Source to Target**.
7. Appliance confirms the operation is destructive to TARGET.
8. Appliance prepares/formats/initializes TARGET as required by the supported DiskOnChip command layer.
9. Appliance reads SOURCE logical blocks.
10. Appliance writes TARGET logical blocks.
11. Appliance verifies TARGET block-by-block.
12. Appliance also saves a verified image file to SD2.
13. Appliance stores a log on SD1.

### 2. Clone from saved SD image

1. Insert blank or reusable MD2202-D16 into TARGET.
2. Insert SD2 containing a verified image.
3. Power on.
4. Select **Write Image to Target**.
5. Select image file.
6. Appliance prepares/formats/initializes TARGET.
7. Appliance writes image to TARGET.
8. Appliance reads TARGET back and verifies against SD image.

### 3. Read source to SD image only

1. Insert SOURCE.
2. Insert SD2.
3. Select **Read Source to Image**.
4. Appliance reads all logical blocks and saves an image with checksum metadata.

### 4. Verify target against image

1. Insert TARGET.
2. Insert SD2 containing image.
3. Select **Verify Target Against Image**.
4. Appliance compares every logical block.

## Two-card storage design

- **SD1 system card:** configuration, logs, firmware-update package staging, diagnostics, UI assets.
- **SD2 image card:** source images, verified golden images, clone jobs, checksums.

The STM32 firmware itself runs from internal flash. SD1 is not a boot disk; it is a service/config/log card.

## Image file format

Each image stored on SD2 should include:

- Raw logical block image: `.docimg`
- Metadata sidecar: `.json`
- SHA-256 checksum file: `.sha256`
- Operation log: `.log`

Suggested naming:

```text
/images/MD2202D16_YYYYMMDD_HHMMSS.docimg
/images/MD2202D16_YYYYMMDD_HHMMSS.json
/images/MD2202D16_YYYYMMDD_HHMMSS.sha256
```

A 16MB MD2202 image is small. Even an 8GB card can hold hundreds of images.

## Important note about formatting

The datasheet describes DiskOnChip 2000 as a TrueFFS-managed flash disk. A normal user-level clone should copy the logical block image. A true factory-level format/low-level initialization may require M-Systems TrueFFS utilities, SDK behavior, or exact controller command support.

For this project, **format target** means:

1. Confirm target identity and capacity.
2. Confirm write support is enabled only for TARGET.
3. Prepare the target using the supported DiskOnChip command layer.
4. Overwrite all logical blocks with the source image.
5. Verify all blocks.

If the final command layer supports an official format/initialize command, the firmware should call it before writing. If it does not, the firmware should warn the user and proceed with full logical overwrite only.
