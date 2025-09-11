#define BOTH_SHIFTS_TURNS_ON_CAPS_WORD

// This value is much higher than many of my actual hold-applications.
// But that should be fine, since those are handled by the "permissive hold" feature, not the actual timer.
// On the other hand, the high value avoids misreadings of a key roll (overlapping typing of "ys", for example) as a hold.
// The only reason to have this smaller than infinite is because sometimes the hold is used together with the mouse.
// (Ctrl+Scroll for zoom and Ctrl+Click.)
#define TAPPING_TERM 500

// Switch to Hold mode before tapping term, when other key is pressed _and_ released,
// all before the tap/hold key is released.
#define PERMISSIVE_HOLD

#define RGB_MATRIX_TIMEOUT (5*60*1000) // number of milliseconds to wait until rgb automatically turns off
#define RGB_MATRIX_SLEEP               // turn off effects when suspended

#define UNICODE_SELECTED_MODES      UNICODE_MODE_WINDOWS
