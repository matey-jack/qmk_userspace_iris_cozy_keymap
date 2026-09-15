# xkb reference files

These are unmodified copies of the X keyboard layout definitions that the `cozy_de` keymap targets.
They are kept here only as a *reference*, so that every `#define DE_xxx ALGR(...)` line in
`keyboards/keebio/iris_ce/keymaps/cozy_de/keymap.c` can be checked against its source
without having a Linux machine at hand. Nothing in this directory is compiled.

| file | content |
| --- | --- |
| `symbols/de` | The German layouts. `cozy_de` targets the default one, `de(basic)`. |
| `symbols/latin` | `de(basic)` starts with `include "latin(type4)"`, which in turn includes `latin(basic)`. Most of the AltGr (level 3 and 4) characters that `cozy_de` uses actually come from here. |

How to read the tables: each key line is

    key <XXnn>  {[ level1, level2, level3, level4 ]};

with level 1 = plain, level 2 = Shift, level 3 = AltGr, level 4 = AltGr+Shift.
The `<XXnn>` names are physical key positions: `AEnn` is the number row, `ADnn` the top letter row,
`ACnn` the home row and `ABnn` the bottom letter row, each counted from the left.

Source: <https://gitlab.freedesktop.org/xkeyboard-config/xkeyboard-config>, files `symbols/de`
and `symbols/latin`. Copied from the `xkb-data` 2.41 package (`/usr/share/X11/xkb/symbols/`).
xkeyboard-config is distributed under the MIT/X11 licence; see the project's `COPYING` file.
