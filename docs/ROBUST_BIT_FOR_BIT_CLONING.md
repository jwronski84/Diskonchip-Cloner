# Robust cloning and error-correction specification

## Goal

The cloner must be a standalone, robust, verification-first appliance for MD2202-D16 DiskOnChip 2000 devices.

The required workflow is:

1. Detect SOURCE and TARGET.
2. Confirm both are compatible MD2202-D16-class devices.
3. Format/prepare TARGET using TrueFFS-supported low-level command support once available.
4. Read SOURCE through the DiskOnChip/TrueFFS layer.
5. Save a verified image to SD2 first.
6. Write the SD2 image to TARGET.
7. Read TARGET back and compare against the SD2 image.
8. Report PASS only after a full byte-for-byte logical compare succeeds.

## Important terminology

### Logical bit-for-bit clone

This is the primary supported goal.

A logical bit-for-bit clone means every logical 512-byte sector exposed by the DiskOnChip/TrueFFS block-device layer is copied exactly and verified exactly.

This is the correct target for making a working duplicate because DiskOnChip 2000 uses TrueFFS for hard-disk emulation, bad-block management, wear-leveling, power-failure protection, and ECC behavior.

### Raw physical NAND clone

A raw physical NAND clone would copy hidden physical flash pages, bad-block tables, spare/OOB data, wear-leveling layout, and internal mapping structures.

This is not the primary design goal because DiskOnChip intentionally abstracts that physical layout behind TrueFFS. A physical raw clone may not be portable between devices and may conflict with the device's bad-block and wear-leveling management.

## Error-correction strategy

The datasheet states that DiskOnChip 2000 includes hardware Reed-Solomon EDC and that the TrueFFS driver implements complementary ECC. The design therefore requires the following:

- Use TrueFFS-compatible read/write routines whenever available.
- Surface EDC/ECC status in the UI.
- Never report PASS unless reads are ECC-clean or corrected by the supported layer.
- Log corrected errors, uncorrectable errors, retries, and failing block numbers.
- Refuse destructive operations if ECC/TrueFFS support is missing and the user selected strict mode.

## Robust read policy

For each SOURCE logical block:

1. Read block.
2. If the read reports corrected ECC, log it and continue.
3. If the read fails, retry up to `DOC_READ_RETRY_LIMIT` times.
4. If still failing, stop and show failing block.
5. Write the block to SD2 image.
6. Update rolling SHA-256 for the image.

Recommended defaults:

```c
#define DOC_READ_RETRY_LIMIT 5
#define DOC_WRITE_RETRY_LIMIT 3
#define DOC_VERIFY_RETRY_LIMIT 3
```

## Robust write policy

For each TARGET logical block:

1. Read one block from the verified SD2 image.
2. Write block to TARGET through TrueFFS-compatible command layer.
3. Read TARGET block back.
4. Compare against SD2 block.
5. Retry failed writes up to `DOC_WRITE_RETRY_LIMIT` times.
6. If still mismatched, stop and show failing block.

## Image-first safety

The appliance must never clone directly from SOURCE to TARGET as the only copy path.

The required order is:

```text
SOURCE -> SD2 image -> TARGET -> verify against SD2 image
```

This ensures that once a good source is read successfully, the appliance keeps a reusable golden image that can be written to additional TARGET devices later without needing the original SOURCE.

## SD2 image metadata

Each saved image must include:

- Raw `.docimg` logical image
- `.json` metadata sidecar
- `.sha256` checksum
- `.log` operation log

Minimum metadata fields:

```json
{
  "device_family": "DiskOnChip 2000 DIP",
  "expected_model": "MD2202-D16",
  "clone_type": "logical-bit-for-bit",
  "block_size": 512,
  "block_count": 32768,
  "image_bytes": 16777216,
  "sha256": "...",
  "ecc_mode": "TrueFFS/DiskOnChip EDC-ECC",
  "read_retries": 0,
  "corrected_errors": 0,
  "uncorrectable_errors": 0
}
```

## UI behavior

The UI must clearly show:

- SOURCE detected or missing.
- TARGET detected or missing.
- SOURCE/TARGET capacity match.
- SD1 mounted.
- SD2 mounted.
- TrueFFS/ECC support status.
- Current operation.
- Current block and total blocks.
- Stage percentage.
- Overall job percentage.
- Corrected error count.
- Retry count.
- PASS/FAIL.

## PASS criteria

The appliance may show PASS only if all of the following are true:

- SOURCE was detected and read completely.
- SD2 image was written completely.
- SD2 image checksum was finalized.
- TARGET was formatted/prepared successfully.
- TARGET was written completely.
- TARGET was read back completely.
- Every TARGET block matched the SD2 image exactly.
- No uncorrectable ECC errors occurred.

## FAIL criteria

Show FAIL and stop if any of these occur:

- SOURCE disappears.
- TARGET disappears.
- SD2 write fails.
- TrueFFS format command fails.
- Uncorrectable source read error.
- Target write failure after retries.
- Target verify mismatch after retries.
- Power-loss recovery detects an incomplete or unsafe operation.

## Power-loss recovery

Before every destructive operation, write an operation journal to SD1:

```text
job_id
state
source_id
target_id
image_name
last_completed_block
operation_phase
```

On boot, if an incomplete journal exists, show a recovery screen and do not resume destructive writing automatically. Require user confirmation.
