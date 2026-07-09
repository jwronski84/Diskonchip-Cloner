# Touchscreen UI workflow

The cloner is a standalone touchscreen appliance. Normal use must not require a computer.

## Home / detection screen

The home screen continuously polls both ZIF sockets and shows disk recognition state:

```text
SOURCE:  Detected MD2202-D16 16MB     [green]
TARGET:  Detected MD2202-D16 16MB     [green]
SD1:     System card mounted          [green]
SD2:     Image card mounted           [green]

[Format Target]
```

If either disk is missing, incompatible, or unknown, destructive actions stay hidden/disabled:

```text
SOURCE:  Not detected                 [red]
TARGET:  Detected MD2202-D16 16MB     [green]

Insert source disk to continue.
```

## Workflow states

1. `WAIT_FOR_MEDIA`
   - Show SOURCE/TARGET/SD status.
   - Hide **Format Target** until SOURCE and TARGET are both detected and capacity-compatible.

2. `READY_TO_FORMAT`
   - Show **Format Target** button.
   - Warn that the target will be erased/prepared.

3. `FORMAT_TARGET_CONFIRM`
   - User must confirm destructive operation.
   - Require TARGET present and SOURCE write-protected.

4. `FORMAT_TARGET_RUNNING`
   - Show progress bar and percent.
   - Label: `Formatting target... XX%`
   - Cancel is disabled once low-level format begins unless the command layer supports safe abort.

5. `READY_TO_CLONE`
   - Show **Clone Source to Target** button only after formatting completes.

6. `BACKUP_SOURCE_TO_SD2`
   - Read SOURCE logical blocks and write image to SD2 first.
   - Show progress bar: `Backing up source to SD2... XX%`
   - Generate `.docimg`, `.json`, `.sha256`, and `.log` files.

7. `CLONE_SD2_TO_TARGET`
   - Write the just-created SD2 image to TARGET.
   - Show progress bar: `Cloning SD2 image to target... XX%`

8. `VERIFY_TARGET`
   - Read TARGET back and compare against SD2 image.
   - Show progress bar: `Verifying target... XX%`

9. `COMPLETE`
   - Show `Clone complete: PASS`.
   - Show image filename saved on SD2.

10. `ERROR`
   - Show failing operation, failing block, and next safe action.

## UI buttons

### Home state

- Format Target: visible only when SOURCE and TARGET are recognized and compatible.
- Image Library: visible when SD2 is mounted.
- Diagnostics: always visible.

### After format

- Clone Source to Target: visible only after successful format.
- Reformat Target: visible but requires confirmation.

### Image library

- Write saved image to target
- Verify target against saved image
- Delete image
- View image metadata

## Progress percentage

For operations based on logical blocks:

```c
percent = (completed_blocks * 100) / total_blocks;
```

For multi-stage clone operation, show both stage percent and total job percent:

```text
Stage 1/3: Backup source to SD2   42%
Overall job                       14%
```

## TrueFFS / format requirement

The project requires obtaining legally usable TrueFFS low-level format/command support before implementing real low-level target formatting.

The datasheet states that TrueFFS provides hard-disk read/write emulation, bad-block management, wear-leveling, power-failure management, and extended IOCTL features including formatting. It also states that the TrueFFS SDK can be used in an OS-less environment.

Until TrueFFS-compatible low-level format support is obtained and integrated, the firmware must label the operation as **Prepare/Overwrite Target** rather than claiming a true factory low-level format.

## EDC/ECC requirement

The DiskOnChip 2000 includes Reed-Solomon-based on-the-fly EDC hardware and relies on the TrueFFS driver for complementary ECC behavior. The UI should show:

```text
EDC/ECC: Enabled via DiskOnChip + TrueFFS layer
```

If the TrueFFS/ECC layer is not present, the UI must show:

```text
EDC/ECC: Incomplete - TrueFFS integration required
```
