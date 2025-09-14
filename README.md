# QMK Userspace for The Cozy Keymap on the Iris CE split keyboard

This keymap is a child of three different parents:
 - US ANSI
 - German Qwertz
 - The wide hand mapping for ANSI and ISO keyboards which originated first as a Colemak mod.

The keymap assumes that your computer is running the "US extended international" layout. It works on US ANSI as well, but then, of course, all the non-English letters and some other characters are missing! (Details below.)

The US / German mix is mostly reflected in the distribution of punctuation characters, 
while the wide hand mapping brings the concept of typing a letter (here E) with the right thumb. 
(Shift on the edge of the home row also indirectly comes from there.)

What makes this keymap special is that I have already typed with a "software split" (aka "wide") keymap on the traditional ANSI layout for many years. 
The finger movement for this physical split keyboard is now very similar to the wide ANSI keymap that can be used on any dumb keyboard, even laptops. 
(See [Github: gemütliche Tastatur](https://github.com/matey-jack/gemuetliche-tastatur), 
although that needs to be updated to sync better with the new physical split keyboard.)

TODO: Screeshot of the AltGr and Nav layer from the spreadsheet.

So while this Iris keymap is very compatible with the ANSI / ISO fingerings, those fingerings match the wide mod, which is not always the traditional one.


![Iris CE with MBK Glow key caps reflecting this key map as closely as the caps allow](<Iris CE with Cozy Keyboard mapping 2025-09.jpg>)

[Google Sheet with the exact keymapping (at some point in time)](https://docs.google.com/spreadsheets/d/1JkIiKLAgzVKIijrSS0zbML-NLrd7E52zQ_xwXzjz3oQ/edit?usp=sharing)

Web app which shows the rationale behind the letter mapping: https://matey-jack.github.io/key-layout-visualizer/#layout=1&split=1&mapping=cozy-german (I won't go into that here. You can use Qwerty or Colemak or anything else with the rest of my layout. It will even simplify the mapping, because the apostrophe can move back to the right side, not having the finger conflict with T on pinky anymore.)

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
 - In particular, the Shift mapping on numbers 1..8 follows US ANSI, which is also making nice use of the shine-through keycaps that are only available with those US labels. (Parentheses are all collected on the AltGr layer, leaving Shift 9 and 0 free.)
 - `?` is retained in its German position on Shift+0, which is a great analog to the (US and German) position of `!`.
 - TODO: `/` on Shift+9 as a neighbor of `*` (and similar to its position in the German standard). 
         This way, we'll not be confused by the `/?` keycap which doesn't actually type `?`.  
 - The bottom row triplet `,.-` is exactly as in the German standard, nicely carrying `;:_` on the Shift mapping, which is both logical and comfortable, and lets us dispose with the `;:` key of US ANSI. The Shift labels `,<` and `.>` are still correct, though, because `<` and `>` are mapped to these keys on the AltGr layer (see "stack of parentheses" below).
 - Big thanks goes to Pascal Getreuer for his "custom_shift_keys" module, which makes all this possible! 
   I think that this is an essential tool to help non-American users make good keymaps for their language!
 - The apostrophe/quote key is the standard US shift-pairing, but as a key moves to the left hand to avoid bigram conflicts with the letter T. I recommend swapping this back to the right edge for anyone using a different letter mapping (like Colemak, Qwerty, or other).
 - The `/` ends up on the top right, just because that spot is free. For anyone used to a US ANSI keyboard, I recommend simply swapping its position with the `-_` key.

QMK Layers:
 - There is an AltGr layer and a Fn layer which mimic a bit the AltGr layer from traditional keyboard layouts (only with more characters mapped on it) and the Fn layer from laptop and other small keyboards.
 - For reasons of my personal keyboard history (not having an Fn layer for many years, and the AltGr layer toggle in a much easier to reach spot), nav keys are on the AltGr layer.
 - Both layers have at least one layer activation key on each half of the keyboard. 
 - AltGr behaves much like the legacy AltGr key, but is in fact a pure firmware layer. Thus it can also modify the behavior of non-character keys. 
 - AltGr is activated symmetrically with the secondary thumb keys (Delete and Enter) as a tap/hold. 
   I find that tap/hold in those combinations is quite resilient against various typing styles. Tap/hold on letters that are typed fast and ofte overlap with other letters does not work for me.
 - The Fn activator has non-symmetric positions on both halves. On the left tap/hold with Escape is a great position to hit Function-keys. 
   On the right, bottom row tap/hold with the rarely used Insert key, it's great for volume control, which is placed on the < and > keys. 

One-shot layer / combining accents:
 - German has four more letters than English: äöü and ß. Placing them on the Iris keyboard was the hardest task of all. On the Ergodox, which has many more keys, it was easy to shift punctuation around to keep the German letters äöü in a similar position to the German ISO keyboard, but the Iris is just missing two keys. I have tried various positions for those letters: first on the AltGr layer, but that requires too much coordination of two key presses while typing; then on tap/hold keys (with Ctrl and Fn in the bottom corners and the third letter where the accent key currently sits), but that leads to typos and also those bottom corner diagonal positions are not ergonomic to reach.
 - My favorite solution turned out to use the "diaeresis combining accent" key from the US ext. int. layout. This allows typing two keys in sequence, which is exactly what our fingers do when tying words really fast. And it's resilient towards rolling (press/release overlaps), which makes it both fast and comfortable. From this experience came the idea to use the combining accent key for more than three letters. And thus I created a separate layer which simulates the diaeresis combining accent when used with a, o, and u, and then does a lot of other useful stuff, too.
 - In particular, all five combining accent tables from the US ext. intl. layout (Windows version) are accessible on this layer. 
   Although this means having to press three keys in sequence to produce a letter like ÿ, it's good to have those rarely needed things available at all ... and in a relatively memorable position. 
 - Since this concept is the newest in my keymap (2025-Sept), I expect a few more tweaks and changes to come in the next couple of weeks.
 - For one thing, I want to make good use of the one-shot activation of this layer (as opposed to the hold+press activation of the AltGr layer which mimics standard Shift and AltGr behavior) and use it for characters that fit well into a typing flow. It wouldn't make sense to add anything on this layer, which is often pressed with another key on the same layer. (For example, I can input `<=` by holding AltGr while pressing `<` and `=`, whereas the one-shot modifier would have to be pressed for each character again.)

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
 Having to spread those across many different layers all over the rest of the keyboard would be a lot of mappings to remember!

## Status of the design

When I started with split ergo keyboards, I designed my first key map before even ordering the keyboard... and during the first days of use made a ton of adjustments. 
(For example, I tried the popular "backdelete on thumb" and found it catastrophic to get used to at a time when I needed it more than ever, because of all the other adjustments.) 
I am publishing this keymap now, because things have stabilized. 
However, since the accent layer is brand new, there still might be changes there, and via trickle-down effects also to the AltGr layer.
(The latter used to be quite cramped, which led to some keys not being in a good position neither for pressing nor for remembering them.
By moving some characters to the accent layer, the AltGr layer can become nicer.)

Details of how well everything is production tested:
* The position of basic keys (like Escape, Backspace, Tab, Shift, Enter, Ctrl): I defined those during the early days of my Ergodox use about 11 months ago. 
  All of that worked great both on the Ergodox and the Iris, so I am unlike to modify any of that. 
  - Exception: I initially thought that Space and Enter should be on symmetric (mirrored) positions, but later decided that the two most-used thumb keys (Space and E) should be in mirrored positions. I did this swap only recently and still get used to it. It also has the advantage that AltGr is now on mirrored thumb keys on each side. (Remember that AltGr is on the less-used keys to avoid mistypings due to the tap/hold logic.)
* The Shift pairings: basically unchanged, since my first Iris keymap, and production-proven. (Even the Ergodox version before that was only slightly different, because of having two more keys in the number row to work with.)
    - But there is potential to use the US keycap `+=` instead of `/?` which would reflect both layers correctly. 
      Then `/` could be mapped to Shift+9 where it would directly neighbor `*`. A very aesthetic match, right? :-D 
* The Nav layer: has been the same for more than ten years. 
  I actually got a programmable keyboard partly because the software implementation of the nav layer was unreliable.
  Now with Firmware it's better indeed... but I miss having actual additional arrow keys. :-/
* AltGr characters: the important ones are stable for more than ten years, while some unimportant characters keep shifting around when something else changes. 
* The accent layer: Umlauts äöü and ß are unlikely to change, while everything else still needs production usage to evaluate its effectiveness.
* The Letter mapping (just FYI, since probably few people come for that): I've been using this for more than ten years, and after recently trying to improve it without finding any worthwhile improvements, I will probably use it until the end of my life.

# Building this repository

`qmk compile-userspace` doesn't show any error messages. My workaround is to run `qmk compile -km cozy` instead.

The Github Actions workflow should run the compile on every push. 

To get an easily downloadable firmware, push a new tag to the repository.
The workflow will create a release from that tag and attach the firmware to it.
You should then go to that release and add some release notes, since none are added automatically.


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
