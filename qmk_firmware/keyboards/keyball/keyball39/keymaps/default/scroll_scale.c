// SPDX-License-Identifier: GPL-2.0-or-later
#include "scroll_scale.h"

void scroll_scale_reset(scroll_scale_state_t *state) {
    state->remainder_h = 0;
    state->remainder_v = 0;
}

static int8_t scale_axis(uint8_t gain_percent, int8_t value, int8_t *remainder) {
    // Integer hundredths preserve small scroll steps without float rounding.
    // With gain <= 100 and |remainder| < 100, the sum fits int16_t and the
    // quotient fits int8_t. C division truncates toward zero in both directions.
    const int16_t scaled = (int16_t)value * gain_percent + *remainder;
    *remainder          = (int8_t)(scaled % 100);
    return (int8_t)(scaled / 100);
}

void scroll_scale_apply(uint8_t gain_percent, scroll_scale_state_t *state, int8_t *h, int8_t *v) {
    *h = scale_axis(gain_percent, *h, &state->remainder_h);
    *v = scale_axis(gain_percent, *v, &state->remainder_v);
}
