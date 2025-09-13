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

enum layer_names {
    L_BASE,
    L_COMBINE,
    L_ALTGR,
    L_FN,
};
// "Shift" and "AltGr" keys in this layout are layer switches, thus send nothing to the computer.
// But the Shift key on the AltGr and Fn layers is mapped to a normal Shift.
// This can be used both for expected behavior of cursor and navigation keys.
// It can also be used to send plain Shift to the computer to combine it with mouse clicks.
// (Remember that when pressing AltGr+Shift AltGr is only a layer switch, so that the computer sees only Shift.)
// Unfortunately, since the LT(,) macro only supports basic keycodes, we always have to press first AltGr then Shift to do selections.
// With Shift already pressed, the two AltGr keys become simple Y and _ keys. (Which is not the case when using pure Shift...)

// Macros!
enum custom_keycodes {
    MX_VERS = SAFE_RANGE,
    MX_TQM ,  // toggle quote mode
    // Next three are for characters that need different key taps on Google Pixel and other devices.
    // Toggled by MX_TQM.
    MX_QUOT,
    MX_ACUT,
    MX_DQUO,
    // KC_GRV aka US_DGRV works same on both devices, it produces a dead grave.

    // The following are not device-dependent, but to save typing effort.
    MX_BTIC,  // "live" grave accent, doubling as "backtick" in programming.
    MX_TILD,  // "live" tilde for the Unix Shell...
    MX_FUER,  // Avoids same-finger trigram (f¨ur)

    // We could do the following two with Unicode, but someone said that Unicode is less reliable...
    MX_EGRV,
    MX_AGRV,
};

enum quote_mode {
    // this works for the standard US ANSI keyboard on all platforms,
    // but on Google Pixel 4a it even works with US ext. intl.
    // This is great and all OS' should do it like this!
    // And I am happy that I have independently invented this in my bespoke/support layout :-D
    QUOTE_MODE_ANSI,
    // This is for US ext intl Linux as in QMK's header file. (Currently not used.)
    QUOTE_MODE_LINUX,
    // Maybe I should call it quote mode Windows, because it seems to be the same and Windows is the better known implementation.
    QUOTE_MODE_SAMSUNG,
};

const char *const quote_mode_names[] = {
    [QUOTE_MODE_ANSI] = "ANSI",
    [QUOTE_MODE_LINUX] = "Linux",
    [QUOTE_MODE_SAMSUNG] = "Samsung",
};

// personal preference for my current main devices (Windows PC + Android Tablet).
int current_quote_mode = QUOTE_MODE_SAMSUNG;

/*
    Layer toggles. With additional tap function.
*/
#define L2_DEL   LT(L_ALTGR, KC_DEL)
#define L2_ENT   LT(L_ALTGR, KC_ENT)
#define L3_ESC   LT(L_FN, KC_ESC)
#define L3_INS   LT(L_FN, KC_INS)

// Since this layer should behave like a combining accent key, it is activated by a one-shot key press.
#define L_COMB   OSL(L_COMBINE)

// auxiliary layer keys, partly for historical reasons, partly because some combos are more comfortable this way.
#define L2_Y     LT(L_ALTGR, KC_Y)
#define L2_MINS  LT(L_ALTGR, KC_MINS)

/*
    Modifier keys.
*/
// One-shot-mods as an optional way to enter shortcuts with several modifiers.
#define OSM_ALT  OSM(MOD_LALT)
#define OSM_CTL  OSM(MOD_LCTL)
#define OSM_SFT  OSM(MOD_LSFT)
#define OSM_GUI  OSM(MOD_LGUI)

// One-shot-mod AltGr key, so we can access all characters from software layout AltGr, that don't have
// a direct mapping in our firmware AltGr layer. (Meant for rare characters and as workaround for mapping bugs.)
#define OSM_AGR  OSM(MOD_RALT)

/*
    Various convenience keycodes.
*/
// Mod/Tap for Win and Win+Tab. Need custom code below to make it work, because the MT macro doesn't support modifier bits in the second arg.
#define MC_WINT  MT(MOD_RGUI, KC_TAB)

// previous and next word cursor navigation
// (This helps avoid pressing Ctrl modifier in addition to the layer toggle.)
#define KC_PRWD  LCTL(KC_LEFT)
#define KC_NXWD  LCTL(KC_RGHT)
#define KC_TOP   LCTL(KC_HOME)
#define KC_BOTT  LCTL(KC_END )

// Some delicious Unicode characters, that don't have a direct mapping in US intl. layout.
// Only tested and enabled on Windows (see config.h).
#define UC_PMIL UC(0x2030) // per mille sign
#define UC_NDSH UC(0x2013) // en-dash
#define UC_oe   UC(0x0153) // œ - don't ask me why US ext. intl. has æ, but not œ! Pourtant ils aiment manger le bœf!
#define UC_OE   UC(0x0152) // Œ

