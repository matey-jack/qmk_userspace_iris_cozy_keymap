/*
    "The Cozy Keyboard" letter mapping with a mix of US ANSI and DE Qwertz shift mapping to accommodate for
    the reduced number of keys on the Iris keyboard.

    This is the "German OS layout" edition of the `cozy` keymap:
    it assumes that the computer is running the German *extended* keyboard layout E1
    (xkb's `de(e1)`, standardized as DIN 2137-1:2020-11), and it deliberately contains
     - no community modules (the `getreuer/custom_shift_keys` module is replaced by QMK's built-in Key Overrides), and
     - no `process_record_user()` logic beyond what the accented letters actually need:
       printing the firmware version, and the five accent macros at the end of this file.

    E1 is identical to the standard German layout `de(basic)` on levels 1 and 2 (plain and Shift),
    so every unmodified and every shifted key of this keymap types the same on both layouts.
    The two differ on level 3 (AltGr), and that is exactly where E1 puts the dead keys that
    `de(basic)` lacks — which is the reason for targeting E1 in the first place.

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

// Shared with the other keymap, through USER_NAME in rules.mk: the ISO build date
// and the SEND_STRING delay.
#include "cozy_common.h"

#define VERSION_STRING "Cozy-DE, rev03-combine-layer-cleanup"

/*
    Dead keys and characters that xkb's `de(e1)` puts on the AltGr (level 3) level
    but which QMK's `keymap_german.h` does not define.

    The comment after each line names the xkb key code and the xkb keysym it produces; all of them
    can be checked against the `xkb_symbols "e1"` block of `reference/xkb/symbols/de`.

    Caution: E1 leaves level 4 (AltGr+Shift) unassigned on every key used here — the xkb source
    writes `any` there — and moves what `de(basic)` had on level 4 to levels 5 and 6. This means
    that a held Shift turns any ALGR() keycode below into a no-op, which is why the accent macros
    at the end of this file clear the modifiers before sending their dead key.

    Levels 5 and 6 are reached through the Level-5 latch, which E1 puts on AltGr+F and which this
    keymap exposes as DE_LVL5 on L_ALTGR, mirroring the L_COMB key on the other half of the board:
    both are one-shot layers that apply to the next keystroke only, one implemented in the
    firmware, the other in the xkb config. DE_LVL5 is the one keycode here that is *not* expected
    to work on Windows, whose E1 implementation is not known to have an equivalent; everything
    reachable only through it is listed separately further down.
*/
// Dead keys. `de(basic)` has only the acute, grave and circumflex (which E1 keeps in place, so
// `DE_ACUT`, `DE_GRV` and `DE_CIRC` from `keymap_german.h` still apply) plus a diaeresis on
// AltGr+ü. E1 adds the tilde and the cedilla, which is what finally makes ñ and ç typable here.
#define DE_DTIL ALGR(DE_I)        // ~ dead tilde      <AD08> level 3: dead_tilde
#define DE_DCED ALGR(DE_J)        // ¸ dead cedilla    <AC07> level 3: dead_cedilla
#define DE_DDIA ALGR(DE_Z)        // ¨ dead diaeresis  <AD06> level 3: dead_diaeresis  (moved from AltGr+ü)
#define DE_DSTR ALGR(DE_ADIA)     // / dead stroke     <AC11> level 3: dead_stroke  (ø, đ, ł, ...)

// The Level-5 latch, E1's own version of AltGr, for everything on levels 5 and 6.
// Not a dead key in the xkb sense, but it behaves like one: it modifies the next keystroke.
#define DE_LVL5 ALGR(DE_F)        // ⇮ level-5 latch  <AC04> level 3: ISO_Level5_Latch

// Ordinary characters that E1 places differently from `de(basic)`.
#define DE_IEXL ALGR(DE_5)        // ¡ inverted !      <AE05> level 3: exclamdown
#define DE_IQUE ALGR(DE_6)        // ¿ inverted ?      <AE06> level 3: questiondown
#define DE_MUL  ALGR(DE_CIRC)     // × multiply sign   <TLDE> level 3: multiply
#define DE_NDSH ALGR(DE_N)        // – n-dash          <AB06> level 3: endash

