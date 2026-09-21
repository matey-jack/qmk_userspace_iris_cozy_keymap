# `cozy_de` on stock Vial

Three files that put the `cozy_de` keymap on an Iris CE running an unmodified
[Vial](https://get.vial.today/) build, with no keymap of our own compiled into the firmware.

| File | What it is |
| --- | --- |
| [`PLAN.md`](PLAN.md) | why this works, what changes, and every claim sourced |
| `cozy_de.vil` | the keymap itself: 4 layers, 9 macros, 13 key overrides, 1 combo |
| `seed-cozy-de.sh` | loads the `.vil` and sets what it does not carry |

`cozy_de.vil` was generated from
[`keymaps/cozy_de/keymap.c`](../keyboards/keebio/iris_ce/keymaps/cozy_de/keymap.c) and is the
thing to edit from here on — in the Vial GUI, then *File → Save current layout* back over this
file. It is not regenerated from the C keymap, so the two drift apart once you change either.
As of the L_COMBINE cleanup and the issue #14 fix on `main`, they are in sync; §8 of `PLAN.md`
records what was carried over.

## Using it

Build and flash the stock Vial firmware once, on **both halves**. There is no prebuilt binary
for the Iris CE, so this is one `make` from a `vial-qmk` checkout:

```sh
git clone --recurse-submodules https://github.com/vial-kb/vial-qmk.git
cd vial-qmk
make keebio/iris_ce/rev1:vial
```

Then either open `cozy_de.vil` in the Vial GUI (*File → Load saved layout*), or:

```sh
cargo install vitaly      # or a binary from https://github.com/bskaplou/vitaly/releases
./seed-cozy-de.sh
```

The GUI route applies the `settings` block too; `vitaly load` does not, which is why the script
sets the tapping term and permissive hold explicitly afterwards.

## What differs from the compiled keymap

Four deliberate differences, all decided rather than discovered — §3 and §4 of `PLAN.md` have
the reasoning:

- **Caps Word** is a `KC_LSFT + KC_RSFT → CW_TOGG` combo instead of
  `BOTH_SHIFTS_TURNS_ON_CAPS_WORD`, which stock Vial has no runtime switch for. It *toggles*, so
  both Shifts again turns it off, and stock Vial compiles in `CAPS_WORD_INVERT_ON_SHIFT`, so
  Shift inside Caps Word gives a lowercase letter.
- **The accent macros do not clear modifiers.** `send_prefixed_key()` did; a Vial macro cannot.
  Hold Shift while pressing the é key and you get è, because ´ and ` share a key on the German
  layout. They are minuscule-only macros, so this only bites when you were already off-script.
- **`MX_VERS` is gone.** It printed the firmware version and build date, and there is no longer a
  per-keymap build to identify.
- **`ENABLE_RGB_MATRIX_KEY_GROUPS` is gone.** That colour mode lives in a fork of QMK, so no
  stock Vial build has it. The 44 standard RGB matrix effects are all available instead.

Everything else — the Cozy Shift mapping of the number row, the ä/Tab chameleon key, the five
accented letters, the Level-5 characters, the 500 ms tapping term — carries over unchanged.
