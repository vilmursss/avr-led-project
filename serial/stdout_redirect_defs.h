#ifndef SERIAL_STDOUT_REDIRECT_DEFS_H
#define SERIAL_STDOUT_REDIRECT_DEFS_H

#include <stdint.h>
#include <stdio.h>

typedef struct {
    uint8_t baudrateHigh : 4; // Bits 0-3: Baud rate high
    uint8_t reserved     : 4; // Bits 4-7: Reserved (unused)
} UBRR0HBits;

typedef struct {
    uint8_t baudrateLow : 8; // Bits 0-7: Baud rate low
} UBRR0LBits;

typedef struct {
    uint8_t multiProcessorCommunicationMode : 1; // Bit 0: Multi-processor Communication Mode
    uint8_t doubleTransmissionSpeed         : 1; // Bit 1: Double the USART Transmission Speed
    uint8_t parityError                     : 1; // Bit 2: Parity Error
    uint8_t dataOverRun                     : 1; // Bit 3: Data OverRun
    uint8_t frameError                      : 1; // Bit 4: Frame Error
    uint8_t dataRegisterEmpty               : 1; // Bit 5: USART Data Register Empty
    uint8_t transmitComplete                : 1; // Bit 6: USART Transmit Complete
    uint8_t receiveComplete                 : 1; // Bit 7: USART Receive Complete
} UCSR0ABits;

typedef struct {
    uint8_t transmitData                : 1; // Bit 0: Transmit Data Bit 8
    uint8_t receiveData                 : 1; // Bit 1: Receive Data Bit 8
    uint8_t charactersize               : 1; // Bit 2: Character Size Bit 2
    uint8_t enableTransmitter           : 1; // Bit 3: Transmitter Enable
    uint8_t receiverEnable              : 1; // Bit 4: Receiver Enable
    uint8_t dataRegisterInterruptEnable : 1; // Bit 5: USART Data Register Empty Interrupt Enable
    uint8_t txInterruptEnable           : 1; // Bit 6: TX Complete Interrupt Enable
    uint8_t rxInterruptEnable           : 1; // Bit 7: RX Complete Interrupt Enable
} UCSR0BBits;

typedef struct {
    uint8_t clockPolarity    : 1; // Bit 0: Clock Polarity
    uint8_t characterSize0   : 1; // Bit 1: Character Size Bit 0
    uint8_t characterSize1   : 1; // Bit 2: Character Size Bit 1
    uint8_t stopBitSelect    : 1; // Bit 3: Stop Bit Select
    uint8_t parityMode0      : 1; // Bit 4: Parity Mode Bit 0
    uint8_t parityMode1      : 1; // Bit 5: Parity Mode Bit 1
    uint8_t usartModeSelect0 : 1; // Bit 6: USART Mode Select Bit 0
    uint8_t usartModeSelect1 : 1; // Bit 7: USART Mode Select Bit 1
} UCSR0CBits;

#endif // SERIAL_STDOUT_REDIRECT_DEFS_H