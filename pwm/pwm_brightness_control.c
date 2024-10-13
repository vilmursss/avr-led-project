#include "pwm_brightness_control.h"
#include "pwm_brightness_control_defs.h"

#include <clock/timer.h>
#include <log/logger.h>
#include <writer/reg_writer.h>

#include <stddef.h>

// Registers
volatile uint8_t* const TCCR2A_REG = (volatile uint8_t*)0xB0; // Timer/Counter2 Control Register A
volatile uint8_t* const TCCR2B_REG = (volatile uint8_t*)0xB1; // Timer/Counter2 Control Register b
volatile uint8_t* const OCR2A_REG = (volatile uint8_t*)0xB3; // Timer/Counter2 Output Compare Register A
volatile uint8_t* const OCR2B_REG = (volatile uint8_t*)0xB4; // Timer/Counter2 Output Compare Register B
volatile uint8_t* const ASSR_REG = (volatile uint8_t*)0xB6;  // Asynchronous Status Register

// Local functions
static volatile uint8_t* const get_brightness_register(PWMColor color);

void pwm_setup()
{
    // Ensure AS2 is set correctly because it may corrupt OCR2A/B
    *ASSR_REG &= ~(1 << 5); // Clear AS2 to use the I/O clock

    // Set Fast PWM mode with non-inverted output
    TCCR2ABits tccr2aBits = {0};
    tccr2aBits.waveformGenMode = 0x2;
    tccr2aBits.compareOutputModeA = 0x1;

    WriteStatus ret = reg_write_bits(TCCR2A_REG, &tccr2aBits, REG_SIZE_8);
    if (ret != WRITE_OK)
    {
        log_warning("Failed to write TCCR2ABits (SetFast PWM mode) to TCCR2A_REG (Error Code: %d)\n", ret);
    }

    // Set prescaler to 64 and start the timer
    TCCR2BBits tccr2bBits = {0};
    tccr2bBits.clockSelect = 0x4;
    ret = reg_write_bits(TCCR2B_REG, &tccr2bBits, REG_SIZE_8);
    if (ret != WRITE_OK)
    {
        log_warning("Failed to write TCCR2BBits (Set prescaler) to TCCR2B_REG (Error Code: %d)\n", ret);
    }

    // Initial duty cycle
    *OCR2A_REG = 0; // Red
    *OCR2B_REG = 0; // Green
}

PWMStatus pwm_increase_brightness(const PWMColor color, uint8_t targetBrightness, uint16_t durationMs) {
    PWMStatus ret = PWM_SUCCESS;

    if (color == PWM_COLOR_UNKNOWN)
    {
        ret = PWM_FAILURE_INVALID_COLOR;
        log_warning("Invalid PWM color selected.\n");
        return ret;
    }

    volatile uint8_t* const ocrReg = get_brightness_register(color);
    if (ocrReg == NULL)
    {
        ret = PWM_FAILURE_INVALID_COLOR;
        log_warning("Could not find brightness pointer for the given color\n");  
        return ret;
    }

    uint8_t currentBrightness = *ocrReg;
    if (currentBrightness > targetBrightness)
    {
        ret = PWM_FAILURE_HIGHER_CURRENT_BRIGHTNESS;
        log_warning("Current brightness is already higher than the target brightness.\n");
        return ret;
    }
    
    if (durationMs > 10000)
    {
        ret = PWM_FAILURE_TOO_LARGE_DURATION;
        log_warning("Duration is too large.\n");
        return ret;
    }

    uint16_t steps = durationMs / 10;
    uint8_t step = (targetBrightness - currentBrightness) / steps;
    for (uint16_t i = 0; i < steps; i++)
    {
        currentBrightness += step;
        *ocrReg = currentBrightness;
        timer_delay_ms(10);
    }

    // Ensure the final brightness is set to the target value
    *ocrReg = targetBrightness;

    return ret;
}

static volatile uint8_t* const get_brightness_register(const PWMColor color)
{
    switch (color)
    {
        case PWM_BLUE:
            // Not implemented yet   
            return NULL; 
        case PWM_RED:
            return OCR2A_REG;
        case PWM_GREEN:
            return OCR2B_REG;
            break;
        default:
            return NULL;
    }

    return NULL;
}