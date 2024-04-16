#include QMK_KEYBOARD_H

#include "transport.h"

static bool mic_indicator = false;
static bool mic_indicator_phase = false;
static uint8_t mic_indicator_time = 0;

static bool leader_indicator = false;
static bool leader_indicator_phase = false;
static uint8_t leader_indicator_time = 0;
static bool leader_keys_effect_state = false;

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    uint8_t brightness;
    if (get_transport() == TRANSPORT_USB && mic_indicator) {
        if (mic_indicator_time != 0 || mic_indicator_phase) {
            brightness = mic_indicator_time * (!mic_indicator_phase * 255 - mic_indicator_phase * 255) / 255 + mic_indicator_phase * 255;
            RGB_MATRIX_INDICATOR_SET_COLOR(14, brightness, 0, 0);
        }
        if (mic_indicator_time == 255)
        {
            mic_indicator_phase = !mic_indicator_phase;
        }
        mic_indicator_time++;
    }
    if (leader_indicator) {
        if (leader_indicator_time != 0 || leader_indicator_time) {
            brightness = leader_indicator_time * (!leader_indicator_phase * 255 - leader_indicator_phase * 255) / 255 + leader_indicator_phase * 255;
            RGB_MATRIX_INDICATOR_SET_COLOR(81, 0, 0, brightness);
        }
        if (leader_indicator_time == 255)
        {
            leader_indicator_phase = !leader_indicator_phase;
            leader_keys_effect_state = true;
        }

		if (leader_keys_effect_state) {
			RGB_MATRIX_INDICATOR_SET_COLOR(75, 0, 0, 255);
	        RGB_MATRIX_INDICATOR_SET_COLOR(84, 0, 0, 255);
	        RGB_MATRIX_INDICATOR_SET_COLOR(85, 0, 0, 255);
	        RGB_MATRIX_INDICATOR_SET_COLOR(86, 0, 0, 255);
		}
		else
		{
			RGB_MATRIX_INDICATOR_SET_COLOR(75, 0, 0, leader_indicator_time);
	        RGB_MATRIX_INDICATOR_SET_COLOR(84, 0, 0, leader_indicator_time);
	        RGB_MATRIX_INDICATOR_SET_COLOR(85, 0, 0, leader_indicator_time);
	        RGB_MATRIX_INDICATOR_SET_COLOR(86, 0, 0, leader_indicator_time);
		}
        
        leader_indicator_time++;
    }
    else if (leader_keys_effect_state) {
    	RGB_MATRIX_INDICATOR_SET_COLOR(75, 0, 0, leader_indicator_time);
        RGB_MATRIX_INDICATOR_SET_COLOR(84, 0, 0, leader_indicator_time);
        RGB_MATRIX_INDICATOR_SET_COLOR(85, 0, 0, leader_indicator_time);
        RGB_MATRIX_INDICATOR_SET_COLOR(86, 0, 0, leader_indicator_time);
    	leader_indicator_time -= 2;

    	if (leader_indicator_time == 0)
        {
            leader_keys_effect_state = false;
        }
    }
    return false;
}

void via_command_user(uint8_t *data, uint8_t length) {
    switch (data[1]) {
        case 0:
            bool new_mic_state = data[2] > 0;
            if (new_mic_state != mic_indicator) {
                mic_indicator_phase = false;
                mic_indicator_time = 0;
                mic_indicator = new_mic_state;
            }
            break;
    }
}

// Tap Dance declarations
enum {
    TD_MIC,
};

void td_mic_fn(tap_dance_state_t *state, void *user_data) {
    if (get_default_transport() == TRANSPORT_USB) {
        switch (state->count) {
            case 1:
                register_code(KC_LGUI);
                tap_code(KC_MUTE);
                unregister_code(KC_LGUI);

                break;
            default:
                tap_code(KC_PSCR);
        }
    }
}

void leader_start_user() {
    leader_keys_effect_state = false;
    leader_indicator_phase = false;
    leader_indicator_time = 0;
    leader_indicator = true;
}

