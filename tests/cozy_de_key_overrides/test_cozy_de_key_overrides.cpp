// Regression test for issue #14: an override whose replacement carries a Shift must keep that
// Shift on the side the user actually pressed, or the modifier byte changes in the same report
// as the replacement key and the host may type the unshifted character.
#include "test_common.hpp"

using testing::_;
using testing::AnyNumber;
using testing::AtLeast;

#define DE_QUOT S(KC_NUHS)

struct Pair {
    const char *name;
    uint16_t    trigger;   // key on the base layer
    uint16_t    expected;  // basic keycode the host must see
};

class ShiftPair : public TestFixture, public ::testing::WithParamInterface<Pair> {
   public:
    // Hold one Shift, tap the trigger. The report must carry the replacement key together with
    // that same Shift, and must never carry it unshifted.
    void check(uint16_t shift_kc, const Pair &p) {
        TestDriver driver;
        KeymapKey  shift = KeymapKey{0, 0, 0, shift_kc};
        KeymapKey  trig  = KeymapKey{0, 1, 0, p.trigger};
        set_keymap({shift, trig});

        EXPECT_ANY_REPORT(driver).Times(AnyNumber());
        EXPECT_REPORT(driver, (shift_kc, p.expected)).Times(AtLeast(1)); // right char, Shift side kept
        EXPECT_REPORT(driver, (p.expected)).Times(0);                    // never unshifted

        shift.press(); run_one_scan_loop();
        trig.press();  run_one_scan_loop();
        trig.release();  run_one_scan_loop();
        shift.release(); run_one_scan_loop();
        testing::Mock::AllowLeak(&driver);
    }
};

TEST_P(ShiftPair, LeftShift)  { check(KC_LSFT, GetParam()); }
TEST_P(ShiftPair, RightShift) { check(KC_RSFT, GetParam()); }

INSTANTIATE_TEST_SUITE_P(CozyDe, ShiftPair,
    ::testing::Values(
        Pair{"dquo", DE_QUOT, KC_2},     // " is Shift+2 on the German layout
        Pair{"ampr", KC_7,    KC_6},     // & is Shift+6
        Pair{"astr", KC_8,    KC_RBRC},  // * is Shift++
        Pair{"ques", KC_0,    KC_MINS}   // ? is Shift+ss
    ),
    [](const testing::TestParamInfo<Pair> &i) { return i.param.name; });
