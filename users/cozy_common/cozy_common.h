// Copyright 2025 Robert Jack Will
// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

/*
    What the `cozy` and `cozy_de` keymaps both need, in one place so that a fix lands in both
    at once rather than in whichever one was being edited at the time.

    Both keymaps reach this directory by setting `USER_NAME = cozy_common` in their `rules.mk`.
    QMK defaults that to the keymap's own name, which would give two separate directories; the
    override points both at this one, and `build_keyboard.mk` then puts it on the include path,
    which is why the include below needs no path.

    Only build plumbing belongs here. Everything a reader would compare between the two keymaps
    — the layers, the key positions, the keycodes — stays in its own `keymap.c`, which is the
    point of having two keymaps at all.
*/

// Written by `qmk generate-version-h` at the start of every build.
#include "version.h"

/*
    The build date in ISO 8601 order, as in 2026-09-21.

    `__DATE__` cannot give that. The C standard fixes its form as "Mmm dd yyyy" ("Sep 21 2026",
    with a leading space instead of a zero on the days 1 to 9), and the preprocessor cannot
    reorder the characters of a string literal, so no amount of macro work turns it into a date
    that sorts. QMK's generated `version.h` already has one: QMK_BUILDDATE is
    "YYYY-MM-DD-hh:mm:ss", whose first ten characters are exactly the date wanted here.

    Copying those ten out by hand is what makes this a compile-time constant: the compiler folds
    each subscript of the literal away, so nothing of the build time is computed on the keyboard.
    The static assert guards against a QMK_BUILDDATE too short to hold a date, which would
    otherwise read past the end of the literal.
*/
_Static_assert(sizeof(QMK_BUILDDATE) >= sizeof("YYYY-MM-DD"), "QMK_BUILDDATE is too short to hold a date");
static const char BUILD_DATE[] = {
    QMK_BUILDDATE[0], QMK_BUILDDATE[1], QMK_BUILDDATE[2], QMK_BUILDDATE[3], // YYYY
    QMK_BUILDDATE[4],                                                       // -
    QMK_BUILDDATE[5], QMK_BUILDDATE[6],                                     // MM
    QMK_BUILDDATE[7],                                                       // -
    QMK_BUILDDATE[8], QMK_BUILDDATE[9],                                     // DD
    '\0'
};

// The pause between the keystrokes that SEND_STRING() and the accent macros send:
// 3 ms still had some dropped letters.
static const int SEND_STRING_DELAY_MS = 10;
