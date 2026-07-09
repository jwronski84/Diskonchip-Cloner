"""Board-level notes for MD2202-D16 bus access.

Signals expected by the adapter board:
- A0..A12 address outputs
- D0..D7 bidirectional data
- CE#, OE#, WE#, RESET# controls

The MD2202 DiskOnChip behaves like a memory-mapped peripheral, not a raw NAND flash.
Implement the read8/write8 primitives first, then implement the DiskOnChip command
sequence from the official datasheet/driver source before enabling real writes.
"""

def read8(address: int) -> int:
    raise NotImplementedError

def write8(address: int, value: int) -> None:
    raise NotImplementedError
