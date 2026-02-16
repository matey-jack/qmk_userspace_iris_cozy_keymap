/*
    "The Cozy Keyboard" letter mapping with a mix of US ANSI and DE Qwertz shift mapping to accomodate for
    the reduced number of keys on the Iris keyboard.
 */

#include QMK_KEYBOARD_H
#if __has_include("keymap.h")
#    include "keymap.h"
#endif

// we import the Windows version of the file, since Linux has more key codes defined, but those don't work on Windows.
#include "keymap_us_international.h"

// for debugging only; needs the QMK Toolbox to receive. DOESN'T WORK YET!
#include "print.h"

#define VERSION_STRING "Layout ASDR_NILT standalone, rev21.7-anti-tab, "

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
    MX_VERS = SAFE_RANGE,
    MX_TABA,  // Chameleon key for Tab and äÄ.
    MX_TQM ,  // toggle quote mode
    // Next three are for characters that need different key taps on Google Pixel and other devices.
    // Toggled by MX_TQM.
    MX_QUOT,
    MX_ACUT,
    MX_DQUO,
    MX_HAT,
    // KC_GRV aka US_DGRV works same on both devices, it produces a dead grave.

    // The following are not device-dependent, but to save typing effort.
    MX_BTIC,  // "live" grave accent, doubling as "backtick" in programming.
    MX_TILD,  // "live" tilde for the Unix Shell...
    MX_FUER,  // Avoids same-finger trigram (f¨ur)

    // We could do the following two with Unicode, but someone said that Unicode is less reliable...
    MX_EGRV,
    MX_AGRV,
};

typedef enum {
    // this works for the standard US ANSI keyboard on all platforms,
    // but on Google Pixel 4a it even works with US ext. intl.
    // This is great and all OS' should do it like this!
    // And I am happy that I have independently invented this in my bespoke/support layout :-D
    QUOTE_MODE_ANSI,
    // This is for US ext intl Linux as in QMK's header file. (Currently not used.)
    QUOTE_MODE_LINUX,
    // Maybe I should call it quote mode Windows, because it seems to be the same and Windows is the better known implementation.
    QUOTE_MODE_SAMSUNG,
} quote_mode_t;

const char *const quote_mode_names[] = {
    [QUOTE_MODE_ANSI] = "ANSI",
    [QUOTE_MODE_LINUX] = "Linux",
    [QUOTE_MODE_SAMSUNG] = "Samsung",
};

// personal preference for my current main devices (Windows PC + Android Tablet).
quote_mode_t current_quote_mode = QUOTE_MODE_SAMSUNG;

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
#define UC_BOT  UC(0x22A5) // bottom ⊥ – doesn't work :(
/*
    Comment for visually separating the actual keymap.
*/
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    // standard keyboard layer
    [L_BASE] = LAYOUT(
            L3_ESC , KC_1, KC_2, KC_3, KC_4, KC_5   ,                     KC_6   , KC_7, KC_8   , KC_9  , KC_0   , KC_BSPC,
            MX_TABA, KC_Q, KC_W, KC_B, KC_F, US_ODIA,                     KC_Z   , KC_K, KC_U   , KC_O  , KC_P   , US_UDIA ,
            KC_LSFT, KC_A, KC_S, KC_D, KC_R, KC_G   ,                     KC_H   , KC_N, KC_I   , KC_L  , KC_T   , KC_RSFT,
            KC_LCTL, L2_Y, KC_X, KC_C, KC_V, MX_QUOT, MC_WINT,    MC_WINT, KC_J  , KC_M, KC_COMM, KC_DOT, L2_MINS, L3_ENT,
                                    KC_LALT, L2_DEL , KC_SPC ,      KC_E , L2_INS, KC_RCTL
        ),
    // Extra letter layer, rarely used, since äöü are on base layer and ß is on AltGr.
    // accented letters: äöü ß àèé çñ æ œ.
    // combining accents: á à ã â ä
    // other: µ ¼½¾.
    // Caution here: Esc and Backspace leave the layer, but still get sent to the computer with their L0 keycode.
    // Maybe QMK exits the one-shot layer when recognizing and layer-related keycode and then does the entire processing on the pre-OSL layer?
    [L_COMBINE] = LAYOUT(
            TO(0)  , US_QRTR, US_HALF, US_TQTR, KC_NO  , KC_NO  ,                     US_DCIR, US_DIAE, MX_ACUT, US_DGRV, US_DTIL, TO(0)  ,
            KC_NO  , US_AE  , KC_NO  , KC_NO  , MX_FUER, TO(0)  ,                     US_SS  , KC_NO  , US_UDIA, US_ODIA, KC_NO  , KC_NO  ,
            KC_NO  , US_ADIA, US_SS  , KC_NO  , KC_NO  , KC_NO  ,                     KC_NO  , US_NTIL, KC_NO  , UC_oe  , US_OSTR, KC_NO  ,
            KC_NO  , MX_AGRV, KC_NO  , US_CCED, KC_NO  , KC_NO  , KC_NO  ,   KC_NO  , KC_NO  , US_MICR, KC_NO  , KC_NO  , KC_NO  , KC_NO  ,
                                                KC_NO  , KC_NO  , KC_NO  ,   US_EACU, MX_EGRV, KC_NO
        ),
    // Alternate character and navigation layer.
    // MX_HAT, MX_BTIC, and MX_TILD here are the 'live' (non-combining) keys (as used in programming languages, among others).
    [L_ALTGR] = LAYOUT(
            KC_NO  , US_YEN , US_CENT, US_PND , US_EURO, UC_PMIL,                       MX_HAT , KC_PIPE, KC_LBRC, KC_RBRC, US_SECT, KC_DEL ,
            KC_TAB ,S(KC_TAB),KC_PRWD, KC_UP  , KC_NXWD, L_COMB ,                       US_SS  , KC_BSLS, KC_LCBR, KC_RCBR, MX_TILD, US_DEG ,
            KC_LSFT, KC_HOME, KC_LEFT, KC_DOWN, KC_RGHT, KC_END ,                       MX_BTIC, KC_SLSH, KC_LPRN, KC_RPRN, KC_SCLN, KC_RSFT,
			KC_LCTL, KC_PGUP, MS_WHLD, MS_WHLU, KC_PGDN, KC_ENT , KC_LGUI,     KC_RGUI, US_MUL , KC_EQL , KC_LT  , KC_GT  , UC_NDSH, KC_INS ,
                                                KC_LALT, KC_TRNS, KC_ENT ,     KC_NO  , KC_TRNS, KC_RCTL
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
        //     current_quote_mode = QUOTE_MODE_LINUX;
        //     break;
        case QUOTE_MODE_LINUX:
            current_quote_mode = QUOTE_MODE_SAMSUNG;
            break;
        case QUOTE_MODE_SAMSUNG:
            current_quote_mode = QUOTE_MODE_ANSI;
            break;
    }
    // TODO: flash the LED on letter A, L or S to indicate the new quote mode.
    // Actually, that's not as important, since I will usually test the quote right after switching.
    // More useful, however, would be to do the switching on a further-away layer (such as L_COMBINE) to avoid accidental presses.
}

