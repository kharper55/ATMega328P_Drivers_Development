/***********************************************************************
* Hardware Abstraction for AVR Microcontrollers USART driver           *
* @author Kevin Harper                                                 *
* @date July 13, 2023                                                  *
* Purpose: Provide an easy interface to deal with communication b/w    *
*          I2C devices                                                 *
***********************************************************************/

#ifndef HW_TWI_H_
#define HW_TWI_H_

#include "twi_types.h"

#define TWBR  0xB8  // Two wire serial interface bit rate register
#define TWSR  0xB9  // Two wire serial interface status register
#define TWAR  0xBA  // Two wire serial interface (slave) address register
#define TWDR  0xBB  // Two wire serial interface data register
#define TWCR  0xBC  // Two wire serial interface control register
#define TWAMR 0xBD  // Two wire serial interface address mask register

// Pin definitions taken from avr gnu toolchain avr/include/avr/iom328p.h

//TWBR
#define TWBR0 0
#define TWBR1 1
#define TWBR2 2
#define TWBR3 3
#define TWBR4 4
#define TWBR5 5
#define TWBR6 6
#define TWBR7 7

//TWSR
#define TWPS0 0
#define TWPS1 1
#define TWS3 3
#define TWS4 4
#define TWS5 5
#define TWS6 6
#define TWS7 7

//TWAR
#define TWGCE 0
#define TWA0 1
#define TWA1 2
#define TWA2 3
#define TWA3 4
#define TWA4 5
#define TWA5 6
#define TWA6 7

//TWDR
#define TWD0 0
#define TWD1 1
#define TWD2 2
#define TWD3 3
#define TWD4 4
#define TWD5 5
#define TWD6 6
#define TWD7 7

//TWCR
#define TWIE 0
#define TWEN 2
#define TWWC 3
#define TWSTO 4
#define TWSTA 5
#define TWEA 6
#define TWINT 7

//TWAMR
#define TWAM0 0
#define TWAM1 1
#define TWAM2 2
#define TWAM3 3
#define TWAM4 4
#define TWAM5 5
#define TWAM6 6

// MCU Power reduction register initialized to all 0's, so TWI en by def
#define PRR 0x64
#define PRTWI 7

#endif //HW_TWI_H_