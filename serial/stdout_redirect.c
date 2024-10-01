#include "stdout_redirect.h"
#include "stdout_redirect_defs.h"

#include <container/vector.h>
#include <log/logger.h>
#include <writer/reg_writer.h>

#include <stdbool.h>
#include <string.h>

// Define the clock frequency if not already defined
#ifndef F_CPU
    #define F_CPU 16000000UL // 16 MHz
#endif

// Local functions
static bool stdout_redirect_data_register_empty();
static void stdout_redirect_send_char(const char c);
static void stdout_redirect_transmit_data();
static void stdout_redirect_setup_output_file();
static int stdout_redirect_putchar(char c, FILE* stream);
static int stdout_redirect_set_baud_rate_regs(uint32_t baud);
static int stdout_redirect_enable_transmitter();
static int stdout_set_frame_format();

// Constants
static const uint32_t PRESCALER = 16;
static const uint8_t FILE_IO_FLAGS = 0x0002;

// Serial output file
static FILE stdout_redirect_output;

// Registers
volatile uint8_t* const UDR0_REG   = (volatile uint8_t*)0xC6; // USART Data Register 0
volatile uint8_t* const UBRR0H_REG = (volatile uint8_t*)0xC5; // USART Baud rate Register High
volatile uint8_t* const UBRR0L_REG = (volatile uint8_t*)0xC4; // USART Baud rate Register Low
volatile uint8_t* const UCSR0C_REG = (volatile uint8_t*)0xC2; // USART Control and Status Register C
volatile uint8_t* const UCSR0B_REG = (volatile uint8_t*)0xC1; // USART Control and Status Register B
volatile uint8_t* const UCSR0A_REG = (volatile uint8_t*)0xC0; // USART Control and Status Register A

// Locals
static Vector data_buffer;

void stdout_redirect_init(const uint32_t baud)
{
    if (stdout_redirect_set_baud_rate_regs(baud) != 0)
    {
        log_error("Failed to set baud rate registers");
        return;
    }

    if (stdout_redirect_enable_transmitter() != 0)
    {
        log_error("Failed to enable transmitter");
        return;
    }

    if (stdout_set_frame_format() != 0)
    {
        log_error("Failed to set frame format");
        return;
    }

    // Initialize data buffer
    vector_init(&data_buffer, sizeof(char));

    // Setup stdout to serial output
    stdout_redirect_setup_output_file();
}

// Local functions
static bool stdout_redirect_data_register_empty()
{
    // Create a pointer to the UCSR0A register
    volatile UCSR0ABits* ucsr0a = (volatile UCSR0ABits*)UCSR0A_REG;

    // Check if the data register is empty
    return ucsr0a->dataRegisterEmpty;
}

static void stdout_redirect_send_char(const char c)
{
    // Wait untill data register is empty
    while (!stdout_redirect_data_register_empty());

    // Send character to serial
    *(volatile uint8_t*)UDR0_REG = c;
}

static void stdout_redirect_transmit_data()
{
    while (VECTOR_SIZE(data_buffer) > 0)
    {
        char* value = VECTOR_POP_FRONT(data_buffer, char);
        if (value != NULL)
        {
            stdout_redirect_send_char(*value);
        }
    }
}

static void stdout_redirect_setup_output_file()
{
    fdev_setup_stream(&stdout_redirect_output, stdout_redirect_putchar, NULL, FILE_IO_FLAGS);
    stdout = &stdout_redirect_output;
}

static int stdout_redirect_putchar(char c, FILE* stream)
{
    int funcRet = 0;
    uint64_t vec_size = VECTOR_SIZE(data_buffer);

    // Store character in buffer
    // Leave room for possible new line and carriage return
    if (vec_size < MAX_VEC_ELEMENT_SIZE - 2)
    {
        const VecStatus ret = VECTOR_PUSH_BACK(data_buffer, c);
        if (ret != VEC_OK)
        {
            log_error("Failed to write '%c' to data buffer. (Error Code: %d)", c, ret);
            funcRet = -1;
        }
        else
        {
            vec_size++;
        }
    }

    char newline = '\n';
    if (vec_size == MAX_VEC_ELEMENT_SIZE - 2 && c != newline)
    {
        const VecStatus ret = VECTOR_PUSH_BACK(data_buffer, newline);
        if (ret != VEC_OK)
        {
            VECTOR_CLEAR(data_buffer);
            log_warning("Failed to push newline to data buffer. (Error Code: %d)", ret);
            funcRet = -1;
        }
    }

    if (VECTOR_GET(data_buffer, char, (vec_size - 1)) == newline)
    {
        char carriage = '\r';
        const VecStatus ret = VECTOR_PUSH_BACK(data_buffer, carriage);
        if (ret != VEC_OK)
        {
            VECTOR_CLEAR(data_buffer);
            log_warning("Failed to push carrage return to data buffer. (Error Code: %d)", ret);
            funcRet = -1;
        }
        else
        {
            // Transmit message to serial
            stdout_redirect_transmit_data();

            // Clear vector after data has been transmitted just in case
            VECTOR_CLEAR(data_buffer);
        }
    }

    return funcRet;
}

static int stdout_redirect_set_baud_rate_regs(const uint32_t baud)
{
    uint32_t ubrr = (F_CPU / PRESCALER / baud) - 1;

    if (ubrr > 0xFFF)
    {
        log_warning("UBRR value (%d) is out of range.\n", ubrr);
        return -1;
    }

    // Set high bits
    UBRR0HBits ubbr0h = {0};
    ubbr0h.baudrateHigh = (uint8_t)(ubrr >> 8);

    WriteStatus ret = reg_write_bits(UBRR0H_REG, &ubbr0h, REG_SIZE_8);
    if (ret != WRITE_OK)
    {
        log_warning("Failed to write UBRR0HBits to UBRR0H_REG (Error Code: %d)", ret);
        return -1;
    }

    // Set low bits
    UBRR0LBits ubbr0l = {0};
    ubbr0l.baudrateLow = (uint8_t)ubrr;

    ret = reg_write_bits(UBRR0L_REG, &ubbr0l, REG_SIZE_8);
    if (ret != WRITE_OK)
    {
        log_warning("Failed to write UBRR0LBits to UBRR0L_REG (Error Code: %d)", ret);
        return -1;
    }

    return 0;
}

static int stdout_redirect_enable_transmitter()
{
    // Enable transmitter
    UCSR0BBits ucsr0b = {0};
    ucsr0b.enableTransmitter = 1;

    const WriteStatus ret = reg_write_bits(UCSR0B_REG, &ucsr0b, REG_SIZE_8);
    if (ret != WRITE_OK)
    {
        log_warning("Failed to write UCSR0BBits to UCSR0B_REG (Error Code: %d)", ret);
        return -1;
    }

    return 0;
}

static int stdout_set_frame_format()
{
    // Set frame format: 8 data bits, 1 stop bit
    UCSR0CBits ucsr0c = {0};
    ucsr0c.characterSize0 = 1;
    ucsr0c.characterSize1 = 1;

    const WriteStatus ret = reg_write_bits(UCSR0C_REG, &ucsr0c, REG_SIZE_8);
    if (ret != WRITE_OK)
    {
        log_warning("Failed to write UCSR0CBits to UCSR0C_REG (Error Code: %d)", ret);
        return -1;
    }

    return 0; 
}