/*
    "The Cozy Keyboard" letter mapping with a mix of US ANSI and DE Qwertz shift mapping to accomodate for
    the reduced number of keys on the Iris keyboard.
 */

#include QMK_KEYBOARD_H
#if __has_include("keymap.h")
#    include "keymap.h"
#endif

// we import the Windows version of the file, since it's the smaller set and its keycodes work on Linux, too.
#include "keymap_us_international.h"

// for debugging only; needs the QMK Toolbox to receive. DOESN'T WORK YET!
#include "print.h"

#define VERSION_STRING "Layout ASDR_NILT standalone, rev21.10-pure-left-gui"

enum layer_names {
    L_BASE,
    L_COMBINE,
    L_ALTGR,
    L_FN,
};
// The AltGr does almost everything that the AltGr level does in the software layout,
// therefore we don't need the AltGr modifier on the base layer. (But there's one on the Fn layer.)

// Macros!
enum custom_keycodes {
    MX_VERS = SAFE_RANGE, // ugly hack: prints the firmware version.
    MX_TABA,  // Chameleon key for Tab and äÄ.
    MX_TQM ,  // toggle quote mode: adapts quote and accent keys between ANSI and Windows International mode.

    // Ironically the "grave tilde" and "6 circumflex" keys were added to typewriters to write combining accents.
    // But early computers appropriated those characters to create some concise programming syntax.
    // When combining accents were needed later, some systems introduced them separately on the AltGr layer,
    // while others replaced the programmer's characters with those combining accents.
    // Now we have to switch the keyboard between both... see quote_mode_t below.

    // Keys that should produce the ANSI characters: quotes and programmer's punctuation.
    MX_QUOT,  // single quote which in ANSI is the same as the apostrophe
    MX_DQUO,  // double quote
    MX_BTIC,  // programmer's "backtick", the non-combining grave accent.
    MX_TILD,  // programmer's tilde for the Unix Shell...
    MX_HAT,   // programmer's circumflex

    // Combining accents, aka dead keys, which only work on internationalized layouts (ANSI-compatible or not).
    // Same order as their programmer's versions above.
    MX_ACUT, // acute
    MX_DIA,  // diaeresis
    MX_GRV,  // grave
    MX_CTIL, // combining tilde
    MX_CIRC, // circumflex

    // We could do the following two with Unicode, but someone said that Unicode is less reliable...
    MX_EGRV,
    MX_AGRV,
};

typedef enum {
    // This works for the standard US ANSI keyboard on all platforms.
    // It also works with xkb's us(altgr-intl) in which case it also includes dead accents without redefining any of the ANSI keys.
    // (Google Pixel phones seem to use this version of the US intl keymap as well!)
    QUOTE_MODE_ANSI,
    // Linux also offers a variant of us(intl) which works like this, althoug GTK somehow messes with the compose table.
    // But it's the preferred choice for a computer that runs Linux and Windows at the same time, one of them in a VM or via RDP.0
    // My Samsung Android tablets also use this.
    QUOTE_MODE_WINDOWS,
} quote_mode_t;
/*
  What keycodes need to be sent to create the reuired character or trigger the required compose table in each mode:
   - ANSI: plain keycode creates the ANSI punctuation / programmer's version; AltGr+keycode triggers the accent combiner.
   - WINDOWS: plain keycode triggers the accent combiner; keycode followed by space inserts the plain ANSI character.
   Note that in three cases the "plain keycode" includes a pressed shift key.
*/

const char *const quote_mode_names[] = {
    [QUOTE_MODE_ANSI] = "ANSI",
    [QUOTE_MODE_WINDOWS] = "Samsung",
};

// personal preference for my current main devices (Windows PC + Android Tablet).
quote_mode_t current_quote_mode = QUOTE_MODE_WINDOWS;

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
#define L2_Y     LT(L_ALTGR, KC_Y)
#define L2_MINS  LT(L_ALTGR, KC_MINS)

/*
    Various convenience keycodes.
*/
// Mod/Tap for Win and Win+Tab. Need custom code below to make it work, because the MT macro doesn't support modifier bits in the second arg.
// Hint to open the start menu: use Ctrl+Esc. (Or L2 + a tab on the Gui key.)
#define MC_WINT  MT(MOD_RGUI, KC_TAB)

