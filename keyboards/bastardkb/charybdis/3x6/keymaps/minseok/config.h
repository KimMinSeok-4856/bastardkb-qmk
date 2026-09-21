#pragma once

#define SPLIT_LAYER_STATE_ENABLE
#define SPLIT_LED_STATE_ENABLE

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

