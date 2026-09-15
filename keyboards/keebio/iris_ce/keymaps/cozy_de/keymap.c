/*
    "The Cozy Keyboard" letter mapping with a mix of US ANSI and DE Qwertz shift mapping to accomodate for
    the reduced number of keys on the Iris keyboard.

    This is the "German OS layout" edition of the `cozy` keymap:
    it assumes that the computer is running the *standard* German keyboard layout (xkb's `de(basic)`,
    which is also what Windows calls "German (Germany)"), and it deliberately contains
     - no custom keycodes / no `process_record_user()` logic, except for printing the firmware version, and
     - no community modules (the `getreuer/custom_shift_keys` module is replaced by QMK's built-in Key Overrides).

    The physical key positions, the layer structure and all the QMK-specific keycodes are identical to `cozy`.
    Only the character-producing keycodes are swapped for their German counterparts.

    A copy of the xkb source of the assumed OS layout is in `reference/xkb/symbols/` of this repository,
    so that all the `#define`s below can be verified without a Linux box at hand.
 */

#include QMK_KEYBOARD_H
#if __has_include("keymap.h")
#    include "keymap.h"
#endif

// The standard German layout. Positions of the German characters on a physical US/ANSI-wired board.
#include "keymap_german.h"
// ... and the matching ASCII table, so that SEND_STRING() below types on a German layout, too.
#include "sendstring_german.h"

// for debugging only; needs the QMK Toolbox to receive. DOESN'T WORK YET!
#include "print.h"

#define VERSION_STRING "Layout ASDR_NILT standalone DE, rev22.2-german-os-layout"

/*
    Characters that xkb's `de(basic)` puts on the AltGr (level 3) and AltGr+Shift (level 4) levels
    but which QMK's `keymap_german.h` does not define.

    The level-3/4 mappings come partly from `symbols/de` itself and partly from the `latin(type4)` /
    `latin(basic)` layouts that it includes; both files are checked into `reference/xkb/symbols/`.
    The comment after each line names the xkb key code and the xkb keysym it produces.
*/
#define DE_NDSH ALGR(DE_MINS)     // – n-dash          <AB10> level 3: endash
#define DE_IEXL ALGR(S(DE_1))     // ¡ inverted !      <AE01> level 4: exclamdown
#define DE_IQUE ALGR(S(DE_SS))    // ¿ inverted ?      <AE11> level 4: questiondown
#define DE_QRTR ALGR(DE_4)        // ¼ one quarter     <AE04> level 3: onequarter
#define DE_HALF ALGR(DE_5)        // ½ one half        <AE05> level 3: onehalf
#define DE_PND  ALGR(S(DE_3))     // £ pound sign      <AE03> level 4: sterling
#define DE_CENT ALGR(DE_C)        // ¢ cent sign       <AB03> level 3: cent
#define DE_YEN  ALGR(S(DE_Z))     // ¥ yen sign        <AD06> level 4: yen
#define DE_AE   ALGR(DE_A)        // æ                 <AC01> level 3: ae
#define DE_OSTR ALGR(DE_O)        // ø                 <AD09> level 3: oslash
#define DE_MUL  ALGR(S(DE_COMM))  // × multiply sign   <AB08> level 4: multiply
#define DE_DDIA ALGR(DE_UDIA)     // ¨ combining diaeresis (dead key)  <AD11> level 3: dead_diaeresis