/*
    Characters of the `cozy` keymap that are not a single keycode on `de(e1)` and are therefore
    absent from the layers below. Sorted by how they are typed now.

    All of them sit on E1's level 5, reached by tapping DE_LVL5 (the Level-5 latch, on L_ALTGR)
    and then the key named here. Since L_ALTGR is a held layer, that means releasing the layer key
    between the two keystrokes, so that the second one comes from the base layer. This is the part
    of the keymap expected to be Linux-only: the Windows E1 implementation is not known to have a
    Level-5 latch, and nothing else here depends on it.

    DE_LVL5, then:
     - 1, 2 or 3  – US_QRTR ¼, US_HALF ½, US_TQTR ¾.  `de(basic)` inherited ¼ and ½ from
                    `latin(basic)` on level 3; E1 moves all three fractions to level 5.
     - 5          – UC_PMIL ‰.  E1 does have the per mille sign; `de(basic)` did not.
     - ä          – US_AE æ.  `de(basic)` had it on AltGr+A, which in E1 is the Compose key.
     - ö          – UC_oe œ, which `cozy` typed via QMK Unicode input.
     - o          – US_OSTR ø.  DE_DSTR (row 1 of L_COMBINE) then o does the same, works on
                    Windows too, and additionally gives đ and ł.  `de(basic)` had ø on AltGr+O,
                    which in E1 is dead_abovering.

    Not a character, and gone for good: MX_TQM, the ANSI/Windows "quote mode" switch, disappeared
    together with the custom code it needed — the OS layout now decides what the accent keys do.

    MX_TABA, the "chameleon" Tab/ä key, keeps its behavior without custom code: it is a plain
    DE_ADIA (ä/Ä), and the ko_adia_tab key override further down makes it a Tab whenever Ctrl, Alt
    or Gui is held. An unmodified Tab is on the L_ALTGR layer, as in `cozy`.
*/

enum layer_names {
    L_BASE,
    L_COMBINE,
    L_ALTGR,
    L_FN,
};
// The AltGr does almost everything that the AltGr level does in the software layout,
// therefore we don't need the AltGr modifier on the base layer. (But there's one on the Fn layer.)

/*
    Custom keycodes: printing the firmware version, and the two-keystroke sequences that are
    worth collapsing into a single keypress.

    Five of them are the accented letters of section 2 of issue #8, minuscules only. Their
    capitals are rare enough to be typed the explicit way, with the dead accent keys on row 1 of
    the L_COMBINE layer followed by a shifted letter, so these macros never deal with Shift
    at all — see send_prefixed_key() below.

    MX_HAT and MX_BTIC are the *live* (non-combining) ^ and `, which the German layout has no key
    for — both are dead keys there, and a dead key followed by a space yields its spacing form.

    MX_CENT and MX_PND are the two currency signs that E1 keeps behind the Level-5 latch, so they
    are Linux-only in the same way the latch is. £ needs the macro most: its second keystroke is
    the ´ key, which is not on the base layer, so typing it by hand would mean re-entering a layer
    in the middle of the sequence. ¢ simply has a free spot waiting for it.

    All of them sit where the `cozy` keymap puts them.
*/
enum custom_keycodes {
    MX_VERS = SAFE_RANGE, // ugly hack: prints the firmware version.

