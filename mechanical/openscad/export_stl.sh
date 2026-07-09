#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")"
OUT="../stl/generated"
mkdir -p "$OUT"
for p in bottom top knob buttons; do
  openscad -o "$OUT/MD2202_cloner_${p}.stl" -D "part=\"$p\"" MD2202_enclosure.scad
done
echo "STL files written to $OUT"