/*
    Keycodes of the `cozy` keymap that have no equivalent in the standard German layout
    and are therefore mapped to KC_NO here.
    (Sorted the way they appear in the layers below.)

    On the L_COMBINE layer:
     - US_TQTR ¾ – `de(basic)` inherits ¼ and ½ from `latin(basic)`, but `latin(type4)` replaces
                   the ¾ on AltGr+6 with ¬ (notsign). Only ¼ and ½ survive.
     - MX_CTIL  ̃ – the combining (dead) tilde only exists in the `de(deadtilde)` variant.
                   `de(basic)` has a live ~ on AltGr++, which is used on the AltGr layer below.
     - US_NTIL ñ – no precomposed ñ; and without a dead tilde it cannot be composed either.
     - US_CCED ç – no precomposed ç. (`de(basic)` has dead_cedilla on AltGr+´, but that is a dead key,
                   so it cannot be sent as a single keycode.)
     - US_EACU é – no precomposed é; type ´ then e instead (´ is on this layer).
     - MX_AGRV à – no precomposed à; type ` then a instead (` is on this layer).
     - MX_EGRV è – no precomposed è; type ` then e instead.
     - UC_oe   œ – was typed via QMK Unicode input, which is custom/OS-specific; not in `de(basic)`.

    On the L_ALTGR layer:
     - UC_PMIL ‰ – was typed via QMK Unicode input; the per mille sign only exists in the German
                   E1 extended layout (`de(e1)`), not in `de(basic)`.
     - MX_HAT  ^ – in `de(basic)` the ^ key is dead_circumflex, so a *live* ^ needs a trailing space,
                   which would require custom code. The dead version is on L_COMBINE as DE_CIRC.
     - MX_BTIC ` – likewise: Shift+´ is dead_grave, there is no live backtick.
                   The dead version is on L_COMBINE as DE_GRV.

    On the L_FN layer:
     - MX_TQM    – the ANSI/Windows "quote mode" switch is gone together with the custom code;
                   the OS layout now decides what the accent keys do.

    MX_TABA, the "chameleon" Tab/ä key, keeps its behavior without custom code: it is a plain
    DE_ADIA (ä/Ä), and the ko_adia_tab key override further down turns Ctrl+ä and Alt+ä into
    Ctrl+Tab and Alt+Tab. An unmodified Tab is on the L_ALTGR layer, as in `cozy`.
*/

enum layer_names {
    L_BASE,
    L_COMBINE,
    L_ALTGR,
    L_FN,
};
// The AltGr does almost everything that the AltGr level does in the software layout,
// therefore we don't need the AltGr modifier on the base layer. (But there's one on the Fn layer.)

// The only custom keycode left: printing the firmware version.
enum custom_keycodes {
    MX_VERS = SAFE_RANGE, // ugly hack: prints the firmware version.
};

/*
    Layer toggles. With additional tap function.
*/
#define L2_DEL   LT(L_ALTGR, KC_DEL)
#define L2_INS   LT(L_ALTGR, KC_INS)
#define L3_ESC   LT(L_FN, KC_ESC)
#define L3_ENT   LT(L_FN, KC_ENT)

// Since this layer should behave like a combining accent key, it is activated by a one-shot key press.
#define L_COMB   OSL(L_COMBINE)

// auxiliary layer keys, partly for historical reasons, partly because some combos are more comfortable this way.
// Note the Qwertz swap: the letter Y sits on the physical Z key and vice versa.
#define L2_Y     LT(L_ALTGR, DE_Y)
#define L2_MINS  LT(L_ALTGR, DE_MINS)

/*
    Various convenience keycodes.
*/
// The expose / task view key: a plain Win+Tab, no hold function.
// (Ubuntu only accepts the *left* Gui key for its expose feature, hence G() and not RGUI.)
// The Gui modifier itself stays on the left thumb key.
// Hint to open the start menu: use Ctrl+Esc.
#define MC_WINT  G(KC_TAB)

// previous and next word cursor navigation
// (This helps avoid pressing Ctrl modifier in addition to the layer toggle.)
#define KC_PRWD  LCTL(KC_LEFT)
#define KC_NXWD  LCTL(KC_RGHT)
// previous and next (virtual) desktop
#define KC_PRDESK G(C(KC_LEFT))
#define KC_NXDESK G(C(KC_RIGHT))
// screenshot (Windows Snipping Tool)
#define KC_SCRNS G(S(KC_S))