    MX_EACU,  // é   dead acute      + e
    MX_EGRV,  // è   dead grave      + e
    MX_AGRV,  // à   dead grave      + a
    MX_NTIL,  // ñ   dead tilde      + n   (needs E1; `de(basic)` has no dead tilde)
    MX_CCED,  // ç   dead cedilla    + c   (needs E1; `de(basic)` hides the cedilla on AltGr+´)
    MX_HAT ,  // ^   dead circumflex + space   (the live, non-combining version)
    MX_BTIC,  // `   dead grave      + space   (likewise)
    MX_CENT,  // ¢   level-5 latch   + c   (Linux only, like everything behind the latch)
    MX_PND ,  // £   level-5 latch   + ´   (likewise)
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
// The expose / task view key for Windows: a plain Win+Tab, no hold function.
// The Gui modifier itself stays on the left thumb key (for the start menu in Windows and Expose in Ubuntu).
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
    // Extra letter layer, rarely used, since äöü are on the base layer and ß is on Shift+6.
    // accented letters: ß once more (on the S key), and the five macros é è à ñ ç
    //                   (minuscules only, see the enum above). äöü are not repeated here:
    //                   an unshifted key of the base layer is never worth a second position.
    // combining accents: all seven that E1 offers, side by side on the 4…0 keys of row 1 —
    //                    the cedilla and the stroke on the left half, where the ¼½¾¥ used to be,
    //                    and ^ ¨ ´ ` ~ on the right half (as in `cozy`).
    //                    Any accent-letter pair that has no macro is typed as accent + letter,
    //                    and that is also how the capitals É È À Ñ Ç are made.
    // other: § on the 3 key and µ on the M key — both the keys the standard German layout puts
    //        them on: <AE03> level 2 (Shift+3) and <AB07> level 3 (AltGr+M), reached unmodified here.
    [L_COMBINE] = LAYOUT(
            TO(0)  , KC_NO  , KC_NO  , DE_SECT, DE_DCED, DE_DSTR,                     DE_CIRC, DE_DDIA, DE_ACUT, DE_GRV , DE_DTIL, TO(0)  ,
            KC_NO  , KC_NO  , KC_NO  , KC_NO  , KC_NO  , TO(0)  ,                     KC_NO  , KC_NO  , KC_NO  , KC_NO  , KC_NO  , KC_NO  ,
            KC_NO  , MX_AGRV, DE_SS  , KC_NO  , KC_NO  , KC_NO  ,                     KC_NO  , MX_NTIL, KC_NO  , KC_NO  , KC_NO  , KC_NO  ,
            KC_NO  , KC_NO  , KC_NO  , MX_CCED, KC_NO  , KC_NO  , KC_NO  ,   KC_NO  , KC_NO  , DE_MICR, KC_NO  , KC_NO  , KC_NO  , KC_NO  ,
                                                KC_NO  , KC_NO  , KC_NO  ,   MX_EACU, MX_EGRV, KC_NO
        ),
    // Alternate character and navigation layer.
    // DE_TILD here is the 'live' (non-combining) tilde, as used in programming languages, among others.
    // Its siblings ^ and ` are dead keys in the German layout, so MX_HAT and MX_BTIC add the space
    // that turns them into the live characters. ¢ and £ are macros too, see the enum above.
    [L_ALTGR] = LAYOUT(
            KC_NO  , DE_IEXL, MX_CENT, MX_PND , DE_EURO, KC_NO  ,                       MX_HAT , DE_PIPE, DE_LBRC, DE_RBRC, DE_IQUE, KC_DEL ,
            KC_TAB ,S(KC_TAB),KC_PRWD, KC_UP  , KC_NXWD, L_COMB ,                       DE_LVL5, DE_BSLS, DE_LCBR, DE_RCBR, DE_TILD, DE_DEG ,
            KC_LSFT, KC_HOME, KC_LEFT, KC_DOWN, KC_RGHT, KC_END ,                       MX_BTIC, DE_SLSH, DE_LPRN, DE_RPRN, DE_SCLN, KC_RSFT,
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
    They live in their own header so that the host-side test in `tests/cozy_de_key_overrides/`
    compiles the same definitions this firmware does; see the comments there.
*/
#include "key_overrides.h"

/*
    The two-keystroke sequences behind the custom keycodes above: a prefix key — a dead accent,
    or E1's Level-5 latch — followed by the key that the prefix applies to.

    The accented letters here are minuscules. Capitals are out of scope on purpose: they are rare
    enough to be typed as an explicit dead accent from row 1 of L_COMBINE plus a shifted letter,
    and leaving them out keeps this table a plain list of two keycodes per entry. The same two
    columns describe the live ^ and ` (a dead key plus a space) and the two level-5 currency
    signs (the latch plus a key), so all of them share one sender.
*/
typedef struct {
    uint16_t keycode;  // the custom keycode, as placed in the layers above
    uint16_t prefix;   // the de(e1) dead key or Level-5 latch that opens the sequence
    uint16_t base;     // the key that the prefix is applied to
} prefixed_key_t;

static const prefixed_key_t prefixed_keys[] = {
    {MX_EACU, DE_ACUT, DE_E   },  // é
    {MX_EGRV, DE_GRV , DE_E   },  // è
    {MX_AGRV, DE_GRV , DE_A   },  // à
    {MX_NTIL, DE_DTIL, DE_N   },  // ñ
    {MX_CCED, DE_DCED, DE_C   },  // ç
    {MX_HAT , DE_CIRC, KC_SPC },  // ^  — dead key plus space gives the spacing character
    {MX_BTIC, DE_GRV , KC_SPC },  // `
    {MX_CENT, DE_LVL5, DE_C   },  // ¢
    {MX_PND , DE_LVL5, DE_ACUT},  // £
};

/*
    Send one entry of the table above, with every modifier taken out of the way first.

    Clearing the modifiers is not just about the (out of scope) capitals — it is what makes the
    macros produce the right character at all. With a Shift still held,
     - DE_ACUT would become DE_GRV, because ´ and ` share one key in the German layout, so é would
       silently come out as è, and
     - DE_DTIL, DE_DCED and DE_LVL5 would land on E1's unassigned level 4 (AltGr+Shift) and
       produce nothing at all.
    tap_code16() only ever *adds* the modifiers encoded in its keycode; it never removes a
    modifier that is physically held, so the clearing has to happen here.

    Three modifier registers have to be dealt with: the real modifiers (a held Shift), the weak
    ones (where Caps Word parks its Shift) and the one-shot ones (OS_LSFT on the L_FN layer).
    Real and weak modifiers are restored afterwards; a one-shot modifier is not, because it was
    spent on this keypress, exactly as it would have been on any ordinary key.

    Note that this does stop Caps Word, since QMK's default caps_word_press_user() deactivates on
    any unknown keycode. That is the correct behavior here: Caps Word wants capitals, and these
    macros deliberately only make minuscules.
*/
static void send_prefixed_key(const prefixed_key_t *sequence) {
    const uint8_t real_mods = get_mods();
    const uint8_t weak_mods = get_weak_mods();

    clear_mods();
    clear_weak_mods();
    clear_oneshot_mods();
    send_keyboard_report();

    // The same delay that SEND_STRING needs below: dead key sequences get dropped when typed too fast.
    tap_code16_delay(sequence->prefix, SEND_STRING_DELAY_MS);
    tap_code16_delay(sequence->base, SEND_STRING_DELAY_MS);

    set_mods(real_mods);
    set_weak_mods(weak_mods);
    send_keyboard_report();
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    for (uint8_t i = 0; i < ARRAY_SIZE(prefixed_keys); i++) {
        if (keycode == prefixed_keys[i].keycode) {
            if (record->event.pressed) {
                send_prefixed_key(&prefixed_keys[i]);
            }
            return false;
        }
    }
    switch (keycode) {
        case MX_VERS:
            if (record->event.pressed) {
                send_string_with_delay(VERSION_STRING, SEND_STRING_DELAY_MS);
                send_string_with_delay(", ", SEND_STRING_DELAY_MS);
                send_string_with_delay(BUILD_DATE, SEND_STRING_DELAY_MS);
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
    // Not println(): that macro pastes its argument onto "\r\n" at compile time, so it takes a
    // string literal only, and BUILD_DATE is an array.
    xprintf("%s\r\n", BUILD_DATE);
    //debug_matrix=true;
    //debug_keyboard=true;
    //debug_mouse=true;
}
