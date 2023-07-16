/***********************************************************************
* Application for AVR UART, TWI, GPIO peripherals, etc.                *
* @author Kevin Harper                                                 *
* @date July 12, 2023                                                  *
* Purpose: Show how to use the GPIO driver in the applications         *
*          Debugged with UART thru the onboard CH340 IC for the nano   *
*		   which is home to an ATMega328P                              *
***********************************************************************/

#include "utility.h"  // For utility functions
#include "hw_types.h" // Needed for writing to registers
#include <GPIO/gpio.c>

// Need for BR calculation inside uart.h
#define UART_BAUD 115200UL

#include <UART/uart.c>

#define F_SCL 400000UL

#include <TWI/twi.c>

uint8_t numBits = 6;
uint8_t flagCount = 0;
uint8_t i;					// bad name for a modular program
uint8_t maxBitPos = 7;

uint8_t buff[2];
twi_error_t initSuccess;

int main(void) {

	UART_Init(TX, false, NONE);
	gpio_port_init(BASE_B, DIR_OUTPUT);

	I2C_Master_Init(true);
	UART_Transmit_String((unsigned char*)"I2C bit rate and SCL initialized");
	UART_Transmit_NL(1, false);

	I2C_Start();
	UART_Transmit_String((unsigned char*)"I2C START condition sent");
	UART_Transmit_NL(1, false);
	//while(TWI_Get_Status != 0x08);
	
	I2C_Write(0x68 << 1);
	UART_Transmit_String((unsigned char*)"TWI SLA address packet sent");
	UART_Transmit_NL(1, false);
	while(TWI_Get_Status != 0x18);
	UART_Transmit_String((unsigned char*)"SLA+W has been transmitted, ACK received");
	UART_Transmit_NL(1, false);
	

	//TWI_Write_Data(0xFF);
	I2C_Write(0x00);
	UART_Transmit_String((unsigned char*)"TWI data packet sent");
	UART_Transmit_NL(1, false);
	//while(TWI_Get_Status() != 0x28);
	UART_Transmit_String((unsigned char*)"Data byte has been transmitted, ACK received");
	UART_Transmit_NL(1, false);

	I2C_Stop();

	UART_Transmit_String((unsigned char*)"Welcome to ATMega328P Driver Dev. by Kevin Harper");
	UART_Transmit_NL(2, false);

	_delay_ms(1000);

	while(1) {

		UART_Transmit_String((unsigned char*)"Flashing all bits on port B...");
		UART_Transmit_NL(2, false);

		// Dont flash pinB6 or 7; external oscillator pins
		for (i = 0; i <= maxBitPos - 2; i++) {
			gpio_pin_write(BASE_B, i, HIGH);
			_delay_ms(100);
			gpio_pin_write(BASE_B, i, LOW);
			_delay_ms(100);
			UART_Transmit_Byte(i + 0x30);
			UART_Transmit_String((unsigned char*)" done.");
			UART_Transmit_NL(1, false);
		}

		UART_Transmit_NL(1, false);
		_delay_ms(100);

		//UART_Transmit_String((unsigned char*)"Mackenzie");
		//UART_Transmit_NL(2, false);

		UART_Transmit_String((unsigned char*)"All pins done.");
		UART_Transmit_NL(2, false);
		_delay_ms(100);

		UART_Transmit_String((unsigned char*)"Starting UART Flag Check...");
		UART_Transmit_NL(2, false);

		for (i = maxBitPos; i >= maxBitPos - numBits; i--) {
			// Dont increment for transmit complete or UDRE
			if ((UART_Check_Flag(i) == true) && (!(i == TXC) || (i == UDRE))) flagCount++;
		}

		if (flagCount == 0) {
			UART_Transmit_String((unsigned char*)"No flags found.");
			UART_Transmit_NL(2, false);
		}
		else {
			decToASCII(buff, flagCount);
			UART_Transmit_String((unsigned char*)buff);
			UART_Transmit_String((unsigned char*)" flags found.");
			UART_Transmit_NL(2, false);
		}

		_delay_ms(2000);
	}
}

/* =================================================================================
 *                                    // END FILE //
 * ================================================================================= */