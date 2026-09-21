# `cozy_de` key override tests

A regression test for [issue #14](https://github.com/matey-jack/qmk_userspace_iris_cozy_keymap/issues/14):
a key override whose replacement is itself a shifted keycode must keep that Shift on the side the
user really pressed. `ko_make_basic(MOD_MASK_SHIFT, ...)` does not — it suppresses whichever Shift
was held and re-adds a *left* one, so pressing the key with the **right** Shift swaps the modifier
byte from RSFT to LSFT in the same breath as the replacement key goes down. A host that resolves
the keycode before the modifier delta then types the unshifted character: `2` for `"`, `6` for `&`,
`+` for `*`, `ß` for `?`.

Nothing about that is visible in the keymap source, and it only shows up with one of the two Shift
keys, which is why it went unnoticed for so long on the number row. Hence a test.

It drives QMK's real key override machinery on the host — no hardware, no ARM toolchain — and
asserts, for each of the four affected pairings and for **both** Shift keys, that

- a report carrying the replacement key together with that same Shift is sent, and
- a report carrying the replacement key *unshifted* is never sent.

## What is in here

| file | |
|---|---|
| `test_cozy_de_key_overrides.cpp` | the test cases |
| `keymap.c` | the test keyboard's empty layers, plus an `#include` of the real overrides |
| `config.h`, `test.mk` | the harness settings `cozy_de` builds with |

`keymap.c` includes `key_overrides.h`, which is **not** checked in here: it is copied in from
`keyboards/keebio/iris_ce/keymaps/cozy_de/` just before the build. So the test compiles the same
definitions the firmware does, and there is no mirror to fall out of date. That is the whole reason
the overrides live in a header of their own rather than inside `keymap.c` — the test keyboard's
matrix and the Iris `LAYOUT` macro are incompatible, so `keymap.c` itself cannot be included here.

## Running it locally

```sh
git clone --depth 1 --branch master https://github.com/qmk/qmk_firmware.git qmk
git -C qmk submodule update --init --depth 1 lib/googletest lib/printf

python3 -m venv qmkvenv                                   # `pip install qmk` against the system
./qmkvenv/bin/pip install --upgrade pip setuptools wheel   # Python fails to build one of milc's
./qmkvenv/bin/pip install qmk                              # dependencies

cp -r tests/cozy_de_key_overrides qmk/tests/
cp keyboards/keebio/iris_ce/keymaps/cozy_de/key_overrides.h qmk/tests/cozy_de_key_overrides/

PATH="$PWD/qmkvenv/bin:$PATH" make -C qmk SKIP_GIT=yes test:cozy_de_key_overrides
```

`SKIP_GIT` is there because the submodule check wants submodules the shallow clone deliberately
lacks.

In CI this is the `key-override-tests` job of `build-on-push.yaml`, which runs the same commands on
every push and pull request, and which `release` depends on — a regression here cannot be tagged.

## Checking that it still bites

Reverting the four `ko_shifted_pair()` overrides in `key_overrides.h` to
`ko_make_basic(MOD_MASK_SHIFT, ...)` makes exactly the four `RightShift` cases fail and leaves all
four `LeftShift` cases passing. That is what makes this a regression test rather than a smoke test,
and it is worth re-checking if the assertions are ever reworked.
