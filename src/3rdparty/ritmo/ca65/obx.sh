#!/bin/sh
# obx.sh out source.s [ca65 options...] - assemble a ca65 source of asm/ca65 into an Atari binary file as MADS
# does: two passes of ca65 (the first finds the empty segments, see mads.inc), then ld65 with obx.cfg.
#   asm/ca65/obx.sh build/tracker.obx Patch-16/rmtplayr.s -D FEAT_IS_TRACKER=1
# OBX_SYMBOLS="name ...": also print "OBX_SYMBOL name XXXX" for each symbol (the addresses a tracker patches).
# OBX_DBG=file: also write the ld65 debug file, with every symbol (obxheader.py makes the C header of MADS -hc of it).
set -e
[ $# -ge 2 ] || { echo "usage: $0 out source.s [ca65 options...]" >&2; exit 2; }
out=$(cd "$(dirname "$1")" && pwd)/$(basename "$1")
src=$2
shift 2
here=$(cd "$(dirname "$0")" && pwd)
srcdir=$(cd "$(dirname "$src")" && pwd)
# the binary files (ins: fonts, songs) are those next to the MADS source
madsdir=$(echo "$srcdir" | sed 's#/ca65/#/#')
tmp=$(mktemp -d)
trap 'rm -rf "$tmp"' EXIT

printf '.include "mads.inc"\n.include "%s/%s"\nOBX_END\n' "$srcdir" "$(basename "$src")" >"$tmp/main.s"
for sym in ${OBX_SYMBOLS:-}; do
    printf '.out .sprintf("OBX_SYMBOL %s %%04X", %s)\n' "$sym" "$sym" >>"$tmp/main.s"
done
: >"$tmp/obx_empty.inc"
# from the temporary folder: ca65 2.19 puts the folder of the main file before an absolute include name
asm() {
    (cd "$tmp" && ca65 -i -t none ${OBX_DBG:+-g} -I "$here" -I "$srcdir" -I "$tmp" --bin-include-dir "$madsdir" "$@" -o main.o main.s)
}
asm "$@" >"$tmp/pass1.txt"
awk '$1 == "OBX_EMPTY" { print "OBX_EMPTY_" $2 " = 1" }' "$tmp/pass1.txt" >"$tmp/obx_empty.inc"
asm "$@" >"$tmp/pass2.txt"
grep -v '^OBX_EMPTY ' "$tmp/pass2.txt" || true
ld65 -C "$here/obx.cfg" ${OBX_DBG:+--dbgfile "$OBX_DBG"} -o "$out" "$tmp/main.o"
