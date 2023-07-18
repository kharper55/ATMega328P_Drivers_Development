/***********************************************************************
* Low-Level Hardware Abstraction for AVR Microcontrollers GPIO Driver  *
* @author Ahmed Elzoughby                                              *
* @date July 26, 2017                                                  *
* Purpose: Provide an easy interface to deal with digital I/O devices  *
*																	   *
* Adapted by Kevin Harper for the ATMega328P 07/13/2023			   *
***********************************************************************/

#ifndef HW_TYPES_H_
#define HW_TYPES_H_

#include <GPIO/gpio_types.h>

#define REG_ADDR (*((volatile uint8_t*) reg_addr))

static inline void hw_bit_write(uint8_t reg_addr, uint8_t bit_num, bit_t val) {
	REG_ADDR = val? REG_ADDR | (1 << bit_num) : REG_ADDR & (~(1 << bit_num));
}

static inline bit_t hw_bit_read(uint8_t reg_addr, uint8_t bit_num) {
	return (REG_ADDR & (1 << bit_num))? 1: 0;
}

static inline void hw_reg8_write(uint8_t reg_addr, uint8_t val) {
	REG_ADDR = val;
}

static inline uint8_t hw_reg8_read(uint8_t reg_addr) {
	return REG_ADDR;
}

#endif //HW_TYPES_H_
