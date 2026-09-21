#!/bin/sh
#
# Seed a stock-Vial Iris CE with the cozy_de keymap.
#
# `cozy_de.vil` carries the layers, the nine macros, the thirteen key overrides and the Caps Word
# combo. It also carries a `settings` block, but vitaly's `load` does not apply it — only the
# Vial GUI does — so the two tap-hold settings are set explicitly further down. Running this
# after a GUI restore is harmless: every step is idempotent.
#
# Usage:  ./seed-cozy-de.sh            (keyboard plugged in, both halves)
#         VIAL_ID=1234 ./seed-cozy-de.sh
#
set -eu

# 0x1556 in decimal, the Iris CE product id from keebio/iris_ce/rev1/keyboard.json.
# Override if `vitaly devices` reports something else.
ID="${VIAL_ID:-5462}"

cd "$(dirname "$0")"

command -v vitaly > /dev/null || {
    echo "vitaly not found. Install it with 'cargo install vitaly' or from" >&2
    echo "https://github.com/bskaplou/vitaly/releases/latest" >&2
    exit 1
}
[ -f cozy_de.vil ] || { echo "cozy_de.vil is missing next to this script" >&2; exit 1; }

echo "== the keyboard vitaly sees =="
vitaly -i "$ID" devices

# Vial refuses to write macros, key overrides and combos while locked. The unlock keys are
# compiled in as matrix (0,0) and (9,5) — the Esc position and the right half's inner key
# next to the thumbs. vitaly draws which ones to hold and counts down.
echo
echo "== unlocking: hold the two keys vitaly marks until it says unlocked =="
vitaly -i "$ID" lock -u

echo
echo "== loading the keymap, macros, key overrides and the Caps Word combo =="
vitaly -i "$ID" load -f cozy_de.vil

# Not in what `load` writes, so set here. These are `config.h`'s TAPPING_TERM 500 and
# PERMISSIVE_HOLD, which stock Vial keeps as runtime settings instead of compiling in.
echo
echo "== tap-hold settings =="
vitaly -i "$ID" settings -q 7  -v 500
vitaly -i "$ID" settings -q 22 -v true

# RGB is per-taste and persists on its own, so nothing is forced here. To pin it down, add
# something like the line below: `-e` picks an effect from `vitaly -i "$ID" rgb -i`, and `-p`
# makes it survive a replug. Effect 2 is Solid Color.
#   vitaly -i "$ID" rgb -e 2 -c '#20304a' -p

echo
echo "== locking again =="
vitaly -i "$ID" lock -l

echo
echo "Done. Worth checking by hand: Shift+2 gives @, Shift+0 gives ?, holding Alt and tapping"
echo "ä walks the window list, both Shifts together toggle Caps Word, and the L_COMBINE thumb"
echo "keys type é and è."