// previous and next word cursor navigation
// (This helps avoid pressing Ctrl modifier in addition to the layer toggle.)
#define KC_PRWD  LCTL(KC_LEFT)
#define KC_NXWD  LCTL(KC_RGHT)
// previous and next (virtual) desktop
#define KC_PRDESK G(C(KC_LEFT))
#define KC_NXDESK G(C(KC_RIGHT))
// screenshot (Windows Snipping Tool)
#define KC_SCRNS G(S(KC_S))

// Some delicious Unicode characters, that don't have a direct mapping in US intl. layout.
// Only tested and enabled on Windows (see config.h).
#define UC_PMIL UC(0x2030) // per mille sign ‰
#define UC_NDSH UC(0x2013) // en-dash –  // interestingly, Windows also types that with G(KC_MINS) 🤯
#define UC_oe   UC(0x0153) // œ fun-fact: the sad story of why this is not in Latin-1 and thus not in the OS' keymap: https://en.wikipedia.org/wiki/ISO/IEC_8859-1
/*
    Comment for visually separating the actual keymap.
*/
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    // standard keyboard layer
    [L_BASE] = LAYOUT(
            L3_ESC , KC_1, KC_2, KC_3, KC_4, KC_5   ,                     KC_6   , KC_7, KC_8   , KC_9  , KC_0   , KC_BSPC,
            MX_TABA, KC_Q, KC_W, KC_B, KC_F, US_ODIA,                     KC_Z   , KC_K, KC_U   , KC_O  , KC_P   , US_UDIA ,
            KC_LSFT, KC_A, KC_S, KC_D, KC_R, KC_G   ,                     KC_H   , KC_N, KC_I   , KC_L  , KC_T   , KC_RSFT,
            KC_LCTL, L2_Y, KC_X, KC_C, KC_V, MX_QUOT,MOD_RGUI,    MC_WINT, KC_J  , KC_M, KC_COMM, KC_DOT, L2_MINS, L3_ENT,
                                    KC_LALT, L2_DEL , KC_SPC ,      KC_E , L2_INS, KC_RCTL
        ),
    // Extra letter layer, rarely used, since äöü are on base layer and ß is on AltGr.
    // accented letters: äöü ß àèé çñ æ œ.
    // combining accents: á à ã â ä
    // other: µ ¼½¾.
    // Caution here: Esc and Backspace leave the layer, but still get sent to the computer with their L0 keycode.
    // Maybe QMK exits the one-shot layer when recognizing and layer-related keycode and then does the entire processing on the pre-OSL layer?
    [L_COMBINE] = LAYOUT(
            TO(0)  , US_QRTR, US_HALF, US_TQTR, US_YEN , US_SECT,                     US_DCIR, US_DIAE, MX_ACUT, US_DGRV, US_DTIL, TO(0)  ,
            KC_NO  , US_AE  , KC_NO  , KC_NO  , KC_NO  , TO(0)  ,                     US_SS  , KC_NO  , US_UDIA, US_ODIA, KC_NO  , KC_NO  ,
            KC_NO  , US_ADIA, US_SS  , KC_NO  , KC_NO  , KC_NO  ,                     KC_NO  , US_NTIL, KC_NO  , UC_oe  , US_OSTR, KC_NO  ,
            KC_NO  , MX_AGRV, KC_NO  , US_CCED, KC_NO  , KC_NO  , KC_NO  ,   KC_NO  , KC_NO  , US_MICR, KC_NO  , KC_NO  , KC_NO  , KC_NO  ,
                                                KC_NO  , KC_NO  , KC_NO  ,   US_EACU, MX_EGRV, KC_NO
        ),
    // Alternate character and navigation layer.
    // MX_HAT, MX_BTIC, and MX_TILD here are the 'live' (non-combining) keys (as used in programming languages, among others).
    [L_ALTGR] = LAYOUT(
            KC_NO  , US_IEXL, US_CENT, US_PND , US_EURO, UC_PMIL,                       MX_HAT , KC_PIPE, KC_LBRC, KC_RBRC, US_IQUE, KC_DEL ,
            KC_TAB ,S(KC_TAB),KC_PRWD, KC_UP  , KC_NXWD, L_COMB ,                       US_SS  , KC_BSLS, KC_LCBR, KC_RCBR, MX_TILD, US_DEG ,
            KC_LSFT, KC_HOME, KC_LEFT, KC_DOWN, KC_RGHT, KC_END ,                       MX_BTIC, KC_SLSH, KC_LPRN, KC_RPRN, KC_SCLN, KC_RSFT,
			KC_LCTL, KC_PGUP, MS_WHLD, MS_WHLU, KC_PGDN, KC_ENT , KC_LGUI,     KC_RGUI, US_MUL , KC_EQL , KC_LT  , KC_GT  , UC_NDSH, KC_INS ,
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
			OS_LCTL, QK_BOOT, KC_NO   , KC_NO  , KC_NO  , MX_TQM , OS_LGUI,     OS_RGUI, KC_MSTP, KC_MPLY, KC_VOLD, KC_VOLU, KC_MUTE, KC_NO  ,
                                                 OS_LALT, OS_RALT, KC_NO  ,     KC_NO  , OS_RALT, OS_RCTL
        )
};

