/***********************************************************************
* Hardware Abstraction for AVR Microcontrollers USART driver           *
* @author Kevin Harper                                                 *
* @date July 13, 2023                                                  *
* Purpose: Provide an easy interface to deal with serial terminal      *
***********************************************************************/

#ifndef HW_UART_H_
#define HW_UART_H_

#include "uart_types.h"

// Ports
#define USCR0A 0xC0
#define USCR0B 0xC1
#define USCR0C 0xC2
#define UBRR0L 0xC4
#define UBRR0H 0xC5
#define UDR0   0xC6

// Pin definitions taken from avr gnu toolchain avr/include/avr/iom328p.h

//USCR0A
#define MPCM0 0
#define U2X0 1
#define UPE0 2
#define DOR0 3
#define FE0 4
#define UDRE0 5
#define TXC0 6
#define RXC0 7

//USCR0B
#define TXB80 0
#define RXB80 1
#define UCSZ02 2
#define TXEN0 3
#define RXEN0 4
#define UDRIE0 5
#define TXCIE0 6
#define RXCIE0 7

//USCR0C
#define UCPOL0 0
#define UCSZ00 1
#define UCSZ01 2
#define USBS0 3
#define UPM00 4
#define UPM01 5
#define UMSEL00 6
#define UMSEL01 7

#endif //HW_UART_H_