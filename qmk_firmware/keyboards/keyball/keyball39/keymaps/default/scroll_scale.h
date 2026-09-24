// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#include <stdint.h>

typedef struct {
    // Signed hundredths of a scroll step, always in [-99, 99].
    int8_t remainder_h;
    int8_t remainder_v;
} scroll_scale_state_t;

void scroll_scale_reset(scroll_scale_state_t *state);

// Apply a fixed gain to final scroll reports, after direction and snapping.
// Requires 1 <= gain_percent <= 100 and a zero-initialized or reset state.
// Mutates h, v and state only; zero input cannot emit accumulated motion.
void scroll_scale_apply(uint8_t gain_percent, scroll_scale_state_t *state, int8_t *h, int8_t *v);
