/***********************************************************************
* Hardware Abstraction for AVR Microcontrollers UART driver            *
* @author Kevin Harper                                                 *
* @date July 13, 2023                                                  *
* Purpose: Provide an easy interface to deal with serial terminal      *
***********************************************************************/

#ifndef TWI_H_
#define TWI_H_

#include "hw_twi.h"

typedef enum twi_error {
	SUCCESS,
	ERROR,
	INVALID_PSC,
} twi_error_t;

typedef enum twi_modes {
	MASTER_TX,
	MASTER_RX,
	SLAVE_TX,
	SLAVE_RX,
} twi_mode_t;

//Controls clock frequency for I2C in master mode
#define TWBR_VALUE ((F_CPU/F_SCL - 16UL)/(2UL))
//#define TWBR_VALUE  round_(TWBR_VALUE_FLOAT) //round macro is defined in utility.h

uint8_t TWI_Get_Status();
void I2C_Master_Init(bool PUE);
void I2C_Start();
void I2C_Write(uint8_t data);
void I2C_Stop();

#endif //TWI_H_