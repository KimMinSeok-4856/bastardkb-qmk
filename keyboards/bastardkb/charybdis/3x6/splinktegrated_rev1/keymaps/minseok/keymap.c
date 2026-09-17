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
#include "raw_hid.h"
#include "via.h"
#include "pointing_device_auto_mouse.h"
#include "charybdis.h"

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

// Charybdis WebHID Custom Configuration Struct (Extended for Tap Dance)
#define CHARYBDIS_CONFIG_MAGIC 0xCB
#define CHARYBDIS_CONFIG_VERSION 2
#define NUM_TD_SLOTS 11

typedef struct {
    uint16_t tap;
    uint16_t hold;
} td_config_slot_t;

typedef struct {
    uint8_t magic;
    uint8_t version;
    uint16_t default_dpi;
    uint16_t sniping_dpi;
    uint8_t auto_mouse_en;
    uint8_t auto_mouse_layer;
    uint16_t auto_mouse_time;
    uint8_t dragscroll_buffer;
    uint8_t dragscroll_rev_y;
    uint8_t layer1_r;
    uint8_t layer1_g;
    uint8_t layer1_b;
    uint8_t layer2_r;
    uint8_t layer2_g;
    uint8_t layer2_b;
    uint8_t layer3_r;
    uint8_t layer3_g;
    uint8_t layer3_b;
    uint8_t reserved[12];
    td_config_slot_t td_slots[NUM_TD_SLOTS];
} charybdis_user_config_t;

static charybdis_user_config_t g_user_config = {
    .magic = CHARYBDIS_CONFIG_MAGIC,
    .version = CHARYBDIS_CONFIG_VERSION,
    .default_dpi = 800,
    .sniping_dpi = 200,
    .auto_mouse_en = 1,
    .auto_mouse_layer = 3,
    .auto_mouse_time = 650,
    .dragscroll_buffer = 20,
    .dragscroll_rev_y = 1,
    .layer1_r = 255, .layer1_g = 0, .layer1_b = 0,
    .layer2_r = 0, .layer2_g = 0, .layer2_b = 255,
    .layer3_r = 180, .layer3_g = 0, .layer3_b = 255,
    .td_slots = {
        [0]  = {KC_N, KC_B},
        [1]  = {KC_Z, LCTL(KC_Z)},
        [2]  = {KC_X, LCTL(KC_X)},
        [3]  = {KC_C, LCTL(KC_C)},
        [4]  = {KC_V, LCTL(KC_V)},
        [5]  = {KC_E, LGUI(KC_E)},
        [6]  = {KC_RALT, KC_RCTL},
        [7]  = {KC_MINS, KC_EQL},
        [8]  = {KC_BSPC, KC_DEL},
        [9]  = {KC_SLSH, KC_BSLS},
        [10] = {KC_LBRC, KC_RBRC}
    }
};

void apply_user_config(void) {
    if (g_user_config.default_dpi >= 200 && g_user_config.default_dpi <= 3200) {
        pointing_device_set_cpi(g_user_config.default_dpi);
    }
    set_auto_mouse_enable(g_user_config.auto_mouse_en != 0);
    set_auto_mouse_layer(g_user_config.auto_mouse_layer);
    if (g_user_config.auto_mouse_time >= 100) {
        set_auto_mouse_timeout(g_user_config.auto_mouse_time);
    }
    if (g_user_config.dragscroll_buffer >= 1) {
        g_charybdis_dragscroll_buffer_size = g_user_config.dragscroll_buffer;
    }
    g_charybdis_dragscroll_reverse_y = (g_user_config.dragscroll_rev_y != 0);

    for (uint8_t i = 0; i < NUM_TD_SLOTS; i++) {
        if (g_user_config.td_slots[i].tap != 0) {
            td_tap_holds[i].tap = g_user_config.td_slots[i].tap;
            td_tap_holds[i].hold = g_user_config.td_slots[i].hold;
        }
    }
}

void load_user_config(void) {
    charybdis_user_config_t loaded;
    if (via_read_custom_config(&loaded, 0, sizeof(loaded)) == sizeof(loaded)) {
        if (loaded.magic == CHARYBDIS_CONFIG_MAGIC && loaded.version == CHARYBDIS_CONFIG_VERSION) {
            g_user_config = loaded;
        }
    }
    apply_user_config();
}

