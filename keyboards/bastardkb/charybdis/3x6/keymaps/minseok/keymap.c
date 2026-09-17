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

// Tap Dance Tap-Hold struct and functions
typedef struct {
    uint16_t tap;
    uint16_t hold;
    uint16_t held;
} tap_dance_tap_hold_t;

void tap_dance_tap_hold_finished(tap_dance_state_t *state, void *user_data) {
    tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)user_data;

    if (state->pressed) {
        if (state->count == 1
#ifndef PERMISSIVE_HOLD
            && !state->interrupted
#endif
        ) {
            register_code16(tap_hold->hold);
            tap_hold->held = tap_hold->hold;
        } else {
            register_code16(tap_hold->tap);
            tap_hold->held = tap_hold->tap;
        }
    }
}

void tap_dance_tap_hold_reset(tap_dance_state_t *state, void *user_data) {
    tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)user_data;

    if (tap_hold->held) {
        unregister_code16(tap_hold->held);
        tap_hold->held = 0;
    }
}

// Tap Dance definitions matching dd.vil (Tap on tap, Shortcut on hold)
static tap_dance_tap_hold_t td_tap_holds[] = {
    [TD_0]  = {KC_N, KC_B, 0},
    [TD_1]  = {KC_Z, LCTL(KC_Z), 0},
    [TD_2]  = {KC_X, LCTL(KC_X), 0},
    [TD_3]  = {KC_C, LCTL(KC_C), 0},
    [TD_4]  = {KC_V, LCTL(KC_V), 0},
    [TD_5]  = {KC_E, LGUI(KC_E), 0},
    [TD_6]  = {KC_RALT, KC_RCTL, 0},
    [TD_7]  = {KC_MINS, KC_EQL, 0},
    [TD_8]  = {KC_BSPC, KC_DEL, 0},
    [TD_9]  = {KC_SLSH, KC_BSLS, 0},
    [TD_10] = {KC_LBRC, KC_RBRC, 0},
};

#define ACTION_TAP_DANCE_TAP_HOLD_IDX(idx) \
    { \
        .fn = {NULL, tap_dance_tap_hold_finished, tap_dance_tap_hold_reset, NULL}, \
        .user_data = (void *)&td_tap_holds[idx], \
    }

tap_dance_action_t tap_dance_actions[] = {
    [TD_0]  = ACTION_TAP_DANCE_TAP_HOLD_IDX(TD_0),
    [TD_1]  = ACTION_TAP_DANCE_TAP_HOLD_IDX(TD_1),
    [TD_2]  = ACTION_TAP_DANCE_TAP_HOLD_IDX(TD_2),
    [TD_3]  = ACTION_TAP_DANCE_TAP_HOLD_IDX(TD_3),
    [TD_4]  = ACTION_TAP_DANCE_TAP_HOLD_IDX(TD_4),
    [TD_5]  = ACTION_TAP_DANCE_TAP_HOLD_IDX(TD_5),
    [TD_6]  = ACTION_TAP_DANCE_TAP_HOLD_IDX(TD_6),
    [TD_7]  = ACTION_TAP_DANCE_TAP_HOLD_IDX(TD_7),
    [TD_8]  = ACTION_TAP_DANCE_TAP_HOLD_IDX(TD_8),
    [TD_9]  = ACTION_TAP_DANCE_TAP_HOLD_IDX(TD_9),
    [TD_10] = ACTION_TAP_DANCE_TAP_HOLD_IDX(TD_10),
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

// 탭댄스 키를 손에서 떼는 순간 0ms 딜레이로 즉시 기본 키(Tap) 입력 전송
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case QK_TAP_DANCE ... QK_TAP_DANCE_MAX: {
            uint16_t idx = QK_TAP_DANCE_GET_INDEX(keycode);
            if (idx < sizeof(td_tap_holds) / sizeof(td_tap_holds[0])) {
                tap_dance_state_t *state = tap_dance_get_state(idx);
                if (!record->event.pressed && state != NULL && state->count && !state->finished) {
                    tap_code16(td_tap_holds[idx].tap);
                }
            }
            break;
        }
    }
    return true;
}

// 3번 레이어(마우스 레이어) 진입 시 자동으로 트랙볼 드래그 스크롤 활성화
layer_state_t layer_state_set_user(layer_state_t state) {
    charybdis_set_pointer_dragscroll_enabled(IS_LAYER_ON_STATE(state, 3));
    return state;
}

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
