# Plan: running `cozy_de` on stock Vial firmware

**Status:** files written, not yet tried on hardware. Written 2026-09-19, and re-synced with
`main` on 2026-09-21 — see [§8](#8-re-sync-with-main).
The decisions this plan asked for have been made — see [§7](#7-decisions). What is left is
flashing the firmware and the test pass in [§6](#6-order-of-work).

**Goal (as stated):** flash unmodified Vial firmware onto the Iris CE, then seed it with the
`cozy_de` keymap from a `.vil` file plus a small script that sets the things a `.vil` does not
carry. No custom `keymap.c` to maintain, no per-change recompile.

**Verdict: feasible.** Every keycode, all four layers and all thirteen key overrides of `cozy_de`
can be expressed in a `.vil` file, and the two `config.h` settings that matter at typing time
(`TAPPING_TERM`, `PERMISSIVE_HOLD`) are runtime-settable over the Vial protocol. Four things do
not survive the move; they are listed under [What is lost](#4-what-is-lost) and none of them is
a character you can no longer type.

There is one practical catch up front: **nobody ships a prebuilt Vial binary for the Iris CE**,
so "flash stock Vial" still means running `make` once. See [step 1](#step-1-get-the-firmware).

Terms used below, defined at first use:

- **Vial** — a fork of QMK plus a configurator GUI that keeps the whole keymap in EEPROM
  (the keyboard's non-volatile memory) instead of in compiled firmware, so it can be changed
  live over USB. <https://get.vial.today/>
- **VIA** — the older configurator protocol Vial extends. Vial speaks VIA plus its own commands.
- **`.vil`** — Vial's layout-export file. Despite the extension it is plain JSON.
- **`vitaly`** — a third-party command-line client for the VIA/Vial protocol, written in Rust.
  <https://github.com/bskaplou/vitaly>
- **qsid** — "QMK setting identifier", the numeric id Vial uses to address one runtime setting.
- **E1** — the German extended keyboard layout, xkb's `de(e1)`, standardised as DIN 2137-1:2020-11.
  Same meaning as in [`keymaps/cozy_de/keymap.c`](../keyboards/keebio/iris_ce/keymaps/cozy_de/keymap.c).

---

## 1. What the stock Vial firmware already gives us

`vial-qmk` has an Iris CE port already: `keyboards/keebio/iris_ce/keymaps/vial/` with a
`vial.json`, a keyboard UID and `VIALRGB_ENABLE` for the per-key RGB.

```
# keyboards/keebio/iris_ce/keymaps/vial/rules.mk
VIA_ENABLE = yes
VIAL_ENABLE = yes
VIALRGB_ENABLE = yes
```
<https://github.com/vial-kb/vial-qmk/blob/vial/keyboards/keebio/iris_ce/keymaps/vial/rules.mk>

Everything else we need is switched on by Vial's own build fragment, without touching the keymap:

```make
# builddefs/build_vial.mk
QMK_SETTINGS       ?= yes
TAP_DANCE_ENABLE   ?= yes
CAPS_WORD_ENABLE   ?= yes
COMBO_ENABLE       ?= yes
KEY_OVERRIDE_ENABLE ?= yes
LAYER_LOCK_ENABLE  ?= yes
REPEAT_KEY_ENABLE  ?= yes
...
OPT_DEFS += -DVIAL_ENABLE -DNO_DEBUG -DSERIAL_NUMBER=\"vial:f64c2b3c\" -DCAPS_WORD_INVERT_ON_SHIFT
```
<https://github.com/vial-kb/vial-qmk/blob/vial/builddefs/build_vial.mk>

So **key overrides are on by default** — that was the single biggest risk, since the Cozy Shift
mapping of the number row is thirteen key overrides. Slot counts are derived from EEPROM size; on a
board with room (the Iris CE is RP2040 with flash-backed EEPROM) that is 32 key-override slots,
32 combos and 32 tap dances.
<https://github.com/vial-kb/vial-qmk/blob/vial/quantum/vial.h>

Sizes that have to fit:

| Resource | Stock Vial default | `cozy_de` needs | Fits |
| --- | --- | --- | --- |
| Layers (`DYNAMIC_KEYMAP_LAYER_COUNT`) | 4 | 4 | exactly |
| Key override slots | 32 | 13 | yes |
| Macro slots | 16 | 9 | yes |
| Combo slots | 32 | 1 (see §3) | yes |

The layer count is the tight one. The default is 4
(<https://github.com/vial-kb/vial-qmk/blob/vial/quantum/dynamic_keymap.h>, line 26) and
`cozy_de` uses exactly 4 — `L_BASE`, `L_COMBINE`, `L_ALTGR`, `L_FN`. The `#define
DYNAMIC_KEYMAP_LAYER_COUNT 5` in our `config.h` is one more than the keymap actually declares,
so nothing is lost. **But there is no headroom for a fifth layer without recompiling.**

---

## 2. What transfers exactly

### 2.1 Keycodes

All of them. The German aliases in `keymap_german.h` are not special keycodes — they resolve to
plain US keycodes, `S(...)` or `ALGR(...)`:

```c
#define DE_ADIA KC_QUOT      // Ä
#define DE_Z    KC_Y         // Z (the Qwertz swap)
#define DE_QUES S(DE_SS)     // ?
#define DE_AT   ALGR(DE_Q)   // @
```

A `.vil` stores each key as a **string** and Vial parses expressions with `simpleeval`, so
`"ALGR(KC_Q)"`, `"LSFT(KC_2)"`, `"LT(2,KC_DEL)"`, `"OSL(1)"`, `"TO(0)"` and `"G(C(KC_LEFT))"`
all work as written. `ALGR` is registered as an alias of `RALT`.
<https://github.com/vial-kb/vial-gui/blob/main/src/main/python/any_keycode.py>

I checked every keycode that appears in `cozy_de/keymap.c` against Vial GUI's keycode table.
**Two need a different spelling** — same keycode, older alias:

| In `keymap.c` | In the `.vil` | Why |
| --- | --- | --- |
| `MS_WHLD` / `MS_WHLU` | `KC_WH_D` / `KC_WH_U` | Vial's table predates the `MS_*` rename; `KC_MS_WH_DOWN` also works |
| `OS_LSFT`, `OS_RALT`, … | `OSM(MOD_LSFT)`, `OSM(MOD_RALT)`, … | Vial has no `OS_*` shorthand, but `OSM()` and the `MOD_*` names are in scope |

Everything else — `RM_TOGG`, `RM_HUED`, `EE_CLR`, `QK_BOOT`, `KC_NUHS`, `KC_NUBS`, the whole
number row, all the navigation and media keys — is in the table under its own name.

### 2.2 Key overrides

Vial's `vial_key_override_entry_t` has exactly the fields our thirteen overrides use: trigger,
replacement, layer bitmask, trigger mods, negative mod mask, suppressed mods, and an options
byte. In the `.vil` the two keycodes are strings and the rest are integers.
<https://github.com/vial-kb/vial-gui/blob/main/src/main/python/protocol/key_override.py>

The options byte packs as: bit 0 `activation_trigger_down`, bit 1 `activation_required_mod_down`,
bit 2 `activation_negative_mod_up`, bit 3 `one_mod`, bit 4 `no_reregister_trigger`,
bit 5 `no_unregister_on_other_key_down`, bit 7 `enabled`.

`ko_make_basic(mods, trig, repl)` expands to `layers = ~0`, `negative_mod_mask = 0`,
`suppressed_mods = trigger_mods` and `options = ko_options_all_activations`
(<https://github.com/vial-kb/vial-qmk/blob/vial/quantum/process_keycode/process_key_override.h>),
which gives:

| Override | trigger | replacement | trigger_mods | suppressed_mods | options |
| --- | --- | --- | --- | --- | --- |
| the four plain `ko_make_basic` ones | e.g. `KC_2` | e.g. `ALGR(KC_Q)` | `0x22` (both shifts) | `0x22` | `0x87` |
| each `ko_shifted_pair()` left half | e.g. `KC_7` | e.g. `LSFT(KC_6)` | `0x02` (left shift) | `0x02` | `0x87` |
| each `ko_shifted_pair()` right half | e.g. `KC_7` | e.g. `RSFT(KC_6)` | `0x20` (right shift) | `0x20` | `0x87` |
| `ko_adia_tab` (the ä/Tab chameleon) | `KC_QUOT` | `KC_TAB` | `0xDD` (Ctrl/Alt/Gui, both sides) | `0x00` | `0x8F` |

Four of the overrides are two entries rather than one, and that is not cosmetic — it is
[issue #14](https://github.com/matey-jack/qmk_userspace_iris_cozy_keymap/issues/14), which
applies here in full. Whenever the replacement is itself a shifted keycode (`DE_DQUO` is
`S(DE_2)`, `DE_AMPR` is `S(DE_6)`, `DE_ASTR` is `S(DE_PLUS)`, `DE_QUES` is `S(DE_SS)`), a single
`MOD_MASK_SHIFT` override suppresses the Shift that was really held and re-adds a *left* one.
Held with the **right** Shift that is a side swap, and the host can resolve the replacement
keycode before the modifier delta and type the unshifted character instead: `2` for `"`, `6` for
`&`, `+` for `*`, `ß` for `?`.

`vial-qmk` is on the near side of the fix. QMK master now sends the modifier change in its own
report before adding the replacement key; `vial-qmk`'s copy of `process_key_override.c` still
calls `add_key()` with no report in between, so mods and key go out together and the race is
live. Compare the `register_replacement` block in
<https://github.com/qmk/qmk_firmware/blob/master/quantum/process_keycode/process_key_override.c>
with <https://github.com/vial-kb/vial-qmk/blob/vial/quantum/process_keycode/process_key_override.c>.

The fix is pure data, so it transfers: one entry per Shift side, each rebuilding the replacement
on the side that was really pressed (`ko_shifted_pair()` in
[`key_overrides.h`](../keyboards/keebio/iris_ce/keymaps/cozy_de/key_overrides.h)). The modifier
byte is then identical before and after, so there is no delta to race with. `RSFT` is in Vial's
keycode-expression table next to `LSFT` and `RALT`, so `"RSFT(KC_6)"` parses like any other
string here (<https://github.com/vial-kb/vial-gui/blob/main/src/main/python/any_keycode.py>).

`0x8F` is `0x87` plus bit 3, `one_mod` — which is exactly what the comment in `keymap.c` asks
for: any one of Ctrl, Alt or Gui suffices, and nothing is suppressed so the held modifier stays
in the report and Alt-Tabbing through windows keeps working. `layers` is `0xFFFF` in both rows.

So the chameleon key survives intact. That was the second-biggest risk.

### 2.3 The tap-hold settings

`TAPPING_TERM` and `PERMISSIVE_HOLD` are in Vial's runtime settings struct, so the 500 ms
tapping term and permissive hold are set over USB rather than compiled in.
<https://github.com/vial-kb/vial-qmk/blob/vial/quantum/qmk_settings.h>

| `config.h` line | qsid | Default | Wanted |
| --- | --- | --- | --- |
| `#define TAPPING_TERM 500` | `7` | 200 | 500 |
| `#define PERMISSIVE_HOLD` | `22` | false | true |

---

## 3. What needs a different expression

### 3.1 The nine custom keycodes become Vial macros

Vial macros support `Tap`, `Down`, `Up`, `Text` and `Delay`, which covers the
`prefixed_keys[]` table in `keymap.c` — each entry is a dead key (or E1's Level-5 latch)
followed by one more key, with a 10 ms gap.

| Keycode | Vial macro slot | Macro body (vitaly syntax) |
| --- | --- | --- |
| `MX_EACU` é | `M0` | `Tap(KC_EQL); Delay(10); Tap(KC_E)` |
| `MX_EGRV` è | `M1` | `Tap(LSFT(KC_EQL)); Delay(10); Tap(KC_E)` |
| `MX_AGRV` à | `M2` | `Tap(LSFT(KC_EQL)); Delay(10); Tap(KC_A)` |
| `MX_NTIL` ñ | `M3` | `Tap(ALGR(KC_I)); Delay(10); Tap(KC_N)` |
| `MX_CCED` ç | `M4` | `Tap(ALGR(KC_J)); Delay(10); Tap(KC_C)` |
| `MX_HAT` ^ | `M5` | `Tap(KC_GRV); Delay(10); Tap(KC_SPC)` |
| `MX_BTIC` ` | `M6` | `Tap(LSFT(KC_EQL)); Delay(10); Tap(KC_SPC)` |
| `MX_CENT` ¢ | `M7` | `Tap(ALGR(KC_F)); Delay(10); Tap(KC_C)` |
| `MX_PND` £ | `M8` | `Tap(ALGR(KC_F)); Delay(10); Tap(KC_EQL)` |

In the layers they are referenced as `"M0"` … `"M8"` (Vial defines `M0..M15` as
`QK_MACRO + n`). The keycodes above are the US positions the German aliases resolve to:
`DE_ACUT` is `KC_EQL`, `DE_GRV` is `S(DE_ACUT)`, `DE_CIRC` is `KC_GRV`, `DE_DTIL` is
`ALGR(DE_I)`, `DE_DCED` is `ALGR(DE_J)`, `DE_LVL5` is `ALGR(DE_F)`.

**The one behavioural difference:** `send_prefixed_key()` in `keymap.c` clears the real, weak and
one-shot modifiers before tapping, and a Vial macro cannot do that. With Shift physically held,
`M0` will type **è** instead of **é**, because ´ and ` share a key on the German layout — exactly
the failure mode the comment in `keymap.c` warns about. Same for `M3`, `M4`, `M7` and `M8`, which
would land on E1's unassigned level 4 and produce nothing.

This only bites if you hold Shift while pressing a macro that is documented to make minuscules
only, so it is accepted rather than papered over, and written down in the
[README](README.md#what-differs-from-the-compiled-keymap). (`Up(KC_LSFT)`
at the top of each macro would clear the report bit, but it would not restore it afterwards, which
is worse.) Caps Word parks a weak Shift the same way, so the macros should not be used inside
Caps Word either — in the compiled firmware Caps Word stops on them, here it will not.

### 3.2 Caps Word needs a combo

`CAPS_WORD_ENABLE` is on in stock Vial, but **`BOTH_SHIFTS_TURNS_ON_CAPS_WORD` is not, and it is
not one of the runtime settings** — it is absent from `qmk_settings_t`. And `cozy_de` has no
`CW_TOGG` key anywhere in its four layers, so without the both-shifts trigger Caps Word would be
unreachable.

Fix: one Vial **combo**, which the `.vil` does carry.

```
KC_LSFT + KC_RSFT = CW_TOGG
```

(vitaly's own README shows this exact combo as an example, so it is a well-trodden substitute.)

Two differences from the compiled behaviour, worth a line in the README:

- QMK's both-shifts feature fires when both shifts are released together; a combo fires on the
  chord itself, and it *toggles*, so pressing both shifts again turns Caps Word off.
- Stock Vial compiles in `CAPS_WORD_INVERT_ON_SHIFT` (see the `build_vial.mk` excerpt above),
  which our build does not have. Inside Caps Word, Shift then produces a *lowercase* letter.

### 3.3 The version key

`MX_VERS` prints `VERSION_STRING` plus `__DATE__`. The build date is compile-time and simply
gone. A `Text(...)` macro is also the wrong tool: the firmware's `SEND_STRING` table is the US
one (`sendstring_german.h` is a compile-time include), so `-` and `,` would come out as ß and
friends on a German OS layout.

**Decided: dropped.** It is `KC_NO` on L_FN in the `.vil`. For the record, the alternative was:

1. **Drop it.** Its whole purpose was to tell which compiled firmware is on the board, and the
   point of this exercise is that there is no longer a per-keymap build to identify.
2. Keep a static `Tap(...)` sequence spelling out a version string in German-layout keycodes,
   and bump it by hand whenever the `.vil` changes.

### 3.4 RGB

`VIALRGB_ENABLE` means effect, speed, hue/sat/val live in EEPROM and are set over the protocol
(`vitaly rgb -e <n> -c '#rrggbb' -p`, where `-p` persists across restarts). The stock Vial
firmware offers the 44 standard RGB matrix effects, and any of them can be chosen at runtime.

**But `ENABLE_RGB_MATRIX_KEY_GROUPS` is not one of them.** That flag selects a colour mode that
exists only in your own QMK fork — which is why I could not find it in QMK master or in
`vial-qmk` — so *no* stock Vial build can offer it, at runtime or otherwise. This is the one
place where "stock firmware" and the current `cozy`/`cozy_de` behaviour genuinely cannot both
be had:

- **Accept the standard effects.** Solid Color is the closest match to what the key-groups mode
  was layered on top of, and it is one `vitaly rgb -e` away.
- **Port the fork's colour mode into `vial-qmk`.** Technically fine — `vial-qmk` is a QMK fork
  itself, so the animation drops in the same way — but it ends the "nothing to maintain" premise:
  every `vial-qmk` update would need the patch reapplied.

**Decided: the standard effects.** Nothing about RGB is pinned in `seed-cozy-de.sh` — it
persists on its own once set — but the script carries a commented `vitaly rgb` line showing how
to fix an effect and colour if you want them reproducible.

---

## 4. What is lost

| From `config.h` / `rules.mk` | Status on stock Vial |
| --- | --- |
| `BOTH_SHIFTS_TURNS_ON_CAPS_WORD` | replaced by a combo, §3.2 — behaviour differs slightly |
| mod-clearing in `send_prefixed_key()` | gone, §3.1 — accent macros misfire if Shift is held |
| `MX_VERS` version + build date | gone, §3.3 |
| `RGB_MATRIX_TIMEOUT`, `RGB_MATRIX_SLEEP`, the `#undef ENABLE_RGB_MATRIX_*` list | compile-time only; the stock Vial build has the full animation list and no idle timeout |
| `CONSOLE_ENABLE` / `keyboard_post_init_user()` | Vial compiles with `-DNO_DEBUG`; the keymap has no custom code anyway |
| `ENABLE_RGB_MATRIX_KEY_GROUPS` | **gone, and this one is a real loss** — it is a colour mode from your own QMK fork, so no stock Vial build can have it. See §3.4. |
| a 5th layer | no headroom, §1 |

---

## 5. Deliverables

Both are now in this folder, alongside a [`README.md`](README.md) covering day-to-day use:

- **[`cozy_de.vil`](cozy_de.vil)** — 4 layers × 10 rows × 6 cols of keycode strings, plus the
  nine macros, the thirteen key overrides and the Caps Word combo.
- **[`seed-cozy-de.sh`](seed-cozy-de.sh)** — unlock, load the `.vil`, then set what the `.vil`
  does not carry.

The `.vil` was generated from `cozy_de/keymap.c` by a throwaway script, not kept: the conversion
happens once, and an unmaintained generator in the repo would rot. What the generator checked is
worth recording, though, because it is what makes the file trustworthy without a keyboard to
test on:

- every one of the 240 cells and every macro, override and combo keycode was evaluated with a
  local reimplementation of Vial's own `AnyKeycode` parser, so each string is one Vial can read;
- the German aliases were resolved out of `keymap_german.h` rather than typed by hand, which is
  what keeps the Qwertz swap (`DE_Y` is `KC_Z`, `DE_Z` is `KC_Y`) honest;
- the LAYOUT-index → matrix map was taken from `keyboard.json` and cross-checked against the
  56 `row,col` labels in `vial.json`, which list the same cells in a different (visual) order;
- the `uid` is `VIAL_KEYBOARD_UID` read back as a little-endian `uint64`, matching the firmware.

`vitaly load` restores macros, key overrides, alt-repeat keys, combos, tap dances and keys —
but its output does not mention settings, so the script sets those explicitly. (Vial GUI's own
`restore_layout` *does* apply the `settings` block of a `.vil`; vitaly appears not to. The script
does not depend on which is true.)

Sketch:

```sh
#!/bin/sh
set -eu
ID=5462                       # Iris CE product id 0x1556 in decimal — confirm with `vitaly devices`

vitaly -i $ID lock -u         # hold the two unlock keys when prompted
vitaly -i $ID load -f cozy_de.vil
vitaly -i $ID settings -q 7  -v 500     # Tapping Term
vitaly -i $ID settings -q 22 -v true    # Permissive Hold
vitaly -i $ID rgb -e 2 -c '#20304a' -p  # Solid Color, persisted
vitaly -i $ID lock -l
```

The unlock keys are fixed in the firmware as matrix `(0,0)` and `(9,5)`
(`VIAL_UNLOCK_COMBO_ROWS {0,9}`, `VIAL_UNLOCK_COMBO_COLS {0,5}` in the keymap's `config.h`) —
that is the top-left Esc position and the right half's inner thumb key. `vitaly lock -u` shows
which to hold.

### Matrix map

Needed to generate the `.vil`, and worth keeping here: the Iris CE `LAYOUT` macro's argument
order maps to matrix positions as follows (rows 0–4 are the left half, 5–9 the right, read
left-to-right per physical row). From `keyboards/keebio/iris_ce/rev1/keyboard.json`.

| Physical row | Left half (cols 0→5) | Right half (cols 5→0) |
| --- | --- | --- |
| 1 (numbers) | `0,0` `0,1` `0,2` `0,3` `0,4` `0,5` | `5,5` `5,4` `5,3` `5,2` `5,1` `5,0` |
| 2 | `1,0` … `1,5` | `6,5` … `6,0` |
| 3 | `2,0` … `2,5` | `7,5` … `7,0` |
| 4 (+ inner) | `3,0` … `3,5`, then `4,5` | `9,5`, then `8,5` … `8,0` |
| thumbs | `4,2` `4,3` `4,4` | `9,4` `9,3` `9,2` |

The four unused cells — `4,0`, `4,1`, `9,0`, `9,1` — are written as `-1` in the `.vil`.

---

## 6. Order of work

1. Build and flash `keebio/iris_ce/rev1:vial` from `vial-qmk`, both halves. ([step 1](#step-1-get-the-firmware))
2. `vitaly devices` — confirm the product id and that the board answers.
3. ~~Generate `cozy_de.vil`~~ — **done**, see §5.
4. `vitaly load -f cozy_de.vil -p` to preview before writing anything.
5. ~~Write~~ **done**, and run `seed-cozy-de.sh`.
6. Test pass, against the README's own list: the Shift mapping of the number row, the ä/Tab
   chameleon under Ctrl/Alt/Gui, the five accent macros, ^ and `, the Level-5 items (¢ £ and
   `DE_LVL5` itself), Caps Word via the new combo, and the 500 ms tapping term on the four
   `LT()` keys.
7. Write up the differences in the repo README.

### Step 1: get the firmware

There is no prebuilt Vial binary for the Iris CE. Keebio's firmware repo ships Vial builds only
for the Iris LM (<https://github.com/keebio/keebio-firmware/tree/master/vial>), and Vial does not
host binaries itself. So:

```sh
git clone --recurse-submodules https://github.com/vial-kb/vial-qmk.git
cd vial-qmk
make keebio/iris_ce/rev1:vial
```

The *source* is stock — we add nothing to it — which is what matters for the "no firmware to
maintain" goal. But it does mean one toolchain run, and a repeat of it whenever vial-qmk moves
on. `vial-qmk` tracks QMK with a lag; its `quantum/keycodes.h` is from 2025 and already has the
`RM_*`, `MS_*` and `CW_TOGG` names, so it is current enough for everything above.

---

## 7. Decisions

1. **`MX_VERS`** — dropped. `KC_NO` in its place on L_FN. §3.3
2. **Generator script** — written but not kept. The conversion happens once; a generator nobody
   reruns would only rot. §5 records what it verified.
3. **`cozy_de` is the basis.** The `.vil` was generated from `cozy_de/keymap.c`, and from here
   on the `.vil` is the one that gets edited — in the Vial GUI, saved back over the file. The
   two will drift, and that is fine: the C keymap is the fallback for if Vial does not work out,
   not a source that keeps being re-converted. Drift is not the same as ignoring the C keymap,
   though: changes that land on `main` before this branch merges are still worth carrying over
   by hand, which is what §8 is.
4. **The key-groups colour mode** — let it go, and use the standard RGB effects. Carrying the
   fork's patch into `vial-qmk` was the alternative; it would have ended the "nothing to
   maintain" premise that makes this whole exercise worth it. §3.4

---

## 8. Re-sync with `main`

The `.vil` was generated from `cozy_de/keymap.c` as it stood at `2915db2`. `main` has moved
since, and this branch is rebased on it; two of those changes are ones the `.vil` has to carry.
Both were applied by hand, cell by cell; nothing was regenerated.

**The L_COMBINE cleanup** (`2fdc6ac`, `015a997`, `af12141`) rearranged the layer:

| | Before | Now |
| --- | --- | --- |
| ä ö ü | the A, O and U keys, as well as the base layer | gone — an unshifted base-layer key is not worth a second position |
| ß | the S key and the Z key | the S key only |
| § | the 5 key | the 3 key, where the standard German layout has it too (Shift+3) |
| the cedilla and the dead stroke | the 3 and 4 keys | the 4 and 5 keys, so the seven accents sit side by side on 4…0 |
| à | the Y key | the A key, freed by ä |

Eight cells of layer 1 changed; no other layer changed a single cell, so the rest of the file is
still in sync with `cozy_de/keymap.c`.

**The issue #14 fix** (`96709de`, `d5dfd09`) split four key overrides into per-Shift-side pairs,
taking the count from 9 to 13. The reasoning is in [§2.2](#22-key-overrides); the short version
is that `vial-qmk` has the pre-fix `process_key_override.c`, so stock Vial reproduces the bug
exactly as the compiled firmware did, and the fix is data-only so it transfers unchanged.

Not carried over, because they do not touch the `.vil`: the shared `users/cozy_common` build-date
header and the `MX_VERS` version bumps (`MX_VERS` is dropped here, §3.3), the host-side key-override
test in `tests/`, the CI action bumps, and the `cozy`-keymap and ReadMe edits.

---

## References

- Vial: <https://get.vial.today/> · manual: <https://get.vial.today/manual/>
- vial-qmk: <https://github.com/vial-kb/vial-qmk>
  - Iris CE Vial keymap: `keyboards/keebio/iris_ce/keymaps/vial/`
  - Vial build defaults: `builddefs/build_vial.mk`
  - Slot counts: `quantum/vial.h` · runtime settings: `quantum/qmk_settings.h`
  - Layer count default: `quantum/dynamic_keymap.h`
  - Key override semantics: `quantum/process_keycode/process_key_override.h`
- vial-gui (defines the `.vil` format): <https://github.com/vial-kb/vial-gui>
  - `src/main/python/protocol/keyboard_comm.py` — `save_layout()` / `restore_layout()`
  - `src/main/python/protocol/key_override.py` · `src/main/python/macro/macro_action.py`
  - `src/main/python/any_keycode.py` — which keycode expressions parse
- vitaly: <https://github.com/bskaplou/vitaly> · crate: <https://crates.io/crates/vitaly>
- QMK Key Overrides: <https://docs.qmk.fm/features/key_overrides>
- QMK Caps Word: <https://docs.qmk.fm/features/caps_word>
- QMK Tap-Hold: <https://docs.qmk.fm/tap_hold>