/*
    Comment for visually separating the actual keymap.
*/
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    // standard keyboard layer
    // The Shift mapping of the number row and of the `'` key is made by the key overrides below.
    [L_BASE] = LAYOUT(
            L3_ESC , KC_1, KC_2, KC_3, KC_4, KC_5   ,                     KC_6   , KC_7, KC_8   , KC_9  , KC_0   , KC_BSPC,
            DE_ADIA, KC_Q, KC_W, KC_B, KC_F, DE_ODIA,                     DE_Z   , KC_K, KC_U   , KC_O  , KC_P   , DE_UDIA,
            KC_LSFT, KC_A, KC_S, KC_D, KC_R, KC_G   ,                     KC_H   , KC_N, KC_I   , KC_L  , KC_T   , KC_RSFT,
            KC_LCTL, L2_Y, KC_X, KC_C, KC_V, DE_QUOT, KC_LGUI,    MC_WINT, KC_J  , KC_M, DE_COMM, DE_DOT, L2_MINS, L3_ENT,
                                    KC_LALT, L2_DEL , KC_SPC ,      KC_E , L2_INS, KC_RCTL
        ),
    // Extra letter layer, rarely used, since äöü are on base layer and ß is on AltGr.
    // accented letters: äöü ß æ ø.
    // combining accents: ´ ` ^ ¨ (the combining tilde is missing in de(basic), see the comment above).
    // other: µ ¼½ ¥ §.
    // Caution here: Esc and Backspace leave the layer, but still get sent to the computer with their L0 keycode.
    // Maybe QMK exits the one-shot layer when recognizing and layer-related keycode and then does the entire processing on the pre-OSL layer?
    [L_COMBINE] = LAYOUT(
            TO(0)  , DE_QRTR, DE_HALF, KC_NO  , DE_YEN , DE_SECT,                     DE_CIRC, DE_DDIA, DE_ACUT, DE_GRV , KC_NO  , TO(0)  ,
            KC_NO  , DE_AE  , KC_NO  , KC_NO  , KC_NO  , TO(0)  ,                     DE_SS  , KC_NO  , DE_UDIA, DE_ODIA, KC_NO  , KC_NO  ,
            KC_NO  , DE_ADIA, DE_SS  , KC_NO  , KC_NO  , KC_NO  ,                     KC_NO  , KC_NO  , KC_NO  , KC_NO  , DE_OSTR, KC_NO  ,
            KC_NO  , KC_NO  , KC_NO  , KC_NO  , KC_NO  , KC_NO  , KC_NO  ,   KC_NO  , KC_NO  , DE_MICR, KC_NO  , KC_NO  , KC_NO  , KC_NO  ,
                                                KC_NO  , KC_NO  , KC_NO  ,   KC_NO  , KC_NO  , KC_NO
        ),
    // Alternate character and navigation layer.
    // DE_TILD here is the 'live' (non-combining) tilde, as used in programming languages, among others.
    // Its siblings ^ and ` are dead keys in the German layout and thus only available on L_COMBINE.
    [L_ALTGR] = LAYOUT(
            KC_NO  , DE_IEXL, DE_CENT, DE_PND , DE_EURO, KC_NO  ,                       KC_NO  , DE_PIPE, DE_LBRC, DE_RBRC, DE_IQUE, KC_DEL ,
            KC_TAB ,S(KC_TAB),KC_PRWD, KC_UP  , KC_NXWD, L_COMB ,                       DE_SS  , DE_BSLS, DE_LCBR, DE_RCBR, DE_TILD, DE_DEG ,
            KC_LSFT, KC_HOME, KC_LEFT, KC_DOWN, KC_RGHT, KC_END ,                       KC_NO  , DE_SLSH, DE_LPRN, DE_RPRN, DE_SCLN, KC_RSFT,
            KC_LCTL, KC_PGUP, MS_WHLD, MS_WHLU, KC_PGDN, KC_ENT , KC_LGUI,     KC_LGUI, DE_MUL , DE_EQL , DE_LABK, DE_RABK, DE_NDSH, KC_INS ,
                                                KC_LALT, KC_TRNS, KC_TRNS,     KC_NO  , KC_TRNS, KC_RCTL
        ),
    // Function layer, like on a laptop.
    // Note that Hue cycles around, while Speed, Saturation and Value clamp at min/max.
    // Shift + any of the RGB Matric keys (RM_***) moves the other way.
    // C(KC_E) is a workaround for Terminal apps that don't handle the End key properly.
    [L_FN] = LAYOUT(
            KC_NO  , KC_F1  , KC_F2   , KC_F3  , KC_F4  , KC_F5  ,                       KC_F6  , KC_F7  , KC_F8  , KC_F9  , KC_F10 , EE_CLR ,
            MX_VERS, KC_F11 , KC_F12, KC_PRDESK, KC_NXDESK, KC_NO,                       RM_TOGG, RM_HUED, RM_SPDD, RM_SATD, RM_VALD, QK_BOOT,
            OS_LSFT, KC_MPRV, KC_SCRNS, KC_NO  , KC_NO  , C(KC_E),                       RM_NEXT, RM_HUEU, RM_SPDU, RM_SATU, RM_VALU, OS_RSFT,
            OS_LCTL, QK_BOOT, KC_NO   , KC_NO  , KC_NO  , KC_NO  , OS_LGUI,     OS_RGUI, KC_MSTP, KC_MPLY, KC_VOLD, KC_VOLU, KC_MUTE, KC_NO  ,
                                                 OS_LALT, OS_RALT, KC_NO  ,     KC_NO  , OS_RALT, OS_RCTL
        )
};

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

    Not listed here, because the German layout already pairs them the way the Cozy keymap wants:
      Shift 1 → !    Shift 4 → $    Shift 5 → %
      Shift , → ;    Shift . → :    Shift - → _
