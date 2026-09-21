# `cozy_de` key override tests

A regression test for [issue #14](https://github.com/matey-jack/qmk_userspace_iris_cozy_keymap/issues/14):
a key override whose replacement is itself a shifted keycode must keep that Shift on the side the
user really pressed. `ko_make_basic(MOD_MASK_SHIFT, ...)` does not: it suppresses whichever Shift
was held and re-adds a *left* one, so pressing the key with the **right** Shift swaps the modifier
byte from RSFT to LSFT in the same HID report that presses the replacement key. A host that
resolves the keycode before the modifier delta then types the unshifted character — `2` for `"`,
`6` for `&`, `+` for `*`, `ß` for `?`.

The test drives the real QMK key override machinery on the host, with no hardware and no ARM
toolchain, and asserts for each of the four affected pairings and for **both** Shift keys that

- a report carrying the replacement key together with that same Shift is sent, and
- a report carrying the replacement key *unshifted* is never sent.

## Running it

```sh
git clone --depth 1 https://github.com/qmk/qmk_firmware.git
cd qmk_firmware
git submodule update --init --depth 1 lib/googletest lib/printf
cp -r ../tests/cozy_de_key_overrides tests/
make SKIP_GIT=yes test:cozy_de_key_overrides
```

Reverting the four `ko_shifted_pair()` overrides in `cozy_de/keymap.c` to
`ko_make_basic(MOD_MASK_SHIFT, ...)` makes exactly the four `RightShift` cases fail, which is what
makes this a regression test rather than a smoke test.

## Keeping it honest

`keymap.c` here mirrors the key override block of
`keyboards/keebio/iris_ce/keymaps/cozy_de/keymap.c`; the test keyboard's 4x10 matrix and the
`LAYOUT` macro of the Iris are incompatible, so the real file cannot simply be included. Change the
overrides in the keymap and this mirror needs the same change. Moving the override block into a
shared header that both files include would remove the duplication, at the cost of splitting the
keymap into two files.
