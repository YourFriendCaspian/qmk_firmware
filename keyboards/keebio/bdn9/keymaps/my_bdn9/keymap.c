/* Copyright 2019 Danny Nguyen <danny@keeb.io>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H

typedef struct {
    bool is_press_action;
    uint8_t state;
} tap;

enum {
  SINGLE_TAP = 1,
  SINGLE_HOLD,
  DOUBLE_TAP,
  DOUBLE_HOLD,
  DOUBLE_SINGLE_TAP, // Send two single taps
  TRIPLE_TAP,
  TRIPLE_HOLD
};

enum {
    LAYR, // Our custom tap dance key; add any other tap dance keys to this enum 
};

// Declare the functions to be used with your tap dance key(s)

// Function associated with all tap dances
uint8_t cur_dance(qk_tap_dance_state_t *state);

// Functions associated with individual tap dances
void la_finished(qk_tap_dance_state_t *state, void *user_data);
void la_reset(qk_tap_dance_state_t *state, void *user_data);

enum encoder_names {
  _LEFT,
  _RIGHT,
  _MIDDLE,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
        | Knob 1: Vol Dn/Up | Tap 1-3: Layer 1-3 | Knob 2: Page Dn/Up |
        | Press: Play/Pause | Hold: Layer 0      | Press: RGB Mode    |
        | Home              | Up                 | End                |
        | Left              | Down               | Right              |
     */
    [0] = LAYOUT(
        KC_MPLY, TD(LAYR) , RGB_MOD,
        KC_HOME, KC_UP    , KC_END,
        KC_LEFT, KC_DOWN  , KC_RGHT
    ),
    /*
        | Escape | Tap 1-3: Layer 1-3 | RGB Mode   |
        | Tab    | Enter              | Backspace  |
        | Win    | End                | Media Next |
     */
    [1] = LAYOUT(
        KC_ESC  , TD(LAYR) , RGB_MOD   ,
        KC_TAB  , KC_ENTER , KC_BSPACE ,
        KC_RGUI , KC_END   , KC_MNXT
    ),
    /*
        | Escape | Tap 1-3: Layer 1-3 | RGB Mode   |
        | Tab    | Enter              | Backspace  |
        | Win    | End                | Media Next |
     */
    [2] = LAYOUT(
        KC_ESC  , TD(LAYR) , RGB_MOD   ,
        KC_TAB  , KC_ENTER , KC_BSPACE ,
        KC_MPRV , KC_END   , KC_MNXT
    ),
};

void encoder_update_user(uint8_t index, bool clockwise) {
    if (index == _LEFT) {
        if (clockwise) {
            tap_code(KC_VOLU);
        } else {
            tap_code(KC_VOLD);
        }
    }
    else if (index == _MIDDLE) {
        if (clockwise) {
            tap_code(KC_DOWN);
        } else {
            tap_code(KC_UP);
        }
    }
    else if (index == _RIGHT) {
        if (clockwise) {
            tap_code(KC_PGDN);
        } else {
            tap_code(KC_PGUP);
        }
    }
}

// Determine the current tap dance state
uint8_t cur_dance(qk_tap_dance_state_t *state) {
    if (state->count == 1) {
        if (!state->pressed) return SINGLE_TAP;
        else return SINGLE_HOLD;
    } else if (state->count == 2) return DOUBLE_TAP;
    else return 8;
}

// Initialize tap structure associated with example tap dance key
static tap la_tap_state = {
    .is_press_action = true,
    .state = 0
};

// Functions that control what our tap dance key does
void la_finished(qk_tap_dance_state_t *state, void *user_data) {
    la_tap_state.state = cur_dance(state);
    switch (la_tap_state.state) {
        case SINGLE_TAP:
            layer_on(1);
            break;
        case SINGLE_HOLD:
            layer_on(0);
            break;
        case DOUBLE_TAP:
            // Check to see if the layer is already set
            if (layer_state_is(2)) {
                // If already set, then switch it off
                layer_on(2);
            } else {
                // If not already set, then switch the layer on
                layer_on(2);
            }
            break;
        case DOUBLE_HOLD:
            layer_on(0);
            break;
    }
}

void la_reset(qk_tap_dance_state_t *state, void *user_data) {
    // If the key was held down and now is released then switch on the layer
    if (la_tap_state.state == SINGLE_HOLD) {
        layer_on(0);
    }
    la_tap_state.state = 0;
}

// Associate our tap dance key with its functionality
qk_tap_dance_action_t tap_dance_actions[] = {
    [LAYR] = ACTION_TAP_DANCE_FN_ADVANCED_TIME(NULL, la_finished, la_reset, 275)
};