void save_user_config(void) {
    g_user_config.magic = CHARYBDIS_CONFIG_MAGIC;
    g_user_config.version = CHARYBDIS_CONFIG_VERSION;
    for (uint8_t i = 0; i < NUM_TD_SLOTS; i++) {
        g_user_config.td_slots[i].tap = td_tap_holds[i].tap;
        g_user_config.td_slots[i].hold = td_tap_holds[i].hold;
    }
    via_update_custom_config(&g_user_config, 0, sizeof(g_user_config));
}

void keyboard_post_init_user(void) {
    load_user_config();
}

void pointing_device_init_user(void) {
    apply_user_config();
}

// Ensure dragscroll cleanly turns off whenever exiting the mouse layer so it never gets stuck
layer_state_t layer_state_set_user(layer_state_t state) {
    uint8_t mouse_layer = g_user_config.auto_mouse_layer ? g_user_config.auto_mouse_layer : 3;
    if (!IS_LAYER_ON_STATE(state, mouse_layer)) {
        charybdis_set_pointer_dragscroll_enabled(false);
    }
    return state;
}

bool is_mouse_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case DRGSCRL:
        case DRG_TOG:
        case SNIPING:
        case SNP_TOG:
        case DPI_MOD:
            return true;
        default:
            return false;
    }
}