*/
// The Shift layer of the number row: US ANSI, except for 6, 9 and 0 (see the ReadMe).
const key_override_t ko_2_at   = ko_make_basic(MOD_MASK_SHIFT, KC_2, DE_AT);   // @ instead of "
const key_override_t ko_3_hash = ko_make_basic(MOD_MASK_SHIFT, KC_3, DE_HASH); // # instead of §
const key_override_t ko_6_ss   = ko_make_basic(MOD_MASK_SHIFT, KC_6, DE_SS);   // ß instead of &
                                                                               // (alternative position for the AltGr+Z mapping, same finger)
const key_override_t ko_7_ampr = ko_make_basic(MOD_MASK_SHIFT, KC_7, DE_AMPR); // & instead of /
const key_override_t ko_8_astr = ko_make_basic(MOD_MASK_SHIFT, KC_8, DE_ASTR); // * instead of (
const key_override_t ko_9_plus = ko_make_basic(MOD_MASK_SHIFT, KC_9, DE_PLUS); // + instead of ) – () move to the "stack of parentheses" on L_ALTGR
const key_override_t ko_0_ques = ko_make_basic(MOD_MASK_SHIFT, KC_0, DE_QUES); // ? instead of =  – = is next to < and > on L_ALTGR

// The apostrophe / quote key uses the US ANSI pairing. In the German layout both characters sit on
// other keys (' is Shift+#, " is Shift+2), so the Shift pairing has to be made here.
const key_override_t ko_quot_dquo = ko_make_basic(MOD_MASK_SHIFT, DE_QUOT, DE_DQUO);

/*
    The "chameleon" ä key: Ctrl+ä is Ctrl+Tab and Alt+ä is Alt+Tab, while plain ä and Shift+ä stay
    ä and Ä. This is what MX_TABA did in custom code, and it keeps Alt+Tab in its traditional place
    now that the ä key sits where Tab used to be.

    This one cannot use ko_make_basic(), which would suppress the trigger modifier and re-add it as
    part of the replacement keycode: those "weak" mods are dropped every time the replacement is
    unregistered, so the app switcher would close between two taps of ä. Suppressing nothing instead
    leaves the physically held Ctrl / Alt in the keyboard report for as long as it is really down,
    so holding Alt and tapping ä repeatedly walks through the window list as it should.

    A held Shift is passed through the same way, which makes Ctrl+Shift+ä a Ctrl+Shift+Tab.
*/
const key_override_t ko_adia_tab = {
    .trigger           = DE_ADIA,
    .trigger_mods      = MOD_MASK_CA, // Ctrl or Alt ...
    .options           = ko_options_all_activations | ko_option_one_mod, // ... any one of them suffices
    .suppressed_mods   = 0,           // keep the real modifier down, see above
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
    &ko_7_ampr,
    &ko_8_astr,
    &ko_9_plus,
    &ko_0_ques,
    &ko_quot_dquo,
    &ko_adia_tab,
    NULL,
};

// 3 ms still had some dropped letters.
const int SEND_STRING_DELAY_MS = 10;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case MX_VERS:
            if (record->event.pressed) {
                send_string_with_delay(VERSION_STRING, SEND_STRING_DELAY_MS);
                send_string_with_delay(__DATE__, SEND_STRING_DELAY_MS);
                send_string_with_delay("\n", SEND_STRING_DELAY_MS);
            }
            return false;
    }
    // all other cases to be handled by QMK.
    return true;
};

void keyboard_post_init_user(void) {
    debug_enable=true;
    println(VERSION_STRING);
    println(__DATE__);
    //debug_matrix=true;
    //debug_keyboard=true;
    //debug_mouse=true;
}
