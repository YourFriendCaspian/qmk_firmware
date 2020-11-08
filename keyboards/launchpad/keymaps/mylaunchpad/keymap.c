// Below layout is based upon /u/That-Canadian's planck layout
#include QMK_KEYBOARD_H


typedef struct {
    bool is_press_action;
    uint8_t state;
} tap;

// Define a type for as many tap dance states as you need
enum {
    SINGLE_TAP = 1,
    SINGLE_HOLD,
    DOUBLE_TAP,
    TRIPLE_TAP
};

enum {
    ENT_LAYR, // Our custom tap dance key; add any other tap dance keys to this enum 
};

// Declare the functions to be used with your tap dance key(s)

// Function associated with all tap dances
uint8_t cur_dance(qk_tap_dance_state_t *state);

// Functions associated with individual tap dances
void el_finished(qk_tap_dance_state_t *state, void *user_data);
void el_reset(qk_tap_dance_state_t *state, void *user_data);


// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.

enum layer_names {
    _ARROW,
    _RGB,
    _FUNC,
    _LAYER3
};

// Defines for task manager and such
#define CALTDEL LCTL(LALT(KC_DEL))
#define TSKMGR LCTL(LSFT(KC_ESC))

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* Arrow
 * ,------------------------------.
 * |  LEFT           |  ESCAPE    |
 * |-----------------+------------|
 * |  DOWN           |  UP        |
 * |-----------------+------------|
 * |  RIGHT          |  TAB       |
 * |-----------------+------------|
 * |  TD(ENTER_LAYER)|  BACKSPACE  |
 * `------------------------------.
 */
[_QWERTY] = LAYOUT( \
    KC_LEFT      ,  KC_ESC    ,
    KC_DOWN      ,  KC_UP     ,
    KC_RIGHT     ,  KC_TAB    ,
    TD(ENT_LAYR) ,  KC_BSPACE
),

/* RGB
 * ,----------------.
 * | MODE- | MODE+  |
 * |-------+--------|
 * | HUE-  | HUE+   |
 * |-------+--------|
 * | SAT-  | SAT+   |
 * |-------+--------|
 * |       | TOGGLE |
 * `----------------'
 */
[_RGB] = LAYOUT(
    RGB_RMOD ,  RGB_MOD ,
    RGB_HUD  ,  RGB_HUI ,
    RGB_SAD  ,  RGB_SAI ,
    KC_TRNS  ,  RGB_TOG
),

/* Function
 * ,-------------.
 * |   Q  |CALDEL|
 * |------+------|
 * |   A  |TSKMGR|
 * |------+------|
 * |   Z  |  X   |
 * |------+------|
 * |      |  C   |
 * `-------------'
 */
[_FUNC] = LAYOUT( \AA
    KC_Q,    CALTDEL, \
    KC_A,    TSKMGR, \
    KC_Z,    KC_X, \
    KC_TRNS, KC_C \
),

/* Layer 3
 * ,-------------.
 * |      |      |
 * |------+------|
 * |      |      |
 * |------+------|
 * |      |      |
 * |------+------|
 * |      |      |
 * `-------------'
 */
[_LAYER3] = LAYOUT(
    KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS
)

};

// Determine the current tap dance state
uint8_t cur_dance(qk_tap_dance_state_t *state) {
    if (state->count == 1) {
        if (!state->pressed) return SINGLE_TAP;
        else return SINGLE_HOLD;
    } else if (state->count == 2) return DOUBLE_TAP;
    else return 8;
}

// Initialize tap structure associated with example tap dance key
static tap el_tap_state = {
    .is_press_action = true,
    .state = 0
};

// Functions that control what our tap dance key does
void el_finished(qk_tap_dance_state_t *state, void *user_data) {
    el_tap_state.state = cur_dance(state);
    switch (el_tap_state.state) {
        case SINGLE_TAP:
            tap_code(KC_ENT);
            break;
        case SINGLE_HOLD:
            layer_on(_ARROW);
            break;
        case DOUBLE_TAP:
            // Check to see if the layer is already set
            if (layer_state_is(_RGB)) {
                // If already set, then switch it off
                layer_off(_RGB);
            } else {
                // If not already set, then switch the layer on
                layer_on(_RGB);
            }
            break;
        case TRIPLE_TAP:
            if (layer_state_is(_FUNC)) { 
              layer_off(_FUNC); }
              else {
              layer_on(_FUNC); }
            break;
    }
}

void el_reset(qk_tap_dance_state_t *state, void *user_data) {
    // If the key was held down and now is released then switch off the layer
    if (el_tap_state.state == SINGLE_HOLD) {
        layer_off(_RGB);
    }
    el_tap_state.state = 0;
}

// Associate our tap dance key with its functionality
qk_tap_dance_action_t tap_dance_actions[] = {
    [ENT_LAYR] = ACTION_TAP_DANCE_FN_ADVANCED_TIME(NULL, el_finished, el_reset, 275)
};

// void matrix_init_user(void) {
//
// }
