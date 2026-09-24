/*
This is the c configuration file for the keymap

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

#pragma once

#ifdef RGBLIGHT_ENABLE
#    define RGBLIGHT_EFFECT_BREATHING
#    define RGBLIGHT_EFFECT_RAINBOW_MOOD
#    define RGBLIGHT_EFFECT_RAINBOW_SWIRL
#    define RGBLIGHT_EFFECT_SNAKE
#    define RGBLIGHT_EFFECT_KNIGHT
#    define RGBLIGHT_EFFECT_CHRISTMAS
#    define RGBLIGHT_EFFECT_STATIC_GRADIENT
#    define RGBLIGHT_EFFECT_RGB_TEST
#    define RGBLIGHT_EFFECT_ALTERNATING
#    define RGBLIGHT_EFFECT_TWINKLE
#endif

#define TAP_CODE_DELAY 5

#define POINTING_DEVICE_AUTO_MOUSE_ENABLE
#define AUTO_MOUSE_DEFAULT_LAYER 1

// Fixed pointer acceleration. Edit these values and rebuild/reflash to tune.
// Gains are percentages of sensor counts, before the host OS applies scaling.
// Smoothly connect LOW at START, MID at MID_SPEED, and HIGH at FULL.
#define KEYBALL_POINTER_ACCEL_ENABLED 1
#define KEYBALL_POINTER_ACCEL_LOW_GAIN_PERCENT 75
#define KEYBALL_POINTER_ACCEL_MID_GAIN_PERCENT 80
#define KEYBALL_POINTER_ACCEL_HIGH_GAIN_PERCENT 150
#define KEYBALL_POINTER_ACCEL_START_SPEED_MM_S 40
#define KEYBALL_POINTER_ACCEL_MID_SPEED_MM_S 200
#define KEYBALL_POINTER_ACCEL_FULL_SPEED_MM_S 400

// Scale final horizontal and vertical scroll amounts independently of cursor
// acceleration. Valid range: 1..100 percent of the existing scroll speed.
#define KEYBALL_SCROLL_GAIN_PERCENT 80

// Avoid the old +/-127-count cursor limit. Scroll reports remain 8-bit.
#define MOUSE_EXTENDED_REPORT

#if KEYBALL_POINTER_ACCEL_ENABLED != 0 && KEYBALL_POINTER_ACCEL_ENABLED != 1
#    error "KEYBALL_POINTER_ACCEL_ENABLED must be 0 or 1"
#endif
#if KEYBALL_POINTER_ACCEL_LOW_GAIN_PERCENT < 1 || KEYBALL_POINTER_ACCEL_MID_GAIN_PERCENT < KEYBALL_POINTER_ACCEL_LOW_GAIN_PERCENT || KEYBALL_POINTER_ACCEL_HIGH_GAIN_PERCENT < KEYBALL_POINTER_ACCEL_MID_GAIN_PERCENT
#    error "Pointer acceleration requires 0 < low gain <= mid gain <= high gain"
#endif
#if KEYBALL_POINTER_ACCEL_START_SPEED_MM_S < 0 || KEYBALL_POINTER_ACCEL_MID_SPEED_MM_S <= KEYBALL_POINTER_ACCEL_START_SPEED_MM_S || KEYBALL_POINTER_ACCEL_FULL_SPEED_MM_S <= KEYBALL_POINTER_ACCEL_MID_SPEED_MM_S
#    error "Pointer acceleration requires 0 <= start speed < mid speed < full speed"
#endif
#if KEYBALL_SCROLL_GAIN_PERCENT < 1 || KEYBALL_SCROLL_GAIN_PERCENT > 100
#    error "KEYBALL_SCROLL_GAIN_PERCENT must be between 1 and 100"
#endif
