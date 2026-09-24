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
#include "pointer_acceleration.h"
#include "scroll_scale.h"

static const pointer_acceleration_config_t pointer_acceleration_config = {
    .enabled           = KEYBALL_POINTER_ACCEL_ENABLED,
    .low_gain_percent  = KEYBALL_POINTER_ACCEL_LOW_GAIN_PERCENT,
    .mid_gain_percent  = KEYBALL_POINTER_ACCEL_MID_GAIN_PERCENT,
    .high_gain_percent = KEYBALL_POINTER_ACCEL_HIGH_GAIN_PERCENT,
    .start_speed_mm_s  = KEYBALL_POINTER_ACCEL_START_SPEED_MM_S,
    .mid_speed_mm_s    = KEYBALL_POINTER_ACCEL_MID_SPEED_MM_S,
    .full_speed_mm_s   = KEYBALL_POINTER_ACCEL_FULL_SPEED_MM_S,
};

// Track each physical side independently, regardless of the USB connection side.
static pointer_acceleration_state_t pointer_acceleration_states[2];

// Scroll scaling runs once on the combined report, after both halves.
static scroll_scale_state_t scroll_scale_state;

// clang-format on
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    // Layer 0: デフォルトレイヤー
    // 標準的なQWERTYレイアウトにいくつかの修正を加えています
    //
    // Q    W    E    R    T              Y    U    I    O    P
    // A    S    D    F    G              H    J    K    L    BSPC
    // Z    X    C    V    B              N    M   LCTL  RSFT RALT
    // LCTL ---  ---  --- SPC  MO(1) RSFT MO(2) ESC BTN1 BTN2 RGUI
  [0] = LAYOUT_universal(
    KC_Q     , KC_W     , KC_E     , KC_R     , KC_T     ,                            KC_Y     , KC_U     , KC_I     , KC_O     , KC_P     ,
    KC_A     , KC_S     , KC_D     , KC_F     , KC_G     ,                            KC_H     , KC_J     , KC_K     , KC_L     , KC_BSPC  ,
    KC_Z     , KC_X     , KC_C     , KC_V     , KC_B     ,                            KC_N     , KC_M     , KC_LCTL  , KC_RSFT   , KC_RALT  ,
    // KC_LCTL  , _______  , _______  , _______  ,LT(1,KC_SPC),LT(2,KC_TAB),     KC_RGUI,LT(3,KC_ENT),KC_ESC,KC_BTN1,KC_BTN2, KC_RSFT
    KC_LCTL  , _______  , _______  , _______  ,KC_SPC, MO(1),     KC_RSFT,MO(2),KC_ESC,KC_BTN1,KC_BTN2, KC_RGUI
  ),
    // Layer 1: シンボルレイヤー
    // プログラミングやテキスト編集のためのシンボルに簡単にアクセスできます
    //
    // `    ~    !    @    #              $    %    ^    &    *
    // \    |    (    [    {              }    ]    )    _    -
    // =    +    '    "    `              ~    :    ;    ?    /
    // LCTL ---  ---  --- SPC  ---  RSFT ---  ESC BTN1 BTN2 RGUI
  [1] = LAYOUT_universal(
    KC_GRV   , KC_TILD  , KC_EXLM  , KC_AT    , KC_HASH  ,                            KC_DLR   , KC_PERC  , KC_CIRC  , KC_AMPR  , KC_ASTR  ,
    KC_BSLS  , KC_PIPE  , KC_LPRN  , KC_LBRC  , KC_LCBR  ,                            KC_RCBR  , KC_RBRC  , KC_RPRN  , KC_UNDS  , KC_MINS  ,
    KC_EQL   , KC_PLUS  , KC_QUOT  , KC_DQUO  , KC_GRV   ,                            KC_TILD  , KC_COLN  , KC_SCLN  , KC_QUES  , KC_SLSH  ,
    KC_LCTL  , _______  , _______  , _______  ,KC_SPC, _______,     KC_RSFT,_______,KC_ESC,KC_BTN1,KC_BTN2, KC_RGUI
    // KC_LCTL  , KC_LGUI  , KC_LALT  , _______  , KC_SPC  , _______  ,      KC_RGUI    , MO(3)    , _______  , KC_RALT  , KC_RGUI  , KC_RSFT
  ),

    // Layer 2: 数字とファンクションレイヤー
    // 上部の数字キーとファンクションキー、シンボルを組み合わせています
    //
    // 1    2    3    4    5              6    7    8    9    0
    // F1   F2   F3   F4   F5             F6   F7   F8   F9   F10
    // F11  F12  TAB  ENT  <              >    [    ]    ,    .
    // LCTL TAB  LALT ---  SPC  ---  RSFT ENT  .    ,    RGUI RGUI
  [2] = LAYOUT_universal(
    KC_1     , KC_2     , KC_3     , KC_4     , KC_5     ,                            KC_6     , KC_7     , KC_8     , KC_9     , KC_0     ,
    // KC_EXLM  , KC_AT    , KC_HASH  , KC_DLR   , KC_LT    ,                            KC_GT    , KC_AMPR  , KC_ASTR  , KC_LT    , KC_GT    ,
    KC_F1    , KC_F2    , KC_F3    , KC_F4    , KC_F5    ,                            KC_F6    , KC_F7    , KC_F8    , KC_F9    , KC_F10   ,
    KC_F11   , KC_F12  , KC_TAB  , KC_ENT  , KC_LT    ,                            KC_GT    , KC_LBRC  , KC_RBRC  , KC_COMM  , KC_DOT   ,
    KC_LCTL   , KC_TAB   , KC_LALT  , _______  , KC_SPC   , _______  ,      KC_RSFT  , KC_ENT   , KC_DOT   , KC_COMM  , KC_RGUI  , KC_RGUI
  ), 

    // Layer 3: ファンクションと設定レイヤー
    // ファンクションキー、矢印キー、および設定を含みます
    //
    // F1   F2   F3   F4   F5             F6   F7   F8   F9   F10
    // F11  F12  ---  ---  ---           LEFT DOWN UP   RIGHT ---
    // ---  ---  ---  ---  ---            ---   ---   ---   ---   ---
    // LCTL ---  ---  ---  ---  ---  ---  ---   ---   ---   RGUI RSFT
  [3] = LAYOUT_universal(
    KC_F1    , KC_F2    , KC_F3    , KC_F4    , KC_F5    ,                            KC_F6    , KC_F7    , KC_F8    , KC_F9    , KC_F10   ,
    KC_F11   , KC_F12   , _______  , _______  , _______ ,                            KC_LEFT  , KC_DOWN  , KC_UP    , KC_RIGHT , _______  ,
    _______ , _______  , _______  , _______  , _______ ,                            _______  , _______ ,  _______ , _______  , _______ ,
    KC_LCTL  , _______  , _______  , _______  , _______  , _______  ,      _______  , _______  , _______  , _______  , KC_RGUI  , KC_RGUI
  ),
};
// clang-format on

