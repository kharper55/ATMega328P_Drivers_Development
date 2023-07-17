/***********************************************************************
* Hardware Abstraction for AVR Microcontrollers UART driver            *
* @author Kevin Harper                                                 *
* @date July 13, 2023                                                  *
* Purpose: Provide an easy interface to deal with serial terminal      *
***********************************************************************/

#ifndef I2C_H_
#define I2C_H_

#include "hw_i2c.h"

// 1mS timeout for 16MHz clock
#define TWI_TIMEOUT 1600

/* Master Transmitter */
/*start condition transmitted */
#define TWI_START		    0x08
/*repeated start condition transmitted */
#define TWI_RSTART		    0x10
/*SLA+W transmitted, ACK received */
#define TWIT_ADDR_ACK		0x18
/*SLA+W transmitted, NACK received */
#define TWIT_ADDR_NACK		0x20
/*data transmitted, ACK received */
#define TWIT_DATA_ACK		0x28
/*data transmitted, NACK received */
#define TWIT_DATA_NACK		0x30

/* Master Receiver */

/*SLA+R transmitted, ACK received */
#define TWIR_ADDR_ACK		0x40
/*SLA+R transmitted, NACK received */
#define TWIR_ADDR_NACK		0x48
/*data received, ACK returned */
#define TWIR_DATA_ACK		0x50
/*data received, NACK returned */
#define TWIR_DATA_NACK		0x58

/*arbitration lost in SLA+W or data */
#define TWI_ERROR    		0x38
/* no code */
#define TWI_NONE			0xF8

//Controls clock frequency for I2C in master mode
#define TWBR_VALUE ((F_CPU/F_SCL - 16UL)/(2UL))

enum {
	TWI_OK,
	TWI_ERROR_START,
	TWI_ERROR_RSTART,
	TWI_NACK
};

void I2C_Init(bool PUE);
uint8_t I2C_Read(uint8_t addr, uint8_t reg, uint8_t* data, uint16_t len);
uint8_t I2C_Write(uint8_t addr, uint8_t reg, uint8_t* data, uint16_t len);

#endif //I2C_H_