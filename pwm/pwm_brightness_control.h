#ifndef PWM_BRIGHTNESS_CONTROL_H
#define PWM_BRIGHTNESS_CONTROL_H

#include "pwm_brightness_control_defs.h"

void pwm_setup();

PWMStatus pwm_increase_brightness(
    PWMColor color, uint8_t targetBrightness, uint16_t durationMs);

#endif // PWM_BRIGHTNESS_CONTROL_H