static void reset_pointer_acceleration(void) {
    const uint32_t now = timer_read32();
    pointer_acceleration_reset(&pointer_acceleration_states[0], now);
    pointer_acceleration_reset(&pointer_acceleration_states[1], now);
}

void keyboard_post_init_user(void) {
    reset_pointer_acceleration();
    scroll_scale_reset(&scroll_scale_state);
}

void keyball_on_apply_motion_to_mouse_move(keyball_motion_t *motion, report_mouse_t *report, bool is_left) {
    // Keyball calls the hook for BOTH halves, even when one has no sensor.
    // Ignore that empty source without advancing the real ball's clock or
    // overwriting a cursor report already produced by the other half.
    const bool has_ball = is_left == is_keyboard_left() ? keyball.this_have_ball : keyball.that_have_ball;
    if (!has_ball) {
        motion->x = 0;
        motion->y = 0;
        return;
    }

    // keyball_get_cpi() returns units of 100 CPI (despite its old API comment).
    const uint16_t cpi = (uint16_t)keyball_get_cpi() * 100;
    pointer_acceleration_apply(&pointer_acceleration_config, &pointer_acceleration_states[is_left ? 0 : 1], timer_read32(), cpi, &motion->x, &motion->y);

    // Preserve Keyball39's existing sensor orientation. Values are symmetric
    // +/-32767, so negation is safe for a trackball on the left half too.
    report->x = is_left ? -motion->y : motion->y;
    report->y = is_left ? -motion->x : motion->x;
    motion->x = 0;
    motion->y = 0;
}

report_mouse_t pointing_device_task_user(report_mouse_t report) {
    // Keep Keyball's direction and snapping; never scale cursor axes or buttons.
    scroll_scale_apply(KEYBALL_SCROLL_GAIN_PERCENT, &scroll_scale_state, &report.h, &report.v);
    return report;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    // レイヤー1とレイヤー2の同時押しで3レイヤーに移動する
    state = update_tri_layer_state(state, 1, 2, 3);

    // レイヤー2に移動したときにスクロールモードを有効にする
    const bool scroll_mode = get_highest_layer(state) == 2;
    if (scroll_mode != keyball_get_scroll_mode()) {
        reset_pointer_acceleration();
        scroll_scale_reset(&scroll_scale_state);
    }
    keyball_set_scroll_mode(scroll_mode);
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
