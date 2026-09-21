CONSOLE_ENABLE=yes
CAPS_WORD_ENABLE = yes
UNICODE_ENABLE = yes

RGB_MATRIX_ENABLE = yes        # per-key RGB

# this messes with static keymap updates (from keymap.c)
# VIA_ENABLE = yes

# Both keymaps share users/cozy_common (the ISO build date, the SEND_STRING delay). QMK would
# default USER_NAME to the keymap name and give each one its own directory, so name it here.
USER_NAME = cozy_common
