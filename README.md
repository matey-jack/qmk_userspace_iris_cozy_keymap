# QMK Userspace for The Cozy Keymap on the Iris CE split keyboard

This keymap is a child of three different parents:
 - US ANSI
 - German Qwertz
 - My personal ergonomic letter map implemented as a national "keyboard layout" for various operating systems: https://github.com/matey-jack/gemuetliche-tastatur – This itself is based on a "wide hands" mod for ANSI keyboards

The keymap assumes that your computer is running the "US extended international" layout. 
It works on US ANSI as well, but then, of course, all the non-English letters and some other characters are missing! 
There is a switch in the keymap to allow apostrophe and quote in US ext int'l to be typed the same as ANSI. (Details below.)

The US / German mix is mostly reflected in the distribution of punctuation characters, 
while my "gemuetliche Tastatur" brings the concept of typing a letter (here E) with the right thumb. 
(Shift on the edge of the home row also indirectly comes from there.)

What makes this keymap special is that I have already typed with a "software split" (aka "wide") keymap on the traditional ANSI layout for many years. 
The finger movement for this physical split keyboard is now very similar to the wide ANSI keymap that can be used on any dumb keyboard, even laptops. 

TODO: Screeshot of the AltGr and Nav layer from the spreadsheet.

So while this Iris keymap is very compatible with the ANSI / ISO fingerings, those fingerings match the wide mod, which is not always the traditional one.


![Iris CE with MBK Glow key caps reflecting this key map as closely as the caps allow](<Iris CE with Cozy Keyboard mapping 2025-09.jpg>) TODO: update for `+=` change to base layer!

