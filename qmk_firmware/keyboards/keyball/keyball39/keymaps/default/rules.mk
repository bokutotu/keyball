RGBLIGHT_ENABLE = yes

# This keyboard has no OLED; leave flash space for pointer acceleration.
OLED_ENABLE = no

SRC += $(KEYMAP_PATH)/pointer_acceleration.c
SRC += $(KEYMAP_PATH)/scroll_scale.c