const custom_shift_key_t custom_shift_keys[] = {
  // The live hat ^ of US intl. needs more than one tap, thus we can't have it here.
  // This is an alternative Shift-layer position for ß, using the same finger as the AltGr+Z mapping.
  {KC_6   , US_SS  }, // Shift 6 is ß
  // This replaces the () and <> characters which move to the "stack of parenthesis" on the AltGr layer.
  {KC_9   , KC_PLUS}, // Shift 9 is +
  {KC_0   , KC_QUES}, // Shift 0 is ?
  {KC_DOT , KC_COLN}, // Shift . is :
  {KC_COMM, KC_SCLN}, // Shift , is ;
};

// 3 ms still had some dropped letters.
const int SEND_STRING_DELAY_MS = 10;

void toggle_quote_mode(void) {
    switch (current_quote_mode) {
        case QUOTE_MODE_ANSI:
            current_quote_mode = QUOTE_MODE_WINDOWS;
            break;
        case QUOTE_MODE_WINDOWS:
            current_quote_mode = QUOTE_MODE_ANSI;
            break;
    }
    // TODO: flash the LED on letter A, L or S to indicate the new quote mode.
    // Actually, that's not as important, since I will usually test the quote right after switching.
    // More useful, however, would be to do the switching on a further-away layer (such as L_COMBINE) to avoid accidental presses.
}

/*
    Helpers for the accent keys and their ANSI partners.

    The first two take the keycode of the *plain* key of the US intl. layout, which for three of the five
    keys implicitly contains a Shift (hence tap_code16 everywhere). All of them return false, so that the
    cases in process_record_user() can simply `return send_xyz(...);` and tell QMK that we handled the key.
*/

// The ANSI punctuation, aka the programmer's version of the character.
static bool send_ansi_punctuation(uint16_t plain_keycode) {
    tap_code16(plain_keycode);
    if (current_quote_mode == QUOTE_MODE_WINDOWS) {
        // The plain key is a dead key here, so it needs a space to commit the standalone character.
        tap_code(KC_SPACE);
    }
    return false;
}

// The combining accent, aka the dead key.
static bool send_combining_accent(uint16_t plain_keycode) {
    if (current_quote_mode == QUOTE_MODE_ANSI) {
        // The dead keys live on the AltGr level, leaving the ANSI characters where they belong.
        tap_code16(ALGR(plain_keycode));
    } else {
        tap_code16(plain_keycode);
    }
    return false;
}

// Shortcut for a letter with a combining grave accent, see MX_EGRV and MX_AGRV below.
static bool send_grave_letter(uint16_t letter_keycode) {
    send_combining_accent(US_DGRV);
    tap_code(letter_keycode);
    return false;
}