// WebHID custom packet receiver (Intercepts 0xFC, forwards everything else to VIA)
bool via_command_kb(uint8_t *data, uint8_t length) {
    if (data[0] == 0xFC) {
        uint8_t subcommand = data[1];
        switch (subcommand) {
            case 0x01: // GET_CONFIG
                data[2]  = (g_user_config.default_dpi >> 8) & 0xFF;
                data[3]  = g_user_config.default_dpi & 0xFF;
                data[4]  = (g_user_config.sniping_dpi >> 8) & 0xFF;
                data[5]  = g_user_config.sniping_dpi & 0xFF;
                data[6]  = g_user_config.auto_mouse_en;
                data[7]  = g_user_config.auto_mouse_layer;
                data[8]  = (g_user_config.auto_mouse_time >> 8) & 0xFF;
                data[9]  = g_user_config.auto_mouse_time & 0xFF;
                data[10] = g_user_config.dragscroll_buffer;
                data[11] = g_user_config.dragscroll_rev_y;
                data[12] = g_user_config.layer1_r;
                data[13] = g_user_config.layer1_g;
                data[14] = g_user_config.layer1_b;
                data[15] = g_user_config.layer2_r;
                data[16] = g_user_config.layer2_g;
                data[17] = g_user_config.layer2_b;
                data[18] = g_user_config.layer3_r;
                data[19] = g_user_config.layer3_g;
                data[20] = g_user_config.layer3_b;
                break;

            case 0x02: // SET_CONFIG (Live apply)
                g_user_config.default_dpi       = ((uint16_t)data[2] << 8) | data[3];
                g_user_config.sniping_dpi       = ((uint16_t)data[4] << 8) | data[5];
                g_user_config.auto_mouse_en     = data[6];
                g_user_config.auto_mouse_layer  = data[7];
                g_user_config.auto_mouse_time   = ((uint16_t)data[8] << 8) | data[9];
                g_user_config.dragscroll_buffer = data[10];
                g_user_config.dragscroll_rev_y  = data[11];
                g_user_config.layer1_r          = data[12];
                g_user_config.layer1_g          = data[13];
                g_user_config.layer1_b          = data[14];
                g_user_config.layer2_r          = data[15];
                g_user_config.layer2_g          = data[16];
                g_user_config.layer2_b          = data[17];
                g_user_config.layer3_r          = data[18];
                g_user_config.layer3_g          = data[19];
                g_user_config.layer3_b          = data[20];
                apply_user_config();
                data[2] = 1; // success
                break;

            case 0x03: // SAVE_EEPROM
                save_user_config();
                data[2] = 1; // success
                break;

            case 0x04: // RESET_CONFIG
                g_user_config.default_dpi       = 800;
                g_user_config.sniping_dpi       = 200;
                g_user_config.auto_mouse_en     = 1;
                g_user_config.auto_mouse_layer  = 3;
                g_user_config.auto_mouse_time   = 650;
                g_user_config.dragscroll_buffer = 20;
                g_user_config.dragscroll_rev_y  = 1;
                g_user_config.layer1_r = 255; g_user_config.layer1_g = 0;   g_user_config.layer1_b = 0;
                g_user_config.layer2_r = 0;   g_user_config.layer2_g = 0;   g_user_config.layer2_b = 255;
                g_user_config.layer3_r = 180; g_user_config.layer3_g = 0;   g_user_config.layer3_b = 255;
                // Reset TD slots
                g_user_config.td_slots[0]  = (td_config_slot_t){KC_N, KC_B};
                g_user_config.td_slots[1]  = (td_config_slot_t){KC_Z, LCTL(KC_Z)};
                g_user_config.td_slots[2]  = (td_config_slot_t){KC_X, LCTL(KC_X)};
                g_user_config.td_slots[3]  = (td_config_slot_t){KC_C, LCTL(KC_C)};
                g_user_config.td_slots[4]  = (td_config_slot_t){KC_V, LCTL(KC_V)};
                g_user_config.td_slots[5]  = (td_config_slot_t){KC_E, LGUI(KC_E)};
                g_user_config.td_slots[6]  = (td_config_slot_t){KC_RALT, KC_RCTL};
                g_user_config.td_slots[7]  = (td_config_slot_t){KC_MINS, KC_EQL};
                g_user_config.td_slots[8]  = (td_config_slot_t){KC_BSPC, KC_DEL};
                g_user_config.td_slots[9]  = (td_config_slot_t){KC_SLSH, KC_BSLS};
                g_user_config.td_slots[10] = (td_config_slot_t){KC_LBRC, KC_RBRC};
                apply_user_config();
                save_user_config();
                data[2] = 1;
                break;

            case 0x10: { // GET_TD_SLOT (slot = data[2])
                uint8_t slot = data[2];
                if (slot < NUM_TD_SLOTS) {
                    data[3] = (td_tap_holds[slot].tap >> 8) & 0xFF;
                    data[4] = td_tap_holds[slot].tap & 0xFF;
                    data[5] = (td_tap_holds[slot].hold >> 8) & 0xFF;
                    data[6] = td_tap_holds[slot].hold & 0xFF;
                    data[7] = 1; // success
                } else {
                    data[7] = 0;
                }
                break;
            }

            case 0x11: { // SET_TD_SLOT (slot = data[2], tap = data[3..4], hold = data[5..6])
                uint8_t slot = data[2];
                if (slot < NUM_TD_SLOTS) {
                    uint16_t tap  = ((uint16_t)data[3] << 8) | data[4];
                    uint16_t hold = ((uint16_t)data[5] << 8) | data[6];
                    td_tap_holds[slot].tap  = tap;
                    td_tap_holds[slot].hold = hold;
                    g_user_config.td_slots[slot].tap  = tap;
                    g_user_config.td_slots[slot].hold = hold;
                    data[7] = 1; // success
                } else {
                    data[7] = 0;
                }
                break;
            }

            default:
                data[1] = 0xFF;
                break;
        }
        raw_hid_send(data, length);
        return true;
    }
    return false;
}

// Layer-dependent RGB Matrix Indicators (Dynamic live colors)
bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    uint8_t current_layer = get_highest_layer(layer_state);

    switch (current_layer) {
        case 1: // 1번 레이어
            for (uint8_t i = led_min; i < led_max; i++) {
                rgb_matrix_set_color(i, g_user_config.layer1_r, g_user_config.layer1_g, g_user_config.layer1_b);
            }
            break;

        case 2: // 2번 레이어
            for (uint8_t i = led_min; i < led_max; i++) {
                rgb_matrix_set_color(i, g_user_config.layer2_r, g_user_config.layer2_g, g_user_config.layer2_b);
            }
            break;

        case 3: // 3번 레이어
            for (uint8_t i = led_min; i < led_max; i++) {
                rgb_matrix_set_color(i, g_user_config.layer3_r, g_user_config.layer3_g, g_user_config.layer3_b);
            }
            break;

        default:
            break;
    }

    return false;
}

