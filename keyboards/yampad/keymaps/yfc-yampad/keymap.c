#include QMK_KEYBOARD_H
extern keymap_config_t keymap_config;

enum layers {
  _BL = 0,
  _NV,
  _FN
};

/* This include relies on the layer constants above and so must be declared after them */
#include "tapdance.c"

enum custom_keycodes {
  KC_DBL0 = SAFE_RANGE,
};

 
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* Keymap _BL: (Base Layer) Default Layer
 * ,-------------------.
 * | SL | /  | *  | -  |
 * |----|----|----|----|
 * | 7  | 8  | 9  |    |
 * |----|----|----| +  |
 * | 4  | 5  | 6  |    |
 * |----|----|----|----|
 * | 1  | 2  | 3  |    |
 * |----|----|----| En |
 * | 0  | 00 | .  |    |
 * `-------------------'
 */
  [_BL] = LAYOUT(
    TD(SL),   KC_PSLS, KC_PAST, KC_PMNS, \
    KC_P7 ,   KC_P8  , KC_P9  ,          \
    KC_P4 ,   KC_P5  , KC_P6  , KC_PPLS, \
    KC_P1 ,   KC_P2  , KC_P3  ,          \
    KC_P0 ,   KC_DBL0, KC_PDOT, KC_PENT
  ),

/* Keymap _NV: Navigation layer
 * ,-------------------------.
 * |  SL  | DEL  |BSPACE |    |
 * |------|------|-------|----|
 * |      |      |       |    |
 * |------|------|-------|    |
 * | END  | INS  | HOME  |    |
 * |------|------|-------|----|
 * | PGDN |  UP  | PGUP  |    |
 * |------|------|-------|    |
 * | LEFT | DOWN | RIGHT |    |
 * `--------------------------'
 */
  [_NV] = LAYOUT(
    _______,  KC_DEL , KC_BSPACE, XXXXXXX , \
    XXXXXXX,  XXXXXXX, XXXXXXX  ,          \
    KC_END ,  KC_INS , KC_HOME  , KC_SPACE, \
    KC_PGDN,  KC_UP  , KC_PGUP  ,          \
    KC_LEFT,  KC_DOWN, KC_RGHT  , _______
  ),

/* Keymap _FN: RGB Function Layer
 * ,--------------------------.
 * | TD  | RMOD | RGBP | RTOG |
 * |-----|------|------|------|
 * | HUD | HUI  |      |      |
 * |-----|------|------|      |
 * | SAD | SAI  |      |      |
 * |-----|------|------|------|
 * | VAD | VAS  |      |      |
 * |-----|------|------|      |
 * | RST |      |      |      |
 * `--------------------------'
 */
  [_FN] = LAYOUT(
    _______,  RGB_MOD,  RGB_M_P,   RGB_TOG, \
    RGB_HUD,  RGB_HUI,  XXXXXXX,            \
    RGB_SAD,  RGB_SAI,  XXXXXXX,   XXXXXXX, \
    RGB_VAD,  RGB_VAI,  XXXXXXX,            \
    RESET,    XXXXXXX,  XXXXXXX,   _______
  ),
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case KC_DBL0:
      if (record->event.pressed) {
        SEND_STRING("00");
      } else {
        // when keycode KC_DBL0 is released
      }
      break;

  }
  return true;
};

#ifdef OLED_DRIVER_ENABLE
extern rgblight_config_t rgblight_config;

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_270;  // flips the display 270 degrees
}

void oled_task_user(void) {
  // Host Keyboard Layer Status
  oled_write_P(PSTR("Layer"), false);
  switch (biton32(layer_state)) {
    case _BL:
      oled_write_ln_P(PSTR(" BAS"), false);
      break;
    case _NV:
      oled_write_ln_P(PSTR(" NAV"), false);
      break;
    case _FN:
      oled_write_ln_P(PSTR(" RGB"), false);
      break;
    default:
      // Or use the write_ln shortcut over adding '\n' to the end of your string
      oled_write_ln_P(PSTR(" UND"), false);
  }

  // Host Keyboard LED Status
  uint8_t led_usb_state = host_keyboard_leds();
  oled_write_P(PSTR("-----"), false);
  oled_write_P(PSTR("Stats"), false);
  oled_write_P(led_usb_state & (1<<USB_LED_NUM_LOCK) ? PSTR("num:*") : PSTR("num:."), false);
  oled_write_P(led_usb_state & (1<<USB_LED_CAPS_LOCK) ? PSTR("cap:*") : PSTR("cap:."), false);
  oled_write_P(led_usb_state & (1<<USB_LED_SCROLL_LOCK) ? PSTR("scr:*") : PSTR("scr:."), false);

  // Host Keyboard RGB backlight status
  oled_write_P(PSTR("-----"), false);
  oled_write_P(PSTR("Light"), false);

  static char led_buf[30];
  snprintf(led_buf, sizeof(led_buf) - 1, "RGB:%cM: %2d\nh: %2ds: %2dv: %2d\n",
      rgblight_config.enable ? '*' : '.', (uint8_t)rgblight_config.mode,
      (uint8_t)(rgblight_config.hue / RGBLIGHT_HUE_STEP),
      (uint8_t)(rgblight_config.sat / RGBLIGHT_SAT_STEP),
      (uint8_t)(rgblight_config.val / RGBLIGHT_VAL_STEP));
  oled_write(led_buf, false);
}
#endif