static bool taba_was_modded = false;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // all the keys modified in this way use tap_code() for sending to the computer.
    // this means they will only send something on tap and then appear to be released immediately, no matter how long you hold them.
    switch (keycode) {
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
        case MC_WINT:
            // only handle the press event in "tap" mode. (Hold mode is fully handled by QMK.)
            if (record->tap.count && record->event.pressed) {
                tap_code16(RWIN(KC_TAB));
                return false;
            }
            break;
        case MX_FUER: // obsolete, to be removed!
            if (!record->event.pressed) return false;
            tap_code(KC_F);
            tap_code16(US_UDIA);  // 16 bits, because the keycode has the AltGr bits set.
            tap_code(KC_R);
            return false;
        case MX_QUOT:
            if (!record->event.pressed) return false;
            switch (current_quote_mode) {
                case QUOTE_MODE_ANSI:
                    tap_code(KC_QUOT);
                    return false;
                case QUOTE_MODE_LINUX:
                    tap_code16(ALGR(KC_QUOT));
                    return false;
                case QUOTE_MODE_SAMSUNG:
                    // it's a dead key with only one mapping, namely the key itself.
                    tap_code(KC_QUOT);
                    tap_code(KC_SPACE);
                    return false;
            }
        case MX_DQUO:
        // This is all unused code, since the shift layer was removed.
        // Instead the MX_QUOT above will be used with the Shift bit passed implicitly.
            if (!record->event.pressed) return false;
            switch (current_quote_mode) {
                case QUOTE_MODE_ANSI:
                    // need tap_code16, because it actually needs to press and hold Shift as part of this virtual key code.
                    tap_code16(KC_DQUO);
                    return false;
                case QUOTE_MODE_LINUX:
                    // Explicit AltGr, implicit Shift.
                    tap_code16(ALGR(KC_DQUO));
                    return false;
                case QUOTE_MODE_SAMSUNG:
                    // it's a dead key with only one mapping, namely the key itself.
                    tap_code16(KC_DQUO);
                    tap_code(KC_SPACE);
                    return false;
            }
        case MX_ACUT:
            if (!record->event.pressed) return false;
            switch (current_quote_mode) {
                case QUOTE_MODE_ANSI:
                    tap_code16(ALGR(KC_QUOT));
                    return false;
                case QUOTE_MODE_LINUX:
                case QUOTE_MODE_SAMSUNG:
                    tap_code(KC_QUOT);
                    return false;
            }
        case MX_HAT:
            if (!record->event.pressed) return false;
            switch (current_quote_mode) {
                case QUOTE_MODE_ANSI:
                    // implicitly contains Shift
                    tap_code16(KC_CIRC);
                    return false;
                case QUOTE_MODE_LINUX:
                case QUOTE_MODE_SAMSUNG:
                    tap_code16(KC_CIRC);
                    tap_code(KC_SPACE);
                    return false;
            }
        case MX_BTIC:
            if (!record->event.pressed) return false;
            tap_code(US_DGRV);   // base layer key
            tap_code(KC_SPACE);
            return false;
        case MX_TILD:
            if (!record->event.pressed) return false;
            tap_code16(US_DTIL); // implicit Shift
            tap_code(KC_SPACE);
            return false;
        case MX_EGRV:
            if (!record->event.pressed) return false;
            // TODO: handle the Shift case, because is currently transforms DGRV into DTIL!
            // OTOH, the capital version of this is almost never used, and can be made with the explicit accent key anyway.
            tap_code(US_DGRV);
            tap_code(KC_E);
            return false;
        case MX_AGRV:
            if (!record->event.pressed) return false;
            // TODO: handle the Shift case, because is currently transforms DGRV into DTIL!
            // OTOH, the capital version of this is almost never used, and can be made with the explicit accent key anyway.
            tap_code(US_DGRV);
            tap_code(KC_A);
            return false;
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
        case MX_TQM:
            if (record->event.pressed) {
                toggle_quote_mode();
            } else {
                // when keycode is released
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
