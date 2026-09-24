// SPDX-License-Identifier: GPL-2.0-or-later
#include "pointer_acceleration.h"

#include <math.h>

void pointer_acceleration_reset(pointer_acceleration_state_t *state, uint32_t now_ms) {
    state->last_report_ms = now_ms;
    state->remainder_x    = 0.0f;
    state->remainder_y    = 0.0f;
}

static float gain_for_speed(const pointer_acceleration_config_t *config, float speed_mm_s) {
    const float low  = config->low_gain_percent / 100.0f;
    const float mid  = config->mid_gain_percent / 100.0f;
    const float high = config->high_gain_percent / 100.0f;
    if (speed_mm_s <= config->start_speed_mm_s) {
        return low;
    }
    if (speed_mm_s >= config->full_speed_mm_s) {
        return high;
    }

    const bool     lower_segment = speed_mm_s < config->mid_speed_mm_s;
    const float    start_gain    = lower_segment ? low : mid;
    const float    end_gain      = lower_segment ? mid : high;
    const uint16_t start_speed   = lower_segment ? config->start_speed_mm_s : config->mid_speed_mm_s;
    const uint16_t end_speed     = lower_segment ? config->mid_speed_mm_s : config->full_speed_mm_s;

    // Each smoothstep segment has zero slope at its endpoints, so the gain
    // stays smooth at the middle anchor too, without averaging or inertia.
    const float t = (speed_mm_s - start_speed) / (end_speed - start_speed);
    return start_gain + (end_gain - start_gain) * t * t * (3.0f - 2.0f * t);
}

static int16_t quantize_axis(float value, float *remainder) {
    // Clamp BEFORE the float-to-int conversion. Never queue overflow as motion
    // to send later: that would keep the cursor moving after the ball stops.
    if (value >= 32767.0f) {
        *remainder = 0.0f;
        return 32767;
    }
    if (value <= -32767.0f) {
        *remainder = 0.0f;
        return -32767;
    }
    const int16_t whole = (int16_t)value;
    *remainder          = value - whole;
    return whole;
}

void pointer_acceleration_apply(const pointer_acceleration_config_t *config, pointer_acceleration_state_t *state, uint32_t now_ms, uint16_t cpi, int16_t *x, int16_t *y) {
    // Unsigned subtraction also handles the 32-bit millisecond timer wrapping.
    const uint32_t elapsed_ms = now_ms - state->last_report_ms;
    state->last_report_ms     = now_ms;

    const float dx = *x;
    const float dy = *y;
    float       gain;
    if (config->enabled) {
        float speed_mm_s = 0.0f;
        if (elapsed_ms != 0 && cpi != 0) {
            // 25.4 mm/inch * 1000 ms/second; use real elapsed time, not an
            // assumed polling rate. Float products avoid int16 overflow on AVR.
            speed_mm_s = sqrtf(dx * dx + dy * dy) * 25400.0f / cpi / elapsed_ms;
        }
        gain = gain_for_speed(config, speed_mm_s);
    } else {
        gain               = 1.0f;
        state->remainder_x = 0.0f;
        state->remainder_y = 0.0f;
    }

    float scaled_x = dx * gain + state->remainder_x;
    float scaled_y = dy * gain + state->remainder_y;

    // Keep a common scale for both axes at the extended report limit, instead
    // of independently clipping them and changing the direction. Divide first
    // so the largest component reaches exactly +/-32767, without rounding
    // 32767 / peak * peak down to 32766.999... before integer conversion.
    const float abs_x = scaled_x < 0.0f ? -scaled_x : scaled_x;
    const float abs_y = scaled_y < 0.0f ? -scaled_y : scaled_y;
    const float peak  = abs_x > abs_y ? abs_x : abs_y;
    if (peak > 32767.0f) {
        scaled_x = (scaled_x / peak) * 32767.0f;
        scaled_y = (scaled_y / peak) * 32767.0f;
    }

    *x = quantize_axis(scaled_x, &state->remainder_x);
    *y = quantize_axis(scaled_y, &state->remainder_y);
}
