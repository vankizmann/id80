/* Copyright 2020 Sergey Vlasov <sigprof@gmail.com>
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

enum custom_layers {
  _WIN,
  _WINM,
  _OSX,
  _OSXM
};

enum custom_keycodes {
  BHOP = SAFE_RANGE
};

uint16_t BHOP_ACTIVE = 0;
uint16_t BHOP_TIMER = false;
uint16_t BHOP_LOOP = 0;
uint16_t BHOP_LIMIT = 204;
uint16_t BHOP_MOD = 300;

void start_bhop_loop(keyrecord_t *record) {

    if ( ! record->event.pressed ) {
 
        // RESET
        BHOP_ACTIVE = 0;
        BHOP_LOOP = 0;
        BHOP_LIMIT = 204;

        return;
    }
  
    // Send keycode for space
    SEND_STRING(SS_TAP(X_SPC));

    // Activate bhop loop
    BHOP_ACTIVE = 1;
    BHOP_TIMER = timer_read();

}

void matrix_scan_user(void) {

    // Return incase bhop is inactive
    if ( BHOP_ACTIVE == 0 ) {
        return;
    }

    // Return incase limit is not reached
    if ( timer_elapsed(BHOP_TIMER) < BHOP_LIMIT ) {
        return;
    }

    BHOP_LOOP = BHOP_LOOP + 1;

    if ( BHOP_LOOP <= 1 ) {
        return;
    }

    // Send keycode for wheeldown
    SEND_STRING(SS_TAP(X_WH_D));

    // Generate new limit
    BHOP_LIMIT = (rand() % 3) + 1;

    if ( BHOP_LOOP == 41 * BHOP_MOD ) {
        BHOP_LIMIT = BHOP_LIMIT + 62;
    }

    if ( BHOP_LOOP == 80 * BHOP_MOD ) {
        BHOP_LIMIT = BHOP_LIMIT + 51;
    }

    if ( BHOP_LOOP == 119 * BHOP_MOD ) {
        BHOP_LIMIT = BHOP_LIMIT + 64;
    }

    if ( BHOP_LOOP == 161 * BHOP_MOD ) {
        BHOP_LIMIT = BHOP_LIMIT + 48;
    }

    if ( BHOP_LOOP == 200 * BHOP_MOD ) {
        BHOP_LIMIT = BHOP_LIMIT + 60;
    }

    if ( BHOP_LOOP == 239 * BHOP_MOD ) {
        BHOP_LIMIT = BHOP_LIMIT + 63;
    }

    if ( BHOP_LOOP == 273 * BHOP_MOD ) {
       BHOP_LOOP = 1;
    }


    // Restart timer
    BHOP_TIMER = timer_read();

}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

    switch (keycode) {

        case BHOP:
            start_bhop_loop(record);
            break;

        case KC_INS:
            if ( ! record->event.pressed ) {
                SEND_STRING(SS_TAP(X_INS)SS_DELAY(100)SS_DOWN(X_INS));
            } else {
                SEND_STRING(SS_UP(X_INS)SS_DELAY(100)SS_TAP(X_INS));
            }
            break;

    }

    return true;
};


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_WIN] = LAYOUT(
        KC_ESC,    KC_F1,   KC_F2,   KC_F3,   KC_F4,     KC_F5,   KC_F6,   KC_F7,   KC_F8,     KC_F9,   KC_F10,  KC_F11,  KC_F12,   MO(_WINM),   KC_INS,
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,            TO(_OSX),
        KC_TAB,      KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,        KC_DEL,
        KC_INS,         KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_ENT,
        KC_LSFT,            KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,          KC_UP,
        KC_LCTL,   KC_LGUI,   KC_LALT,                         BHOP,                              KC_RSFT,     KC_RALT,        KC_LEFT, KC_DOWN, KC_RGHT
    ),
    [_WINM] = LAYOUT(
        RESET,    _______, _______, _______, _______,   _______, _______, _______, _______,   _______, _______, _______, _______,    KC_TRNS,    KC_MPLY,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,            _______,
        _______,     _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,        MAGIC_TOGGLE_NKRO,
        _______,       _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______,            _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          KC_VOLU,
        _______,   _______,   _______,                      _______,                              _______,     _______,        KC_MPRV, KC_VOLD,  KC_MNXT
    ),
    [_OSX] = LAYOUT(
        KC_ESC,    KC_F1,   KC_F2,   KC_F3,   KC_F4,     KC_F5,   KC_F6,   KC_F7,   KC_F8,     KC_F9,   KC_F10,  KC_F11,  KC_F12,   MO(_OSXM),  KC_INS,
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,            TO(_WIN),
        KC_TAB,      KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,        KC_DEL,
        MO(_OSXM),      KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_ENT,
        KC_LSFT,            KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,          KC_UP,
        KC_LGUI,   KC_LCTL,   KC_LALT,                       KC_SPC,                              KC_RSFT,     KC_RALT,        KC_LEFT, KC_DOWN, KC_RGHT
    ),
    [_OSXM] = LAYOUT(
        RESET,    _______, _______, _______, _______,   _______, _______, _______, _______,   _______, _______, _______, _______,    KC_TRNS,    KC_MPLY,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,            _______,
        _______,     _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,        MAGIC_TOGGLE_NKRO,
        _______,       _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______,            _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          KC_VOLU,
        _______,   _______,   _______,                      _______,                              _______,     _______,        KC_MRWD, KC_VOLD,  KC_MFFD
    ),
};
