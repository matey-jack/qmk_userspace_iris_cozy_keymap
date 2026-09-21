// Copyright 2025 Robert Jack Will
// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

/*
    The key overrides of the `cozy_de` keymap, in their own file so that the host-side test in
    `tests/cozy_de_key_overrides/` can compile the very same definitions. The test keyboard's
    matrix and the Iris `LAYOUT` macro are incompatible, so `keymap.c` itself cannot be included
    there; this header can, and that keeps the test and the firmware from drifting apart.
*/

#include "keymap_german.h"

/*
    Key overrides: the Cozy Shift mapping, and the ä / Tab chameleon key below it.
    Together they replace everything that `cozy` needed a community module or custom code for
    (apart from printing the version).

    Three of the five custom shift keys of the `cozy` keymap are recreated here (Shift 6, 9 and 0);
    the other two (Shift `,` and Shift `.`) are already what the German layout does anyway.
    On top of that come five pairings that the US ANSI layout provided for free but the German one
    doesn't: Shift 2, 3, 7, 8 and the double quote.
    QMK's Key Overrides are pure data (no custom code), see https://docs.qmk.fm/features/key_overrides

    `ko_make_basic()` suppresses the trigger modifiers, so the replacement keycode fully determines
    what the host sees.

    That last part is why the four overrides whose replacement is itself a shifted keycode are
    made by ko_shifted_pair() below rather than by ko_make_basic(). `MOD_MASK_SHIFT` covers both
    Shift keys, and the German shifted keycodes are all left-Shift ones (`DE_DQUO` is `S(DE_2)`),
    so a ko_make_basic() override suppresses the Shift that was really held and re-adds a *left*
    one. Pressed with the right Shift that turns into a side swap — the modifier byte goes from
    RSFT to LSFT in the same breath as the replacement key is pressed — and a host that resolves
    the keycode before the modifier delta then types the unshifted character: `2` for `"`, `6`
    for `&`, `+` for `*`, `ß` for `?`. See issue #14, and QMK PR #25886, which splits that into
    two reports for the same reason.

    Keeping the Shift on the side that was really pressed sidesteps all of it: the modifier byte
    is then identical before and after, so there is no delta to race with, on either QMK version.

    The other overrides need no pair. Their replacement carries no Shift at all (`DE_HASH`,
    `DE_SS`, `DE_PLUS`) or a different modifier (`DE_AT` is `ALGR(DE_Q)`), so they change the
    modifier byte the same way whichever Shift is held, and ko_adia_tab suppresses nothing.

    Not listed here, because the German layout already pairs them the way the Cozy keymap wants:
      Shift 1 → !    Shift 4 → $    Shift 5 → %
      Shift , → ;    Shift . → :    Shift - → _
*/
// One override per Shift side, each replacing the trigger with `shifted` rebuilt on that same
// side. `shifted` is the ordinary `keymap_german.h` name, e.g. DE_AMPR; the two macros below take
// it apart and put it back together with the matching LSFT()/RSFT().
#define ko_shifted_pair(name, trigger, shifted)                                                                        \
    const key_override_t name##_l = ko_make_basic(MOD_BIT(KC_LSFT), trigger, LSFT(QK_MODS_GET_BASIC_KEYCODE(shifted))); \
    const key_override_t name##_r = ko_make_basic(MOD_BIT(KC_RSFT), trigger, RSFT(QK_MODS_GET_BASIC_KEYCODE(shifted)))

// The Shift layer of the number row: US ANSI, except for 6, 9 and 0 (see the ReadMe).
const key_override_t ko_2_at   = ko_make_basic(MOD_MASK_SHIFT, KC_2, DE_AT);   // @ instead of "
const key_override_t ko_3_hash = ko_make_basic(MOD_MASK_SHIFT, KC_3, DE_HASH); // # instead of §

const key_override_t ko_6_ss   = ko_make_basic(MOD_MASK_SHIFT, KC_6, DE_SS);   // ß instead of &
ko_shifted_pair(ko_7_ampr, KC_7, DE_AMPR);                                     // & instead of /
ko_shifted_pair(ko_8_astr, KC_8, DE_ASTR);                                     // * instead of (
const key_override_t ko_9_plus = ko_make_basic(MOD_MASK_SHIFT, KC_9, DE_PLUS); // + instead of ) – () move to the "stack of parentheses" on L_ALTGR
ko_shifted_pair(ko_0_ques, KC_0, DE_QUES);                                     // ? instead of =  – = is next to < and > on L_ALTGR

// The apostrophe / quote key uses the US ANSI pairing. In the German layout both characters sit on
// other keys (' is Shift+#, " is Shift+2), so the Shift pairing has to be made here.
ko_shifted_pair(ko_quot_dquo, DE_QUOT, DE_DQUO);

/*
    The "chameleon" ä key: with Ctrl, Alt or Gui held it becomes Tab, so that Ctrl+Tab, Alt+Tab and
    Win+Tab keep working from the key that used to be Tab. Plain ä and Shift+ä stay ä and Ä.
    This is exactly what MX_TABA did in custom code (which also tested for those three modifiers).

    This one cannot use ko_make_basic(), which would suppress the trigger modifier and re-add it as
    part of the replacement keycode: those "weak" mods are dropped every time the replacement is
    unregistered, so the window switcher would close between two taps of ä. Suppressing nothing
    instead leaves the physically held modifier in the keyboard report for as long as it is really
    down, so holding Alt (or Win) and tapping ä repeatedly walks through the window list.

    A held Shift is passed through the same way, which makes Ctrl+Shift+ä a Ctrl+Shift+Tab.
*/
const key_override_t ko_adia_tab = {
    .trigger           = DE_ADIA,
    .trigger_mods      = MOD_MASK_CAG, // Ctrl, Alt or Gui ...
    .options           = ko_options_all_activations | ko_option_one_mod, // ... any one of them suffices
    .suppressed_mods   = 0,            // keep the real modifier down, see above
    .negative_mod_mask = 0,
    .replacement       = KC_TAB,
    .layers            = ~0,
    .custom_action     = NULL,
    .context           = NULL,
    .enabled           = NULL,
};

const key_override_t *key_overrides[] = {
    &ko_2_at,
    &ko_3_hash,
    &ko_6_ss,
    &ko_7_ampr_l,
    &ko_7_ampr_r,
    &ko_8_astr_l,
    &ko_8_astr_r,
    &ko_9_plus,
    &ko_0_ques_l,
    &ko_0_ques_r,
    &ko_quot_dquo_l,
    &ko_quot_dquo_r,
    &ko_adia_tab,
    NULL,
};
