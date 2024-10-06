#include "rgb.h"
#include "rgb_defs.h"

#include <clock/timer.h>
#include <log/logger.h>
#include <writer/reg_writer.h>

// Registers
volatile uint8_t* const DDRB_REG = (volatile uint8_t*)0x24; // Port B Data Direction Register
volatile uint8_t* const PORTB_REG = (volatile uint8_t*)0x25; // Port B Data Register

/// Define the pins for each color
#define RED_PIN PB3
#define GREEN_PIN PB4
#define BLUE_PIN PB5

void rgb_setup()
{
    // Set the RGB pins as output
    DDRBBits ddrBBits = {0};
    ddrBBits.DB3 = 1; // Red
    ddrBBits.DB4 = 1; // Green
    ddrBBits.DB5 = 1; // Blue

    const WriteStatus ret = reg_write_bits(DDRB_REG, &ddrBBits, REG_SIZE_8);
    if (ret != WRITE_OK)
    {
        log_warning("Failed to write PORTBBits to DDRB_REG (Error Code: %d)\n", ret);
    }
}

void rgb_test_all_colors()
{
    // Turn on Red
    PORTBBits portBBits = {0};
    portBBits.RED_PIN = 1;
    portBBits.GREEN_PIN = 0;
    portBBits.BLUE_PIN = 0;

    WriteStatus ret = reg_write_bits(PORTB_REG, &portBBits, REG_SIZE_8);
    if (ret != WRITE_OK)
    {
        log_warning("Failed to write PORTBBits to PORTB_REG (Error Code: %d)\n", ret);
    }
    timer_delay_ms(1000);


    // Turn on Gree
    portBBits.RED_PIN = 0;
    portBBits.GREEN_PIN = 1;
    portBBits.BLUE_PIN = 0;
    ret = reg_clear_and_write_bits(PORTB_REG, &portBBits, REG_SIZE_8);
    if (ret != WRITE_OK)
    {
        log_warning("Failed to write PORTBBits to PORTB_REG (Error Code: %d)\n", ret);
    }
    timer_delay_ms(1000);

    // Turn on Blue
    portBBits.RED_PIN = 0;
    portBBits.GREEN_PIN = 0;
    portBBits.BLUE_PIN = 1;
    ret = reg_clear_and_write_bits(PORTB_REG, &portBBits, REG_SIZE_8);
    if (ret != WRITE_OK)
    {
        log_warning("Failed to write PORTBBits to PORTB_REG (Error Code: %d)\n", ret);
    }
    timer_delay_ms(1000);

    // Turn on Yellow (Red + Green)
    portBBits.RED_PIN = 1;
    portBBits.GREEN_PIN = 1;
    portBBits.BLUE_PIN = 0;
    ret = reg_clear_and_write_bits(PORTB_REG, &portBBits, REG_SIZE_8);
    if (ret != WRITE_OK)
    {
        log_warning("Failed to write PORTBBits to PORTB_REG (Error Code: %d)\n", ret);
    }
    timer_delay_ms(1000);

    // Turn on Cyan (Green + Blue)
    portBBits.RED_PIN = 0;
    portBBits.GREEN_PIN = 1;
    portBBits.BLUE_PIN = 1;
    ret = reg_clear_and_write_bits(PORTB_REG, &portBBits, REG_SIZE_8);
    if (ret != WRITE_OK)
    {
        log_warning("Failed to write PORTBBits to PORTB_REG (Error Code: %d)\n", ret);
    }
    timer_delay_ms(1000);

    // Turn on Magenta (Red + Blue)
    portBBits.RED_PIN = 1;
    portBBits.GREEN_PIN = 0;
    portBBits.BLUE_PIN = 1;
    ret = reg_clear_and_write_bits(PORTB_REG, &portBBits, REG_SIZE_8);
    if (ret != WRITE_OK)
    {
        log_warning("Failed to write PORTBBits to PORTB_REG (Error Code: %d)\n", ret);
    }
    timer_delay_ms(1000);

    // Turn on White (Red + Green + Blue)
    portBBits.RED_PIN = 1;
    portBBits.GREEN_PIN = 1;
    portBBits.BLUE_PIN = 1;
    ret = reg_clear_and_write_bits(PORTB_REG, &portBBits, REG_SIZE_8);
    if (ret != WRITE_OK)
    {
        log_warning("Failed to write PORTBBits to PORTB_REG (Error Code: %d)\n", ret);
    }
    timer_delay_ms(1000);

    // Turn off all colors
    portBBits.RED_PIN = 0;
    portBBits.GREEN_PIN = 0;
    portBBits.BLUE_PIN = 0;
    ret = reg_clear_and_write_bits(PORTB_REG, &portBBits, REG_SIZE_8);
    if (ret != WRITE_OK)
    {
        log_warning("Failed to write PORTBBits to PORTB_REG (Error Code: %d)\n", ret);
    }
    timer_delay_ms(1000);
}
