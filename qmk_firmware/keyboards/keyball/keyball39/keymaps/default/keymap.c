/*
Copyright 2022 @Yowkees
Copyright 2022 MURAOKA Taro (aka KoRoN, @kaoriya)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H

#include "quantum.h"

// clang-format on
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  // Layer 0: Default Layer
  // Standard QWERTY layout with some modifications
  //
  // Q    W    E    R    T              Y    U    I    O    P
  // A    S    D    F    G              H    J    K    L    BSPC
  // Z    X    C    V    B              N    M    ,    .    RALT
  // LCTL ---  ---  --- SPC  TAB  RGUI ENT ESC BTN1 BTN2 RSFT
  //                    (L1) (L2) (L2) (L3)
  [0] = LAYOUT_universal(
    KC_Q     , KC_W     , KC_E     , KC_R     , KC_T     ,                            KC_Y     , KC_U     , KC_I     , KC_O     , KC_P     ,
    KC_A     , KC_S     , KC_D     , KC_F     , KC_G     ,                            KC_H     , KC_J     , KC_K     , KC_L     , KC_BSPC  ,
    KC_Z     , KC_X     , KC_C     , KC_V     , KC_B     ,                            KC_N     , KC_M     , KC_COMM  , KC_DOT   , KC_RALT  ,
    KC_LCTL  , _______  , _______  , _______  ,LT(1,KC_SPC),LT(2,KC_TAB),     LT(2, KC_RGUI),LT(3,KC_ENT),KC_ESC,KC_BTN1,KC_BTN2, KC_RSFT
  ),
  // Layer 1: Symbol Layer
  // Provides easy access to various symbols for programming and text editing
  //
  // `    ~    !    @    #              $    %    ^    &    *
  // \    |    (    [    {              }    ]    )    _    -
  // =    +    '    "    `              ~    :    ;    ?    /
  // LCTL LGUI LALT --- --- --- TO(2) TO(0) --- RALT RGUI RSFT
  [1] = LAYOUT_universal(
    KC_GRV   , KC_TILD  , KC_EXLM  , KC_AT    , KC_HASH  ,                            KC_DLR   , KC_PERC  , KC_CIRC  , KC_AMPR  , KC_ASTR  ,
    KC_BSLS  , KC_PIPE  , KC_LPRN  , KC_LBRC  , KC_LCBR  ,                            KC_RCBR  , KC_RBRC  , KC_RPRN  , KC_UNDS  , KC_MINS  ,
    KC_EQL   , KC_PLUS  , KC_QUOT  , KC_DQUO  , KC_GRV   ,                            KC_TILD  , KC_COLN  , KC_SCLN  , KC_QUES  , KC_SLSH  ,
    KC_LCTL  , KC_LGUI  , KC_LALT  , _______  , _______  , _______  ,      TO(2)    , TO(0)    , _______  , KC_RALT  , KC_RGUI  , KC_RSFT
  ),

  // Layer 2: Number (Top Row) and Symbol Layer
  // Combines number keys on the top row with additional symbols
  //
  // 1    2    3    4    5              6    7    8    9    0
  // !    @    #    $    %              ^    &    *    (    )
  // `    ~    _    -    +              =    [    ]    \    |
  // ESC  TAB  LALT ---  SPC  BSPC  --- ENT  .    ,    ;    '
  [2] = LAYOUT_universal(
    KC_1     , KC_2     , KC_3     , KC_4     , KC_5     ,                            KC_6     , KC_7     , KC_8     , KC_9     , KC_0     ,
    KC_EXLM  , KC_AT    , KC_HASH  , KC_DLR   , KC_PERC  ,                            KC_CIRC  , KC_AMPR  , KC_ASTR  , KC_LPRN  , KC_RPRN  ,
    KC_GRV   , KC_TILD  , KC_UNDS  , KC_MINS  , KC_PLUS  ,                            KC_EQL   , KC_LBRC  , KC_RBRC  , KC_BSLS  , KC_PIPE  ,
    KC_ESC   , KC_TAB   , KC_LALT  , _______  , KC_SPC   , KC_BSPC  ,      _______  , KC_ENT   , KC_DOT   , KC_COMM  , KC_SCLN  , KC_QUOT
  ),

  // Layer 3: Function and Settings Layer (Updated)
  // Includes function keys, RGB controls, keyboard settings, and Vim-like arrow keys
  //
  // F1   F2   F3   F4   F5             F6   F7   F8   F9   F10
  // F11  F12  SAI  VAI  SDV            LEFT DOWN UP   RGHT ---
  // RMOD HUD  SAD  VAD  DVS            CPI- CPI- CPI+ CPI+ SAVE
  // BOOT RST  ---  ---  ---  ---  ---  ---  ---  ---  RST  BOOT
  [3] = LAYOUT_universal(
    KC_F1    , KC_F2    , KC_F3    , KC_F4    , KC_F5    ,                            KC_F6    , KC_F7    , KC_F8    , KC_F9    , KC_F10   ,
    KC_F11   , KC_F12   , RGB_SAI  , RGB_VAI  , SCRL_DVI ,                            KC_LEFT  , KC_DOWN  , KC_UP    , KC_RIGHT , _______  ,
    RGB_RMOD , RGB_HUD  , RGB_SAD  , RGB_VAD  , SCRL_DVD ,                            CPI_D1K  , CPI_D100 , CPI_I100 , CPI_I1K  , KBC_SAVE ,
    QK_BOOT  , KBC_RST  , _______  , _______  , _______  , _______  ,      _______  , _______  , _______  , _______  , KBC_RST  , QK_BOOT
  ),
};
// clang-format on

layer_state_t layer_state_set_user(layer_state_t state) {
    // Auto enable scroll mode when the highest layer is 3
    keyball_set_scroll_mode(get_highest_layer(state) == 3);
    return state;
}

#ifdef OLED_ENABLE

#    include "lib/oledkit/oledkit.h"

void oledkit_render_info_user(void) {
    keyball_oled_render_keyinfo();
    keyball_oled_render_ballinfo();
    keyball_oled_render_layerinfo();
}
#endif
