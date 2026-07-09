#!/usr/bin/env python3
"""MD2202-D16 logical block cloner prototype.

This utility performs block-for-block logical cloning through a backend interface.
The included simulator is safe and complete. The hardware backend is a TODO layer
for the final MD2202 register protocol and bus driver.
"""
from __future__ import annotations
import argparse, hashlib
from pathlib import Path

class BlockDevice:
    def identify(self) -> str: raise NotImplementedError
    def read_block(self, block: int) -> bytes: raise NotImplementedError
    def write_block(self, block: int, data: bytes) -> None: raise NotImplementedError
    def block_count(self) -> int: raise NotImplementedError
    def block_size(self) -> int: raise NotImplementedError

class SimDocBackend(BlockDevice):
    def __init__(self, path: Path, blocks: int, block_size: int, seed: bool=False):
        self.path = path; self.blocks = blocks; self.bs = block_size
        if not path.exists():
            with path.open('wb') as f:
                for i in range(blocks):
                    payload = (hashlib.sha256(f'MD2202-SIM-{i}'.encode()).digest() * ((block_size//32)+1))[:block_size] if seed else bytes([0xFF])*block_size
                    f.write(payload)
    def identify(self) -> str: return f'SIM-MD2202-D16 {self.blocks*self.bs} bytes'
    def read_block(self, block: int) -> bytes:
        if block < 0 or block >= self.blocks: raise ValueError('block out of range')
        with self.path.open('rb') as f:
            f.seek(block*self.bs); return f.read(self.bs)
    def write_block(self, block: int, data: bytes) -> None:
        if len(data) != self.bs: raise ValueError('wrong block size')
        if block < 0 or block >= self.blocks: raise ValueError('block out of range')
        with self.path.open('r+b') as f:
            f.seek(block*self.bs); f.write(data)
    def block_count(self) -> int: return self.blocks
    def block_size(self) -> int: return self.bs

class HardwareDocBackend(BlockDevice):
    def __init__(self, role: str, blocks: int=32768, block_size: int=512):
        self.role=role; self.blocks=blocks; self.bs=block_size
        raise RuntimeError('Hardware backend is a board-specific placeholder. Implement MD2202 register access first.')
    def identify(self) -> str: return f'MD2202-D16 {self.role}'
    def read_block(self, block: int) -> bytes: raise NotImplementedError
    def write_block(self, block: int, data: bytes) -> None: raise NotImplementedError
    def block_count(self) -> int: return self.blocks
    def block_size(self) -> int: return self.bs

def digest_block(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()

def clone(source: BlockDevice, target: BlockDevice) -> None:
    if source.block_count()!=target.block_count() or source.block_size()!=target.block_size():
        raise SystemExit('Source/target geometry mismatch')
    print('SOURCE:', source.identify())
    print('TARGET:', target.identify())
    total=source.block_count()
    for b in range(total):
        data=source.read_block(b)
        target.write_block(b,data)
        rb=target.read_block(b)
        if rb!=data:
            raise SystemExit(f'Verify failed immediately after write at block {b}')
        if b % 256 == 0 or b == total-1:
            print(f'clone {b+1}/{total} blocks', flush=True)
    print('CLONE PASS')

def verify(source: BlockDevice, target: BlockDevice) -> None:
    if source.block_count()!=target.block_count() or source.block_size()!=target.block_size():
        raise SystemExit('Source/target geometry mismatch')
    total=source.block_count()
    for b in range(total):
        a=source.read_block(b); c=target.read_block(b)
        if a != c:
            raise SystemExit(f'VERIFY FAIL at block {b}: src={digest_block(a)} tgt={digest_block(c)}')
        if b % 512 == 0 or b == total-1:
            print(f'verify {b+1}/{total} blocks', flush=True)
    print('VERIFY PASS')

def main(argv=None):
    p=argparse.ArgumentParser()
    p.add_argument('command', choices=['clone','verify'])
    p.add_argument('--backend', choices=['sim','hardware'], default='sim')
    p.add_argument('--source', default='sim_source.bin')
    p.add_argument('--target', default='sim_target.bin')
    p.add_argument('--blocks', type=int, default=32768)
    p.add_argument('--block-size', type=int, default=512)
    args=p.parse_args(argv)
    if args.backend == 'sim':
        src=SimDocBackend(Path(args.source), args.blocks, args.block_size, seed=True)
        tgt=SimDocBackend(Path(args.target), args.blocks, args.block_size, seed=False)
    else:
        src=HardwareDocBackend('source'); tgt=HardwareDocBackend('target')
    if args.command == 'clone': clone(src,tgt)
    else: verify(src,tgt)
if __name__ == '__main__': main()
