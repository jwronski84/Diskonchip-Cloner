# MD2202-D16 Cloner Software Prototype

This is a buildable software skeleton for a logical block cloner. It is intentionally split into a safe simulator backend and a hardware backend placeholder.

Use `SimDocBackend` first to test the UI and verify/compare routines. Replace `HardwareDocBackend` register access methods after probing the MD2202-D16 command/register behavior on the final board.

## Run simulator

```bash
python3 md2202_clone.py --backend sim --source sim_source.bin --target sim_target.bin --blocks 32768 --block-size 512 clone
python3 md2202_clone.py --backend sim --source sim_source.bin --target sim_target.bin --blocks 32768 --block-size 512 verify
```

32768 × 512 bytes = 16 MiB.
