// The keymap the test harness compiles against: the test keyboard's own (empty) layers, plus the
// real key overrides of the `cozy_de` keymap.
//
// `key_overrides.h` is not checked in here. It is copied in from
// `keyboards/keebio/iris_ce/keymaps/cozy_de/` before the test is built, so that what runs here is
// the same text the firmware compiles, with no mirror to drift out of date. See the README.
#ifndef COZY_DE_KEY_OVERRIDES_TEST_KEYMAP_ONCE
#define COZY_DE_KEY_OVERRIDES_TEST_KEYMAP_ONCE

#include "tests/test_common/keymap.c"
#include "key_overrides.h"

#endif
