#ifndef COZY_DE_QUOTE_KEYMAP_ONCE
#define COZY_DE_QUOTE_KEYMAP_ONCE

#include "tests/test_common/keymap.c"

// The relevant subset of keymap_german.h
#define DE_2    KC_2
#define DE_6    KC_6
#define DE_Q    KC_Q
#define DE_HASH KC_NUHS
#define DE_PLUS KC_RBRC
#define DE_SS   KC_MINS
#define DE_QUOT S(DE_HASH) // '
#define DE_DQUO S(DE_2)    // "
#define DE_AT   ALGR(DE_Q) // @
#define DE_AMPR S(DE_6)    // &
#define DE_ASTR S(DE_PLUS) // *
#define DE_QUES S(DE_SS)   // ?
#define DE_ADIA KC_QUOT    // ae

// Mirrors keyboards/keebio/iris_ce/keymaps/cozy_de/keymap.c
#define ko_shifted_pair(name, trigger, shifted)                                                                        \
    const key_override_t name##_l = ko_make_basic(MOD_BIT(KC_LSFT), trigger, LSFT(QK_MODS_GET_BASIC_KEYCODE(shifted))); \
    const key_override_t name##_r = ko_make_basic(MOD_BIT(KC_RSFT), trigger, RSFT(QK_MODS_GET_BASIC_KEYCODE(shifted)))

const key_override_t ko_2_at   = ko_make_basic(MOD_MASK_SHIFT, KC_2, DE_AT);
const key_override_t ko_3_hash = ko_make_basic(MOD_MASK_SHIFT, KC_3, DE_HASH);
const key_override_t ko_6_ss   = ko_make_basic(MOD_MASK_SHIFT, KC_6, DE_SS);
ko_shifted_pair(ko_7_ampr, KC_7, DE_AMPR);
ko_shifted_pair(ko_8_astr, KC_8, DE_ASTR);
const key_override_t ko_9_plus = ko_make_basic(MOD_MASK_SHIFT, KC_9, DE_PLUS);
ko_shifted_pair(ko_0_ques, KC_0, DE_QUES);
ko_shifted_pair(ko_quot_dquo, DE_QUOT, DE_DQUO);

const key_override_t ko_adia_tab = {
    .trigger           = DE_ADIA,
    .trigger_mods      = MOD_MASK_CAG,
    .options           = ko_options_all_activations | ko_option_one_mod,
    .suppressed_mods   = 0,
    .negative_mod_mask = 0,
    .replacement       = KC_TAB,
    .layers            = ~0,
    .custom_action     = NULL,
    .context           = NULL,
    .enabled           = NULL,
};

const key_override_t *key_overrides[] = {
    &ko_2_at,       &ko_3_hash,      &ko_6_ss,
    &ko_7_ampr_l,   &ko_7_ampr_r,
    &ko_8_astr_l,   &ko_8_astr_r,
    &ko_9_plus,
    &ko_0_ques_l,   &ko_0_ques_r,
    &ko_quot_dquo_l,&ko_quot_dquo_r,
    &ko_adia_tab,
    NULL,
};

#endif