void leader_end_user() {
    if (leader_sequence_four_keys(KC_LEFT, KC_RIGHT, KC_DOWN, KC_DOWN)) {
        tap_code(KC_SLEP);
    }
    else if (leader_sequence_four_keys(KC_LEFT, KC_RIGHT, KC_LEFT, KC_LEFT)) {
    	register_code(KC_LGUI);
 	    tap_code(KC_L);
 	    unregister_code(KC_LGUI);
    }
    leader_keys_effect_state = true;
    if ((leader_indicator_time & 1) != 0) {
        leader_indicator_time--;
    }
    leader_indicator = false;
}

// Tap Dance definitions
tap_dance_action_t tap_dance_actions[] = {
    [TD_MIC] = ACTION_TAP_DANCE_FN(td_mic_fn)
};

// clang-format off
enum layers{
  MAC_BASE,
  MAC_FN,
  WIN_BASE,
  WIN_FN,
  CUSTOM
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[MAC_BASE] = LAYOUT_tkl_ansi(
     KC_ESC,   KC_BRID,  KC_BRIU,  KC_MCTL,  KC_LPAD,  RGB_VAD,  RGB_VAI,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,  KC_VOLU,           KC_SNAP,   KC_SIRI,  KC_SNAP,
     KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC, KC_INS,    KC_HOME,  KC_PGUP,
     KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,  KC_BSLS, KC_DEL,    KC_END,   KC_PGDN,
     KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,            KC_ENT,
     KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,            KC_RSFT,            KC_UP,
     KC_LCTL,  KC_LOPTN, KC_LCMMD,                               KC_SPC,                                 KC_RCMMD, KC_ROPTN, MO(MAC_FN),KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT),

[MAC_FN] = LAYOUT_tkl_ansi(
     QK_BOOT,  KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,             KC_TRNS,  KC_TRNS,  RGB_TOG,
     BT_USB,   BT_HST1,  BT_HST2,  BT_HST3,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,
     RGB_TOG,  RGB_MOD,  RGB_VAI,  RGB_HUI,  RGB_SAI,  RGB_SPI,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,
     KC_TRNS,  RGB_RMOD, RGB_VAD,  RGB_HUD,  RGB_SAD,  RGB_SPD,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,            KC_TRNS,
     KC_TRNS,            KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  BAT_LVL,  NK_TOGG,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,            KC_TRNS,            KC_TRNS,
     KC_TRNS,  KC_TRNS,  KC_TRNS,                                KC_TRNS,                                KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS),

[WIN_BASE] = LAYOUT_tkl_ansi(
     KC_ESC,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,             KC_PSCR,  TD(TD_MIC),KC_PSCR,
     KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,  KC_INS,    KC_HOME,  KC_PGUP,
     KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,  KC_BSLS,  KC_DEL,    KC_END,   KC_PGDN,
     KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,            KC_ENT,
     KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,            KC_RSFT,             KC_UP,
     KC_LCTL,  KC_LGUI,  KC_LALT,                                KC_SPC,                                 KC_RALT,  QK_LEAD, MO(WIN_FN),KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT),

[WIN_FN] = LAYOUT_tkl_ansi(
     QK_BOOT,  KC_BRID,  KC_BRIU,  KC_TASK,  KC_FILE,  RGB_VAD,  RGB_VAI,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,  KC_VOLU,            KC_PSCR,  KC_TRNS,  RGB_TOG,
     BT_USB,   BT_HST1,  BT_HST2,  BT_HST3,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,
     RGB_TOG,  RGB_MOD,  RGB_VAI,  RGB_HUI,  RGB_SAI,  RGB_SPI,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,
     KC_TRNS,  RGB_RMOD, RGB_VAD,  RGB_HUD,  RGB_SAD,  RGB_SPD,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,            KC_TRNS,
     KC_TRNS,            KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  BAT_LVL,  NK_TOGG,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,            KC_TRNS,            KC_TRNS,
     KC_TRNS,  KC_TRNS,  KC_TRNS,                                KC_TRNS,                                KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS),

[CUSTOM] = LAYOUT_tkl_ansi(
     KC_ESC,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,             KC_PSCR,  TD(TD_MIC),KC_PSCR,
     KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,  KC_INS,    KC_HOME,  KC_PGUP,
     KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,  KC_BSLS,  KC_DEL,    KC_END,   KC_PGDN,
     KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,            KC_ENT,
     KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,            KC_RSFT,             KC_UP,
     KC_LCTL,  KC_LGUI,  KC_LALT,                                KC_SPC,                                 KC_RALT,  QK_LEAD, MO(WIN_FN),KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT)

};
