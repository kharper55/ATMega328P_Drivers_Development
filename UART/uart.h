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
#define UBRR_VALUE_ROUNDED_NEAREST round_(UBRR_VALUE) //round is included in utility.h
#define UBRRH_VALUE ((unsigned long)UBRR_VALUE_ROUNDED_NEAREST >> 8)
#define UBRRL_VALUE ((unsigned long)UBRR_VALUE_ROUNDED_NEAREST)

// Change return types to an error type in lieu of "void"...?

void UART_Init(UART_Dir_t dir, bool int_en, UART_Parity_t par);
bool UART_Check_Flag(UART_Flag_t flag);         /*to be used to check flags
                                                without need for using 
                                                register access inlines*/
void decToASCII(uint8_t buffer[], uint8_t decimal);
uint8_t UART_Read_Byte(); //stub for future fxn
void UART_Read_String();  //stub for future fxn
void UART_Transmit_Byte(unsigned char data);
void UART_Transmit_String(unsigned char* str);
void UART_Transmit_NL(int num, bool cr);

#endif //UART_H_