/*
    Comment for visually separating the actual keymap.
*/
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    // standard keyboard layer
    [L_BASE] = LAYOUT(
            L3_ESC , KC_1, KC_2, KC_3, KC_4, KC_5   ,                     KC_6   , KC_7, KC_8   , KC_9  , KC_0   , KC_BSPC,
            KC_TAB , KC_Q, KC_W, KC_B, KC_F, L_COMB ,                     KC_Z   , KC_K, KC_U   , KC_O  , KC_P   , KC_EQL ,
            KC_LSFT, KC_A, KC_S, KC_D, KC_R, KC_G   ,                     KC_H   , KC_N, KC_I   , KC_L  , KC_T   , KC_RSFT,
            KC_LCTL, L2_Y, KC_X, KC_C, KC_V, MX_QUOT, KC_LGUI,   MC_WINT, KC_J   , KC_M, KC_COMM, KC_DOT, L2_MINS, L3_INS,
                                  KC_LALT, L2_DEL , KC_SPC ,       KC_E, L2_ENT , KC_RCTL
        ),
    // Current accented letters: äöü ß àèé çñ.
    // Current extras: µ.
    [L_COMBINE] = LAYOUT(
            KC_TRNS, US_QRTR, US_HALF, US_TQTR, KC_TRNS, KC_TRNS,                     US_DCIR, US_DIAE, MX_ACUT, US_DGRV, US_DTIL, KC_TRNS,
            KC_TRNS, US_AE  , KC_TRNS, KC_TRNS, MX_FUER, KC_TRNS,                     US_SS  , KC_TRNS, US_UDIA, US_ODIA, KC_TRNS, KC_TRNS,
            KC_TRNS, US_ADIA, MX_AGRV, KC_TRNS, KC_TRNS, KC_TRNS,                     KC_TRNS, US_NTIL, KC_TRNS, UC_oe  , US_OSTR, KC_TRNS,
            KC_TRNS, US_MICR, KC_TRNS, US_CCED, KC_TRNS, KC_TRNS, KC_TRNS,   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                                                KC_TRNS, KC_TRNS, KC_TRNS,   US_EACU, MX_EGRV, KC_TRNS
        ),
    // alternate character and navigation layer
    // MX_BTIC and MX_TILD here are the 'live' key for programmers.
    [L_ALTGR] = LAYOUT(
            KC_NO  , US_SECT, US_CENT, US_PND , US_EURO, UC_PMIL,                       US_DEG , KC_PIPE, KC_LBRC, KC_RBRC, MX_TILD, KC_DEL ,
            KC_NO  , KC_NO  , KC_PRWD, KC_UP  , KC_NXWD, MX_FUER,                       KC_NO  , KC_BSLS, KC_LCBR, KC_RCBR, MX_BTIC, KC_NO  ,
            KC_LSFT, KC_HOME, KC_LEFT, KC_DOWN, KC_RGHT, KC_END ,                       KC_NO  , KC_NO  , KC_LPRN, KC_RPRN, KC_SCLN, KC_RSFT,
			KC_LCTL, KC_TRNS, KC_TOP , KC_PGUP, KC_PGDN, KC_BOTT, KC_LGUI,     KC_RGUI, US_MUL , KC_EQL , KC_LT  , KC_GT  , UC_NDSH, KC_INS ,
                                                KC_LALT, KC_BSPC, KC_ENT ,     KC_NO  ,KC_SPC ,  KC_RCTL
        ),
    // function layer, like on a laptop.
    [L_FN] = LAYOUT(
            KC_TRNS, KC_F1  , KC_F2  , KC_F3  , KC_F4  , KC_F5  ,                       KC_F6  , KC_F7  , KC_F8  , KC_F9  , KC_F10 , EE_CLR ,
            MX_VERS, KC_F11 , KC_F12 , KC_NO  , KC_NO  , MX_TQM ,                       KC_NO  , RM_TOGG, RM_HUED, RM_SATD, RM_VALD, QK_BOOT,
            OSM_SFT, KC_MPRV, KC_MNXT, KC_NO  , KC_NO  , KC_NO  ,                       KC_NO  , RM_NEXT, RM_HUEU, RM_SATU, RM_VALU, OSM_SFT,
			OSM_CTL, KC_NO, KC_NO, C(KC_PGUP), C(KC_PGDN), KC_NO, KC_LGUI,     KC_RGUI, KC_MSTP, KC_MPLY, KC_VOLD, KC_VOLU, KC_MUTE, KC_NO  ,
                                                 OSM_ALT , OSM_AGR, KC_NO,     KC_NO  , OSM_AGR, OSM_CTL
        )
};

const custom_shift_key_t custom_shift_keys[] = {
  // This replaces the () and <> characters which move to the "stack of parenthesis" on the AltGr layer.
  {KC_9   , KC_SLSH}, // Shift 9 is /
  {KC_0   , KC_QUES}, // Shift 0 is ?
  {KC_DOT , KC_COLN}, // Shift . is :
  {KC_COMM, KC_SCLN}, // Shift , is ;
  // TODO: this doesn't work: it seems to send "arrow up" instead.
  {UC_oe  , UC_OE},   // Shift œ should be Œ
};

// 3 ms still had some dropped letters.
const int SEND_STRING_DELAY_MS = 10;

void toggle_quote_mode(void) {
    switch (current_quote_mode) {
        case QUOTE_MODE_ANSI:
        //     current_quote_mode = QUOTE_MODE_LINUX;
        //     break;
        // case QUOTE_MODE_LINUX:
            current_quote_mode = QUOTE_MODE_SAMSUNG;
            break;
        case QUOTE_MODE_SAMSUNG:
            current_quote_mode = QUOTE_MODE_ANSI;
            break;
    }
    // TODO: flash the LED on letter A, L or S to indicate the new quote mode.
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case MC_WINT:
            // only handle the press event in "tap" mode. (Hold mode is fully handled by QMK.)
            if (record->tap.count && record->event.pressed) {
                tap_code16(RWIN(KC_TAB));
                return false;
            }
            break;
        case MX_FUER:
            if (!record->event.pressed) return false;
            tap_code(KC_F);
            tap_code16(US_UDIA);  // 16 bits, because the keycode has the Shift bit set.
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
                send_string_with_delay("Layout ASDR_NILT standalone, rev15-firmware-accents, ", SEND_STRING_DELAY_MS);
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