static bool taba_was_modded = false;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // All the keys modified in this way use tap_code() for sending to the computer.
    // This means they will only send something on tap and then appear to be released immediately, no matter how long you hold them.
    switch (keycode) {
        case MC_WINT:
            // only handle the press event in "tap" mode. (Hold mode is fully handled by QMK.)
            if (record->tap.count && record->event.pressed) {
                tap_code16(RWIN(KC_TAB));
                return false;
            }
            break;

        case MX_VERS:
            if (record->event.pressed) {
                send_string_with_delay(VERSION_STRING, SEND_STRING_DELAY_MS);
                send_string_with_delay(__DATE__, SEND_STRING_DELAY_MS);
                send_string_with_delay("\nQuote mode: ", SEND_STRING_DELAY_MS);
                send_string_with_delay(quote_mode_names[current_quote_mode], SEND_STRING_DELAY_MS);
                send_string_with_delay("\n", SEND_STRING_DELAY_MS);
            } else {
                // when keycode is released
            }
            return false;
        case MX_TABA:
            // Should behave as Tab when Alt, Ctrl, or Gui is held. Should behave as ä in all other cases (including when just Shift is pressed).
            const uint8_t mods    = get_mods();
            // only need to check the left ones, because the right ones have one extra bit that we can ignore.
            uint8_t       modded = mods & (MOD_LCTL | MOD_LALT | MOD_LGUI);
            if (record->event.pressed) {
                taba_was_modded = modded;
                if (modded) {
                    tap_code(KC_TAB);
                } else {
                    tap_code16(ALGR(KC_Q));
                }
            }
            return false;
        case MX_TQM:
            if (record->event.pressed) {
                toggle_quote_mode();
            } else {
                // when keycode is released
            }
            return false;

        /* * * * * * * * * * * * * * * * * * * * * * * * * * *

            Combining accents and their plain counterparts.

        * * * * * * * * * * * * * * * * * * * * * * * * * * */
        // The ANSI punctuation / programmer's characters, in the same order as in the enum.
        case MX_QUOT:
            // Plain ASCII single quote and apostrophe.
            if (!record->event.pressed) return false;
            return send_ansi_punctuation(KC_QUOT);
        case MX_DQUO:
            // This is currently unused code, since the shift layer was removed.
            // Instead the MX_QUOT above will be used with the Shift bit passed implicitly.
            // We keep it anyhow, in case that we want to map this key to another layer.
            if (!record->event.pressed) return false;
            return send_ansi_punctuation(KC_DQUO); // implicit Shift
        case MX_BTIC:
            if (!record->event.pressed) return false;
            return send_ansi_punctuation(KC_GRV);
        case MX_TILD:
            if (!record->event.pressed) return false;
            return send_ansi_punctuation(KC_TILD); // implicit Shift
        case MX_HAT:
            if (!record->event.pressed) return false;
            return send_ansi_punctuation(KC_CIRC); // implicit Shift

        // The combining accents, same key order as their programmer's versions above.
        // Note that the US_* key codes despite their names are alias of the same numeric codes as the KC_* ones above.
        // The difference in behavior is entirely in the function that is called.
        case MX_ACUT:
            if (!record->event.pressed) return false;
            return send_combining_accent(US_ACUT);
        case MX_DIA:
            if (!record->event.pressed) return false;
            return send_combining_accent(US_DIAE); // implicit Shift
        case MX_GRV:
            if (!record->event.pressed) return false;
            return send_combining_accent(US_DGRV);
        case MX_CTIL:
            if (!record->event.pressed) return false;
            return send_combining_accent(US_DTIL); // implicit Shift
        case MX_CIRC:
            if (!record->event.pressed) return false;
            return send_combining_accent(US_DCIR); // implicit Shift

        // Shortcuts to two very common accented letters that (unlike e-acute and n-tilde) aren't already defined in the US int'l character map.
        case MX_EGRV:
            if (!record->event.pressed) return false;
            // This is a shortcut that works for the minuscule è only, because the presence of a pressed Shift key transforms the accent into a tilde.
            // If the capital version is ever needed (which it isn't in any language I am writing), then it can be made by pressing the combining sequence manually.
            return send_grave_letter(KC_E);
        case MX_AGRV:
            if (!record->event.pressed) return false;
            // This is a shortcut that works for the minuscule à only; see MX_EGRV above.
            return send_grave_letter(KC_A);
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
