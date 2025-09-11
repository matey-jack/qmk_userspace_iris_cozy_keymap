# QMK Userspace for The Cozy Keymap on the Iris CE split keyboard

![Iris CE with MBK Glow key caps reflecting this key map as closely as the caps allow](<Iris CE with Cozy Keyboard mapping 2025-09.jpg>)

[Google Sheet with the exact keymapping (at some point in time)](https://docs.google.com/spreadsheets/d/1JkIiKLAgzVKIijrSS0zbML-NLrd7E52zQ_xwXzjz3oQ/edit?usp=sharing)

Web app which shows the rationale behind the letter mapping: https://matey-jack.github.io/key-layout-visualizer/#layout=1&split=1&mapping=cozy-german (I won't go into that here. You can use Qwerty or Colemak or anything else with the rest of my layout. It will even simplify the mapping, because the apostrophe can move back to the right side, not having the finger conflict with T on pinky any more.)

## Some key points of the layout (other than the letter mapping)
 
Many keys are in the same (relative) position as on a traditional keyboard:
 - On the left edge we have Escape, Tab, Shift, Ctrl in the traditional order. 
   Since Alt is also in the traditional position Alt+Tab remains the same gesture.
 - Both Shift keys are on the home row to avoid diagonal moves during text-typing flow. 
 - On the right, we have Backspace still in the traditional position. 
   Right Ctrl and Fn swap places, since I don't have strong muscle memory for those keys and the new positions make for easier combos.

Layers:
 - There is an AltGr layer and a Fn layer which mimic a bit the AltGr layer from traditional keyboard layouts (only with more characters mapped on it) and the Fn layer from laptop and other small keyboards.
 - For reasons of my personal keyboard history (not having an Fn layer for many years, and the AltGr layer toggle in a much easier to reach spot), nav keys are on the AltGr layer.
 - Both layers have at least one layer activation key on each half of the keyboard. 
 - AltGr behaves much like the legacy AltGr key, but is in fact a pure firmware layer. Thus it can also modify the behavior of non-character keys. 
 - Each thumb has two keys that are most easy to reach. Those are mapped to Delete/AltGr and Space on the left and Enter/AltGr and E on the right. 
   I find that tap/hold in those combinations works on much better than with letters which are often rolled.
 - Fn has non-symmetric positions on both halves. On the left tap/hold with Escape is a great position to hit Function-keys. 
   On the right, bottom row tap/hold with the rarely used Insert key, it's great for volume control, which is placed on the < and > keys. 
 - AltGr also swaps the function of Space and Enter as well as Backward delete and Forward delete. 
   This let's me do more editing and confirming dialogs with the left hand while the right is on the mouse. 
   (You might not need this on a one-piece keyboard, but on a split, the other side of the board is further away!)

Modifiers:
 - Legacy modifier keys (Shift, Ctrl, Alt, Win) are not tap/hold to fully preserve their combined functions with mouse gestures (like Shift+Click for select, Ctrl+Scroll for Zoom, and similar).
 - Alt is the only modifier present only on one side, because it's relatively rarely used. It's on the left so that it can be used when the right hand is on the mouse.
 - I initially had only one Windows/Command key, but I really missed the single-tab expose view from Ubuntu, so I added a second Win key which on tap does not open the start menu, but the expose (or "task view", Win+Tab). 
 - The Windows keys are the hardest to reach from the home position, but this is fine, since they are not part of a typing flow; hands are more in a relaxed editing mode.
 - It's easy to press Ctrl+Shift (on the left side) with just one finger (pinky stretched out) which is used in some software combos. 
 - The same is true for AltGr+Win (aka Nav+Win) on the right hand which is good to press arrow keys on the left hand for some combos. 
   When moving the right hand a bit, it can also easily hold Ctrl+Win+Nav for some other arrow key combos on the other hand.

Firmware combining accents:
 - German has four more letters than English: äöü and ß. I tried many ways to include them on the board (tap/hold, AltGr layer, separate layer, and finally using the US ext. intl. combining diaeresis accent). 
 - I found the combining accent (aka dead key) was the best solution of those and realized that implementing it in Firmware would be even better: I can configure additional combinations without changing the software layout. I can also set it such that entering a non-existing combo will not create a stray accent (without letter) in the text.
 - Since the combining accent is implemented as a plain one-shot layer, I put a few more characters on that layer, especially various other accents, making sure that there is a memorable distinction between this layer and AltGr and that use-case for those additional characters fit the one-shot (prefix key) activation pattern

 I can't emphasize enough how useful the number row is on a keyboard: not only can I reuse muscle memory from traditional keyboards, 
 but I can also look at the keycaps which directly show base and shift layer characters and indirectly show the number of the function key
 and some hint at the AltGr key. (For example € on $, ‰ on %, and similar.) Each key carries up to five functions. 
 Having to spread those across many different layers all over the rest of the keyboard would be a lot of mappings to remember!



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
