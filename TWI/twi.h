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

//Controls clock frequency for I2C in master mode
#define SCL_PSC 16UL
#define TWBR_VALUE_FLOAT ((F_CPU - 16UL)/(2UL*(F_SCL)*(SCL_PSC)))
#define TWBR_VALUE  round_(TWBR_VALUE_FLOAT) //round macro is defined in utility.h

twi_error_t TWI_Master_Init(bool PUE);
void TWI_Slave_Init();

#endif //TWI_H_