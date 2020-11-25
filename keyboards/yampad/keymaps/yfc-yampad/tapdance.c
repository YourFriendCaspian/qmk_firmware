/**
 * Tap Dance config and functions
 **/
/* Copyright 2020 yourfriendcaspian
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

enum {
    SINGLE_TAP = 1,
    SINGLE_HOLD = 2,
    DOUBLE_TAP = 3,
    TRIPLE_TAP = 4,
    UNKNOWN_TAPS = 5
};

typedef struct {
  bool is_press_action;
  int state;
} tap;

int cur_dance (qk_tap_dance_state_t *state) {
    if (state->count == 1) {
      if (state->interrupted || !state->pressed) {
        return SINGLE_TAP;
      } else {
        return SINGLE_HOLD;
      }
    }
    if (state->count == 2) {
      if (state->interrupted) {
        return DOUBLE_SINGLE_TAP;
      } else if (!state->pressed) {
        return DOUBLE_TAP;
      }
    }
    return UNKNOWN_TAPS;
}

/**
 * Switch Layer - sl
 **/

static tap sl_state = {
  .is_press_action = true,
  .state = 0
};

void sl_finished(qk_tap_dance_state_t *state, void *user_data) {
    quote_state.state = cur_dance(state);
    switch(sl_state.state) {
      case SINGLE_TAP: layer_on(_NV)); break;
      case SINGLE_HOLD: layer_on(_BL); break;
      case DOUBLE_TAP: layer_on(_FN); break;
    }
}
void sl_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch(sl_state.state) {
      case SINGLE_TAP: layer_off(_NV); break;
      case SINGLE_HOLD: layer_off(_BL); break;
      case DOUBLE_TAP: layer_off(_FN); break;
    }
    sl_state.state = 0;
}

/**
 * Define the available tap dance keys
 **/
enum {
  TD_SL = 0
};

qk_tap_dance_action_t tap_dance_actions[] = {
  [TD_SL] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, sl_finished, sl_reset)
};