/***********************************************************************
* Hardware Abstraction for AVR Microcontrollers UART driver            *
* @author Kevin Harper                                                 *
* @date July 13, 2023                                                  *
* Purpose: Provide an easy interface to deal with serial terminal      *
***********************************************************************/

#ifndef UART_H_
#define UART_H_

#include "hw_uart.h"

//typedef enum bool {FALSE, TRUE} uint8_t;
#include <stdbool.h>

#define UBRR_VALUE (((F_CPU / (16UL * (UART_BAUD))) - 1UL))
// why the hell does this work?
#define round(x) (x - (unsigned long)(x)>=0?(unsigned long)((x)+1):(unsigned long)((x)-1))
#define UBRR_VALUE_ROUNDED_NEAREST round(UBRR_VALUE)
//#define UBRR_VALUE_ROUNDED_NEAREST 8.1
#define UBRRH_VALUE ((unsigned long)UBRR_VALUE_ROUNDED_NEAREST >> 8)
#define UBRRL_VALUE ((unsigned long)UBRR_VALUE_ROUNDED_NEAREST)

// Change return types to an error type in lieu of "void"...?

void UART_Init(UART_Dir_t dir, bool int_en, UART_Parity_t par);
bool UART_Check_Flag(UART_Flag_t flag);
void decToASCII(uint8_t buffer[], uint8_t decimal);
uint8_t UART_Read_Byte();
void UART_Read_String();
void UART_Transmit_Byte(unsigned char data);
void UART_Transmit_String(unsigned char* str);
void UART_Transmit_NL(int num, bool cr);

#endif //UART_H_