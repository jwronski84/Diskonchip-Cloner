# Important limitations

The MD2202-D16 DiskOnChip is not a simple EEPROM or raw NAND flash. It presents a memory-mapped interface and relies on DiskOnChip/TrueFFS/NFTL-style command handling. This package provides the mechanical design, BOM, safe simulator, and firmware architecture, but the final real-chip driver must be completed and validated against the MD2202-D16 datasheet and sacrificial hardware.

Do not use this to write irreplaceable devices until:

1. Device ID read is confirmed.
2. Read-only sector dumping is confirmed.
3. Voltage level is confirmed.
4. Write command sequence is confirmed.
5. Verify-after-write succeeds on sacrificial target chips.
6. Full image compare succeeds repeatedly.

The intended clone is a logical block-for-block duplicate, not a raw physical NAND/OOB duplicate.
