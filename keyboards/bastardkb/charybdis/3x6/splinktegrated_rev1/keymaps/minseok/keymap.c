/**
 * Charybdis Mini (3x6) Custom Keymap
 * Generated from dd.vil with Layer-dependent RGB Matrix Indicators
 * 
 * Layer Colors:
 *  - Layer 1: Red (빨강)
 *  - Layer 2: Blue (파랑)
 *  - Layer 3: Purple (보라)
 */

#include QMK_KEYBOARD_H

// Tap Dance indexes
enum tap_dance_indexes {
    TD_0 = 0,
    TD_1,
    TD_2,
    TD_3,
    TD_4,
    TD_5,
    TD_6,
    TD_7,
    TD_8,
    TD_9,
    TD_10,
};

// Tap Dance definitions matching dd.vil
tap_dance_action_t tap_dance_actions[] = {
    [TD_0]  = ACTION_TAP_DANCE_DOUBLE(KC_N, KC_B),
    [TD_1]  = ACTION_TAP_DANCE_DOUBLE(KC_Z, LCTL(KC_Z)),
    [TD_2]  = ACTION_TAP_DANCE_DOUBLE(KC_X, LCTL(KC_X)),
    [TD_3]  = ACTION_TAP_DANCE_DOUBLE(KC_C, LCTL(KC_C)),
    [TD_4]  = ACTION_TAP_DANCE_DOUBLE(KC_V, LCTL(KC_V)),
    [TD_5]  = ACTION_TAP_DANCE_DOUBLE(KC_E, LGUI(KC_E)),
    [TD_6]  = ACTION_TAP_DANCE_DOUBLE(KC_RALT, KC_RCTL),
    [TD_7]  = ACTION_TAP_DANCE_DOUBLE(KC_MINS, KC_EQL),
    [TD_8]  = ACTION_TAP_DANCE_DOUBLE(KC_BSPC, KC_DEL),
    [TD_9]  = ACTION_TAP_DANCE_DOUBLE(KC_SLSH, KC_BSLS),
    [TD_10] = ACTION_TAP_DANCE_DOUBLE(KC_LBRC, KC_RBRC),
};

// Keymaps
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT(
  // Row 0
       KC_TAB, KC_Q, KC_W, TD(TD_5), KC_R, KC_T,    KC_Y, KC_U, KC_I, KC_O, KC_P, TD(TD_10),
  // Row 1
       KC_LCTL, KC_A, KC_S, KC_D, LT(1, KC_F), KC_G,    KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOTE,
  // Row 2
       KC_LALT, TD(TD_1), TD(TD_2), TD(TD_3), TD(TD_4), KC_B,    KC_N, KC_M, KC_COMMA, KC_DOT, TD(TD_9), TD(TD_6),
  // Thumb keys (Left: 3, Right: 2)
                                  KC_LSFT, KC_ENTER, MO(3),    TD(TD_8), LT(2, KC_SPC)
  ),

  [1] = LAYOUT(
  // Row 0
       KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6,    KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12,
  // Row 1
       _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    KC_HOME, XXXXXXX, KC_UP, XXXXXXX, KC_PGUP, KC_PSCR,
  // Row 2
       _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    KC_END, KC_LEFT, KC_DOWN, KC_RIGHT, KC_PGDN, XXXXXXX,
  // Thumb keys (Left: 3, Right: 2)
                                  _______, _______, _______,    _______, _______
  ),

  [2] = LAYOUT(
  // Row 0
       QK_GRAVE_ESCAPE, KC_1, KC_2, KC_3, KC_4, KC_5,    KC_6, KC_7, KC_8, KC_9, KC_0, TD(TD_7),
  // Row 1
       _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, KC_KP_4, KC_KP_5, KC_KP_6, XXXXXXX, XXXXXXX,
  // Row 2
       _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    KC_KP_0, KC_KP_1, KC_KP_2, KC_KP_3, KC_KP_DOT, KC_NUM_LOCK,
  // Thumb keys (Left: 3, Right: 2)
                                  _______, _______, _______,    _______, _______
  ),

  [3] = LAYOUT(
  // Row 0
       _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // Row 1
       _______, XXXXXXX, XXXXXXX, DRGSCRL, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // Row 2
       _______, XXXXXXX, MS_BTN1, MS_BTN3, MS_BTN2, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // Thumb keys (Left: 3, Right: 2)
                                  _______, _______, _______,    _______, _______
  )
};

// Layer-dependent RGB Matrix Indicators
// Layer 1: Red (255, 0, 0)
// Layer 2: Blue (0, 0, 255)
// Layer 3: Purple (180, 0, 255)
bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    uint8_t current_layer = get_highest_layer(layer_state);

    switch (current_layer) {
        case 1: // 1번 레이어: 빨강 (Red)
            for (uint8_t i = led_min; i < led_max; i++) {
                rgb_matrix_set_color(i, 255, 0, 0);
            }
            break;

        case 2: // 2번 레이어: 파랑 (Blue)
            for (uint8_t i = led_min; i < led_max; i++) {
                rgb_matrix_set_color(i, 0, 0, 255);
            }
            break;

        case 3: // 3번 레이어: 보라 (Purple)
            for (uint8_t i = led_min; i < led_max; i++) {
                rgb_matrix_set_color(i, 180, 0, 255);
            }
            break;

        default:
            // 0번 기본 레이어는 설정된 기본 효과/색상 유지
            break;
    }

    return false;
}