[Google Sheet with the exact keymapping (at some point in time)](https://docs.google.com/spreadsheets/d/1JkIiKLAgzVKIijrSS0zbML-NLrd7E52zQ_xwXzjz3oQ/edit?usp=sharing)

Web app which shows the rationale behind the letter mapping: https://matey-jack.github.io/key-layout-visualizer/#layout=1&split=1&mapping=cozy-german (I won't go into that here. You can use Qwerty or Colemak or anything else with the rest of my layout. It will even simplify the mapping, because the apostrophe can move back to the right side, not having the finger conflict with T on pinky anymore.)

## Note to self: building on Boxi

In WSL:

       cd qmk_firmware/
       .venv/bin/qmk compile

The userspace is already linked (I forgot how) and keyboard and keymap are configured.       

## Some key points of the layout (other than the letter mapping)
 
Many keys are in the same (relative) position as on a traditional keyboard:
 - On the left edge we have Escape, Tab, Shift, Ctrl in the traditional order. 
   Since Alt is also in the traditional position, Alt+Tab remains the same gesture.
 - Both Shift keys are on the home row to avoid diagonal moves during text-typing flow. 
   (This might be the biggest change compared to the ANSI layout, but on my ANSI wide mapping I also have it set up like this.)
 - On the right, we have Backspace still in the traditional position. 
   Right Ctrl and Fn swap places, since I don't have strong muscle memory for those keys and the new positions make for easier combos.
 - The remaining modifiers (Alt, Windows/Command, right Ctrl) are on thumb keys.

Thumb keys:
 - Space is obviously on the thumb, but also E – which is inherited from the ANSI wide keymap which makes the key just right of space into an actual home key for the right thumb. When typing prose (normal English or German texts), the thumbs will not move sideways but only press down on the keys they are on. (That's why Shift is not on a thumb key! It would lead to same-finger bigrams.)
 - Out of Iris' four thumb keys on each side, only one more is easy to reach. I put (forward) Delete on one of them and Enter on the other. This is the biggest change from the ANSI wide actually, and I find it both very practical and easy to get used to. 

Punctuation and Shift mappings:
 - The ANSI keyboard has 11 punctuation (and accent) keys and 10 more punctuation characters on the Shift+number keys.
 - This keymap has 6 punctuation keys, of which one is actually a one-shot layer activator which behaves like a combining accent key. (For that latter, see below.)
 - I put a lot of thought in choosing the 20 punctuation characters to map to the base and shift layers of those keys (10 on Shift+number, 2×5 on the pure punctuation keys). This allows writing normal text as easily as possible and still finding most of the punctuation in a familiar place (either from US ANSI, German Qwertz, or Cozy special.)
 - In particular, the Shift mapping on numbers 1..5 and 7..8 follows US ANSI, which is also making nice use of the shine-through keycaps that are only available with those US labels. (Parentheses are all collected on the AltGr layer, leaving Shift 9 and 0 free.)
 - `?` is retained in its German position on Shift+0, which is a great analog to the (US and German) position of `!`.
 - `+` is on Shift+9 as a neighbor of `*`. 
 - The bottom row triplet `,.-` is exactly as in the German standard, nicely carrying `;:_` on the Shift mapping, which is both logical and comfortable, and lets us dispose with the `;:` key of US ANSI. The Shift labels `,<` and `.>` are still correct, though, because `<` and `>` are mapped to these keys on the AltGr layer (see "stack of parentheses" below).
 - `=` is mapped on the same layer and next to `<` and `>` for easy typing of `<=`, `>=` and fat arrows.
 - Big thanks goes to Pascal Getreuer for his "custom_shift_keys" module, which makes all this possible! 
   I think that this is an essential tool to help non-American users make good keymaps for their language!
 - The apostrophe/quote key is the standard US shift-pairing, but as a key moves to the left hand to avoid bigram conflicts with the letter T. I recommend swapping this back to the right edge for anyone using a different letter mapping (like Colemak, Qwerty, or other).

QMK Layers:
 - There is an AltGr layer and a Fn layer which mimic a bit the AltGr layer from traditional keyboard layouts (only with more characters mapped on it) and the Fn layer from laptop and other small keyboards.
 - For reasons of my personal keyboard history (not having an Fn layer for many years, and the AltGr layer toggle in a much easier to reach spot), nav keys are on the AltGr layer.
 - Both layers have at least one layer activation key on each half of the keyboard. 
 - AltGr behaves much like the legacy AltGr key, but is in fact a pure firmware layer. Thus it can also modify the behavior of non-character keys. 
 - AltGr is activated symmetrically with the secondary thumb keys (Delete and Enter) as a tap/hold. 
   I find that tap/hold in those combinations is quite resilient against various typing styles. Tap/hold on letters that are typed fast and ofte overlap with other letters does not work for me.
 - The Fn activator has non-symmetric positions on both halves. On the left tap/hold with Escape is a great position to hit Function-keys. 
   On the right, bottom row tap/hold with the rarely used Insert key, it's great for volume control, which is placed on the < and > keys. 

One chameleon key to keep German Umlauts in acceptable positions:
 - ü and ö keep their positions from the 102 keyboard (ü as in Standard German, ö swaps with T in Cozy).
 - ä needs special treatment because it's key is lost on the smaller board (technically the space is taken over by Shift and ä could move down to the corner, but that's a bad place for a letter and also used as a tap/hold layer key which doesn't work well with letters.)
 - Therefore, we map ä to the same key as Tab using the Chameleon technique: using the key without modifier or with Shift, yields ä and Ä respectively. Using the key with Alt or Ctrl yields Alt+Tab and Ctrl+Tab. In this way, a lot of the use-cases are covered within existing muscle memory. Only to tap a plain Tab, we'll have to use L3+Tab. This latter did indeed take some time to get used to, but I find it the best compromise compared to more than three other ways in which I tried to map the German Umlauts. (First I tried them on the AltGr layer, but that requires too much coordination of two key presses while typing; then on tap/hold keys (with Ctrl and Fn in the bottom corners and the third letter where the accent key currently sits), but that leads to typos and also those bottom corner diagonal positions are not ergonomic to reach. And finally, I very long trial period of mapping the keys to a special combining accent layer. That worked better than the previous ways, but that extra combining key creates new bigram conflicts which became annoying once I started typing faster again.)
 - In short, this Chameleon solution has turned out to be the best of all workarounds for having only 56 keys on the keyboard. And it's this and one more missing punctuation key why I think a keyboard with 58 or 60 keys would be better for me. But for now, I use this. 

One-shot layer / combining accents:
 - The idea for this came originally from the combining-accent solution to the Umlaut problem. While I didn't like it for Umlauts, I found it very practical to have an additional layer for rarely (but still!) used characters. The problem with extra layers is always that the layer toggle itself takes away valuable mapping space, but since this layer is only rarely used, we can create a space-saving toggle: a one-shot layer triggered by a mapping that itself is on a layer!
 - In particular, this layer has all five combining accent tables from the US ext. intl. layout (Windows version), so that any character-diacritics combinations can be made. 
   Although this means having to press three keys in sequence to produce a letter like ÿ, it's good to have those rarely needed things available at all ... and in a relatively memorable position. 
 - To make typing French and Spanish easy, this layer also contains some of the most popular accent-letter pairs directly mapped to the key of the letter; and some others next to that letter. This means that the more laborious way of using the combining accent keys won't be needed in most cases.
 - Note that it wouldn't make sense to add anything on this layer, which is often pressed with another key on the same layer. For example, I can input `<=` by holding AltGr while pressing `<` and `=`, whereas the one-shot modifier would have to be pressed for each character again. But those cases are already covered on the Base, Shift, and AltGr layers!

"Stack of parentheses" and other goodies on the AltGr layer:
 - This one has a long tradition on my dumb keyboard software layouts. It uses the same fingers for all four kinds of parentheses. 
   And those are also the longest fingers and the same fingers that German standard uses for `()` and `{}` and US ANSI uses for `<>`. 
   This is just awesome, and in all the rearrangements that I made for the AltGr layer over time, the stack of parentheses has always stood firm.
 - Other characters mostly sit in mnemonic positions like `|` on the `&` key (both play symmetric roles in programming languages) and `€` on `$`, `£` on `#` (which is a pun!) and `¢` right nearby.
 - The AltGr layer also has some alternate positions for punctuation keys to make certain bigrams typeable without having to switch between AltGr and Shift. 
   Those are `);` placed next to each other, for the convenience of C-like programming languages; and `=` placed next to `<` and `>`.
 - AltGr also swaps the function of Space and Enter as well as Backward delete and Forward delete.
   This lets me do more editing and confirming dialogs with the left hand while the right is on the mouse.
   (You might not need this on a one-piece keyboard, but on a split, the other side of the board is further away!)
 - Since I use the AltGr layer a lot, I have secondary activation keys for it that are not on the thumbs. 
   It's great for one-handed usage, especially with nav keys that are used when not in typing flow.

Fn layer:
 - F-keys on the number keys, mapped around, so that F11 (full-screen in many apps) and F12 lie close to Escape which is the Fn-activator.
 - Volume control on the < and > keys.
 - Everything else isn't thought through yet. I rarly use the other stuff there and will optimize it later.

Modifiers:
 - Legacy modifier keys (Shift, Ctrl, Alt, Win) are not tap/hold to fully preserve their combined functions with mouse gestures (like Shift+Click for select, Ctrl+Scroll for Zoom, and similar).
 - Alt is the only modifier present only on one side because it's relatively rarely used. It's on the left so that it can be used when the right hand is on the mouse.
 - I initially had only one Windows/Command key, but I really missed the single-tab expose view from Ubuntu. 
   To recreate that, I added a second Win key which on tap does not open the start menu, but the expose (or "task view", Win+Tab). 
 - The Windows keys are the hardest to reach from the home position, but this is fine, since they are not part of a typing flow; hands are more in a relaxed editing mode.
 - It's easy to press Ctrl+Shift (on the left side) with just one finger (pinky stretched out) which is used in some software combos. 
 - The same is true for AltGr+Win (aka Nav+Win) on the right hand, which is good to press arrow keys on the left hand for some combos. 
   When moving the right hand a bit, it can also easily hold Ctrl+Win+Nav for some other arrow key combos on the other hand.

 I can't emphasize enough how useful the number row is on a keyboard: 
 not only can I reuse muscle memory from traditional keyboards, 
 but I can also look at the keycaps which directly show base and shift layer characters and indirectly show the number of the function key
 and some hint at the AltGr key. 
 (For example, € on $, ‰ on %, and similar.) 
 Each key carries up to five functions. 
 
This is why I wouldn't want one of those "40%" or minimal keyboards which drop the number row and spread its content onto many more layers all over the rest of the keyboard.

## Status of the design

As of 2026-08, I have been using the entire keymap with only minimal updates for six months every day. 
And except for the Umlaut definitions, most things have been stable for about 18 months now.

### Status from when I first wrote this ReadMe in 2026-02

When I started with split ergo keyboards, I designed my first key map before even ordering the keyboard... and during the first days of use made a ton of adjustments. 
(For example, I tried the popular "backdelete on thumb" and found it catastrophic to get used to at a time when I needed it more than ever, because of all the other adjustments.) 

Details of how well everything is production tested:
* The position of basic keys (like Escape, Backspace, Tab, Shift, Enter, Ctrl): I defined those during the early days of my Ergodox use about 11 months ago. 
  All of that worked great both on the Ergodox and the Iris, so I am unlike to modify any of that. 
  - Exception: I initially thought that Space and Enter should be on symmetric (mirrored) positions, but later decided that the two most-used thumb keys (Space and E) should be in mirrored positions. I did this swap only recently and I'm still getting used to it. It also has the advantage that AltGr is now on mirrored thumb keys on each side. (Remember that AltGr is on the less-used keys to avoid mistypings due to the tap/hold logic.)
* The Shift pairings: basically unchanged, since my first Iris keymap, and production-proven. (Even the Ergodox version before that was only slightly different, because of having two more keys in the number row to work with.)
    - I only recently removed the `/?` US keycap in favor of `+=`. After a few weeks, I can say that I like it!
* The Nav layer: has been the same for more than ten years. 
  I actually got a programmable keyboard partly because the software implementation of the nav layer was unreliable.
  Now with Firmware it's better indeed... but I miss having actual additional arrow keys. :-/
* AltGr characters: the important ones are stable for more than ten years, while some unimportant characters keep shifting around when something else changes. 
* The accent layer: still needs production usage to evaluate its effectiveness.
* The Letter mapping (just FYI, since probably few people come for that): I've been using this for more than ten years, and after recently trying to improve it without finding any worthwhile improvements, I will probably use it until the end of my life.

# The `cozy_de` variant: standard German OS layout, no custom code

`keyboards/keebio/iris_ce/keymaps/cozy_de` is a second keymap with the same physical layout,
the same four layers and the same QMK keycodes as `cozy`, but with two differences:

 - It assumes that the computer runs the **German extended keyboard layout E1**
   (xkb's `de(e1)`, standardized as DIN 2137-1:2020-11) instead of US extended international.
   E1 is identical to the standard German layout on the plain and Shift levels, so only the
   AltGr characters below actually depend on it.
 - It contains **no community modules and almost no custom code**.
   The `getreuer/custom_shift_keys` module is replaced by [QMK's built-in Key Overrides](https://docs.qmk.fm/features/key_overrides),
   and `process_record_user()` only prints the firmware version and replays the two-keystroke
   sequences of the `custom_keycodes` enum in `cozy_de/keymap.c`.

Because the German layout already has ä, ö, ü and ß as normal keys, one workaround of `cozy`
is not needed here: the ANSI/Windows "quote mode" switch is gone, since the OS layout now decides
what the accent keys do. In exchange, some characters of `cozy` have no equivalent on levels 1 to 3
of `de(e1)` and are mapped to `KC_NO`; the head of `cozy_de/keymap.c` lists all of them with the reason.

## Accented letters

E1 is what makes this keymap's combining-accent layer complete. On top of the acute, grave and
circumflex that the standard German layout already has, it adds a **dead tilde**, a **dead
cedilla** and a **dead stroke**, and moves the **dead diaeresis**. All seven sit on row 1 of the
`L_COMBINE` layer, so any accent-letter pair can be composed by typing the accent and then the
letter. The `#define` block at the head of `cozy_de/keymap.c` names each one's xkb key and level,
so they can be checked against `reference/xkb/symbols/de` without a Linux box at hand.

The five most frequent pairs also have a **one-key macro** on that layer: é, è, à, ñ and ç.
These are minuscules only — the capitals É È À Ñ Ç are rare enough to be worth the two keystrokes,
and are typed with the explicit dead accent followed by a shifted letter. The macros clear all
modifiers before sending their dead key, because in the German layout a held Shift would turn
´ into ` (silently making è out of é), and would push the AltGr dead keys onto E1's unassigned
AltGr+Shift level, where they produce nothing at all.

A handful of `cozy`'s characters are not single keys on E1: it keeps them on level 5, behind an
`ISO_Level5_Latch`. That latch is on the `L_ALTGR` layer as `DE_LVL5`, in the position that
mirrors the `L_COMB` one-shot layer key on the other half of the board — the two are the same
idea, one implemented in the firmware and one in the xkb config. Like a dead key it applies to
the next keystroke. This is the only part of the keymap expected to be **Linux-only**, since the
Windows E1 implementation is not known to have a Level-5 latch. Which characters these are, and
what to type for each, is in the comment block at the head of `cozy_de/keymap.c`.

The two currency signs ¢ and £ get their own macros instead, in the `L_ALTGR` positions `cozy`
uses. £ needs one: its second keystroke is the ´ key, which is not on the base layer, so typing
it by hand would mean changing layers half way through the sequence.

Two more macros, `MX_HAT` and `MX_BTIC`, produce the **live** (non-combining) `^` and `` ` ``.
The German layout has no key for either — both are dead keys — so the macros add the trailing
space that turns a dead key into its spacing character.

The key overrides restore the Cozy Shift mapping on top of the German layout.
The German layout natively agrees with Cozy on `Shift 1 → !`, `Shift 4 → $`, `Shift 5 → %`,
`Shift , → ;`, `Shift . → :` and `Shift - → _`; the other eight pairings
(`@ # ß & * + ?` on the number row and `"` on the apostrophe key) are made by a key override.

Four of those eight — `&`, `*`, `?` and `"` — take two overrides rather than one, one per Shift
side, because their replacement is itself a shifted keycode. That was issue #14, and the comment
in `cozy_de/key_overrides.h` walks through the modifier-byte race it caused. The host-side test
in `tests/cozy_de_key_overrides/` compiles that same header, so the two cannot drift apart.

One more key override keeps the "chameleon" ä key working: plain and with Shift it types ä and Ä,
while holding Ctrl, Alt or Win turns it into Tab, so `Ctrl+Tab`, `Alt+Tab` and `Win+Tab` stay the
gestures they always were. Unlike the Shift overrides, this one suppresses no modifier, which is
what lets you hold Alt (or Win) and tap ä several times to walk through the window list.

The right thumb key is a plain `Win+Tab` (expose / task view) rather than a Mod-Tap;
the Gui modifier itself sits on the left thumb key, unchanged from `cozy`.

A copy of the xkb sources of the assumed OS layout is in `reference/xkb/`,
so that the AltGr mappings used in the keymap can be verified; see the README there.

# Building this repository

`qmk compile-userspace` doesn't show any error messages when it fails. 
My workaround is to run `./qmk compile -km cozy` instead. 

This means that I don't need `qmk userspace-add` to set this up, 
but I do need `qmk config user.overlay_dir=(where the user space is checked out)`.

The Github Actions workflow compiles both keymaps on every push, to any branch.
`cozy` and `cozy_de` are versioned, tagged and released the same way, but on separate ladders.

## No merge commits

**This repository takes no merge commits.** The history is linear and stays that way.

 - Bring a branch up to date by rebasing it on `main`, never by merging `main` into it.
 - Land a pull request by rebasing or squashing it, so what arrives on `main` is an ordinary
   commit — or a row of them — and not a merge.

That keeps every tag below pointing at a commit that carries exactly one version of each
keymap, which is what makes "which commit is this firmware" a question with one answer.

## Versions and tags

The workflow reads each keymap's version out of the `VERSION_STRING` in its own `keymap.c`.
The version is the whole `rev...` token, suffix and all — `rev22.2-winkey-fix`, not `rev22.2` —
so a tag says what changed, the way the hand-made tags used to. Two rules for writing one:

 - Put it at the end of the string. Extraction stops at the first character outside
   `[0-9A-Za-z._-]`, so `"Layout ASDR_NILT standalone, rev22.2-winkey-fix"` yields the whole
   suffix, while a trailing `", needs testing"` would be silently cut off.
 - Anything that would make an illegal Git ref (a trailing `.`, a doubled `..`, a `.lock`
   ending) is trimmed away rather than failing the build, so you cannot break the tag step by
   mistyping a version.

The tag is the keymap name plus that version: `cozy-rev22.2-winkey-fix`, `cozy_de-rev01`.
Tags made before this scheme are unprefixed and their suffix was written by hand, so it does
not always match the `VERSION_STRING` of the commit they point at.

## What each push does

 - Both firmwares are uploaded as build artifacts, `iris-cozy-${version}.uf2` and
   `iris-cozy-de-${version}.uf2`, so every branch build is downloadable without any tagging.
 - A keymap that **changed** and whose version is still the released one is not an error: it
   is simply not released. Nothing fails over it, and the build says so in the run summary.
   A comment, a refactor or a doc fix nobody has to reflash for therefore costs no bump —
   a release is something you ask for, by bumping `VERSION_STRING`.
 - A keymap you did not touch never has to be bumped, so changing one keymap does not drag
   the other's version along, and a branch that changes nothing under `keyboards/` needs no
   version work at all.
 - On a pull request the workflow comments the tags that merging will create, or says that it
   will create none and how to change that. It edits that one comment on every push, so a
   mistyped suffix or a forgotten bump is visible and fixable long before the merge, and the
   comment never blocks the merge.

## What a push to `main` does

For every keymap that changed **and** carries a version that was never tagged, a tag is created
on the commit that landed — so a push that bumped both gets two tags. There is one Release, on the
first of those tags (`cozy` before `cozy_de`), naming both firmware versions in its body.

Both `.uf2` files are attached whether or not both were tagged, so a release is always a
complete set of firmware. That means one of the two files can carry a version some earlier
release already carried; the release date and tag tell the two apart, and the release body
says which of the two rode along rather than being released in its own right.

A push to `main` creates no tag and no release when it touched nothing under `keyboards/`, and
likewise when every keymap it did touch still carries an already-released version. The firmware
is built either way and downloadable from the workflow run, so the only thing a missing bump
costs you is the Release entry.

You should still go to the release and add some notes, since none are added automatically.


# Original QMK Readme follows

## Howto configure your build targets

1. Run the normal `qmk setup` procedure if you haven't already done so -- see [QMK Docs](https://docs.qmk.fm/#/newbs) for details.
1. Fork this repository
1. Clone your fork to your local machine
1. Enable userspace in QMK config using `qmk config user.overlay_dir="$(realpath qmk_userspace)"`
1. Add a new keymap for your board using `qmk new-keymap`
    * This will create a new keymap in the `keyboards` directory, in the same location that would normally be used in the main QMK repository. For example, if you wanted to add a keymap for the Planck, it will be created in `keyboards/planck/keymaps/<your keymap name>`
    * You can also create a new keymap using `qmk new-keymap -kb <your_keyboard> -km <your_keymap>`
    * Alternatively, add your keymap manually by placing it in the location specified above.
    * `layouts/<layout name>/<your keymap name>/keymap.*` is also supported if you prefer the layout system
1. Add your keymap(s) to the build by running `qmk userspace-add -kb <your_keyboard> -km <your_keymap>`
    * This will automatically update your `qmk.json` file
    * Corresponding `qmk userspace-remove -kb <your_keyboard> -km <your_keymap>` will delete it
    * Listing the build targets can be done with `qmk userspace-list`
1. Commit your changes

## Howto build with GitHub

1. In the GitHub Actions tab, enable workflows
1. Push your changes above to your forked GitHub repository
1. Look at the GitHub Actions for a new actions run
1. Wait for the actions run to complete
1. Inspect the Releases tab on your repository for the latest firmware build

## Howto build locally

1. Run the normal `qmk setup` procedure if you haven't already done so -- see [QMK Docs](https://docs.qmk.fm/#/newbs) for details.
1. Fork this repository
1. Clone your fork to your local machine
1. `cd` into this repository's clone directory
1. Set global userspace path: `qmk config user.overlay_dir="$(realpath .)"` -- you MUST be located in the cloned userspace location for this to work correctly
    * This will be automatically detected if you've `cd`ed into your userspace repository, but the above makes your userspace available regardless of your shell location.
1. Compile normally: `qmk compile -kb your_keyboard -km your_keymap` or `make your_keyboard:your_keymap`

Alternatively, if you configured your build targets above, you can use `qmk userspace-compile` to build all of your userspace targets at once.

## Extra info

If you wish to point GitHub actions to a different repository, a different branch, or even a different keymap name, you can modify `.github/workflows/build_binaries.yml` to suit your needs.

To override the `build` job, you can change the following parameters to use a different QMK repository or branch:
```
    with:
      qmk_repo: qmk/qmk_firmware
      qmk_ref: master
```

If you wish to manually manage `qmk_firmware` using git within the userspace repository, you can add `qmk_firmware` as a submodule in the userspace directory instead. GitHub Actions will automatically use the submodule at the pinned revision if it exists, otherwise it will use the default latest revision of `qmk_firmware` from the main repository.

This can also be used to control which fork is used, though only upstream `qmk_firmware` will have support for external userspace until other manufacturers update their forks.

1. (First time only) `git submodule add https://github.com/qmk/qmk_firmware.git`
1. (To update) `git submodule update --init --recursive`
1. Commit your changes to your userspace repository
