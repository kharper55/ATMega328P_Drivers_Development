/***********************************************************************
* Register Abstraction for AVR Microcontrollers GPIO Driver            *
* @author Ahmed Elzoughby                                              *
* @date July 26, 2017                                                  *
* Purpose: Provide an easy interface to deal with digital I/O devices  *
*                                                                      *
* Adapated by Kevin Harper for the ATMega328P 07/13/2023			   *
***********************************************************************/

#ifndef HW_GPIO_H_
#define HW_GPIO_H_

#define BASE_B 0x23
#define BASE_C 0x26
#define BASE_D 0x29

#define OFFSET_PIN  0x00
#define OFFSET_DIR  0x01
#define OFFSET_PORT 0x02

#define MCUCR 0x55
#define PUD 4       // Pull-up disable bit in MCUCR (only writeable bit)

#endif //HW_GPIO_H_
