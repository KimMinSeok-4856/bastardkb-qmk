#pragma once

#define SPLIT_LAYER_STATE_ENABLE
#define SPLIT_LED_STATE_ENABLE

// PC 절전 모드(USB Suspend) 감지 시 키보드 LED 자동 소등
#define RGB_MATRIX_SLEEP
#define RGB_DISABLE_WHEN_USB_SUSPENDED
#define RGB_MATRIX_TIMEOUT 0 // 동적 타이머(Vial GUI 설정)로 제어

// Trackball Mouse Pointer Sensitivity (기본 1600 -> 800으로 절반 감속)
#define PMW33XX_CPI 800
#define CHARYBDIS_MINIMUM_DEFAULT_DPI 800

// Drag-scroll sensitivity and direction
#define CHARYBDIS_DRAGSCROLL_BUFFER_SIZE 20
#define CHARYBDIS_DRAGSCROLL_REVERSE_Y

// Auto Mouse Layer configuration
#define POINTING_DEVICE_AUTO_MOUSE_ENABLE
#define AUTO_MOUSE_DEFAULT_LAYER 3
#define AUTO_MOUSE_TIME 650
#define AUTO_MOUSE_DEBOUNCE 25
#define AUTO_MOUSE_THRESHOLD 10

// VIA Custom EEPROM block (128 bytes)
#define VIA_EEPROM_CUSTOM_CONFIG_SIZE 128

// Split transaction for syncing dynamic RGB layer colors to slave
#define SPLIT_TRANSACTION_IDS_USER RPC_ID_USER_CONFIG_SYNC

#ifdef VIAL_ENABLE
#    define VIAL_KEYBOARD_UID { 0x6D, 0xA5, 0xCD, 0x8D, 0xC7, 0x3D, 0x7B, 0xA8 }
#    define VIAL_UNLOCK_COMBO_ROWS { 0, 4 }
#    define VIAL_UNLOCK_COMBO_COLS { 0, 0 }
#    define VIAL_INSECURE
#    define DYNAMIC_KEYMAP_LAYER_COUNT 4
#    define DYNAMIC_KEYMAP_MACRO_COUNT 32
#    define VIAL_TAP_DANCE_ENTRIES 32
#    define VIAL_COMBO_ENTRIES 32
#    define VIAL_KEY_OVERRIDE_ENTRIES 32
#endif

