// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    bool     enabled;
    uint16_t low_gain_percent;
    uint16_t mid_gain_percent;
    uint16_t high_gain_percent;
    uint16_t start_speed_mm_s;
    uint16_t mid_speed_mm_s;
    uint16_t full_speed_mm_s;
} pointer_acceleration_config_t;

typedef struct {
    uint32_t last_report_ms;
    float    remainder_x;
    float    remainder_y;
} pointer_acceleration_state_t;

void pointer_acceleration_reset(pointer_acceleration_state_t *state, uint32_t now_ms);

// CPI is in actual counts/inch, NOT Keyball's internal units of 100 CPI.
// Call once per report interval, including intervals with no motion.
// Requires 0 < low_gain <= mid_gain <= high_gain and
// 0 <= start_speed < mid_speed < full_speed.
// Mutates motion and state only; does not access hardware or EEPROM.
void pointer_acceleration_apply(const pointer_acceleration_config_t *config, pointer_acceleration_state_t *state, uint32_t now_ms, uint16_t cpi, int16_t *x, int16_t *y);
