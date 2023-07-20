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
#include <string.h>
#include <stdio.h>

// Need for BR calculation inside uart.h
#define UART_BAUD 115200UL

#include <UART/uart.c>

#define F_SCL 400000UL

#include <avr/interrupt.h>
//#include <I2C/i2c.c>
#include <twi/twi_hal.c>

#define RTC_ADDR 0x68 /*Address for DS3231 RTC clock*/

uint8_t numBits = 6;
uint8_t flagCount = 0;
uint8_t i;					// bad name for a modular program
uint8_t maxBitPos = 7;

uint8_t buff[2];

uint8_t print_buffer[24];

int main(void) {

	UART_Init(TX, false, NONE);
	gpio_port_init(BASE_B, DIR_OUTPUT);

	//I2C_Init(false);
	twi_init(F_SCL, false);
	UART_Transmit_String((unsigned char*)"I2C bit rate and SCL initialized");
	UART_Transmit_NL(1, false);

	// BCD encoded, so hex values == decimal
	uint8_t rtc_data[7] = {0x50, 0x46, 0x20, 0x07, 0x16, 0x07, 0x23};

	sei(); /*Enable interrupts, necessary for I2C*/

	UART_Transmit_String((unsigned char*)"Welcome to ATMega328P Driver Dev. by Kevin Harper");
	UART_Transmit_NL(2, false);

	uint8_t err = twi_write(RTC_ADDR,0x00,rtc_data,sizeof(rtc_data));
	if(err != TWI_OK){
		memset(print_buffer,0,sizeof(print_buffer));
		sprintf(print_buffer,"%d error %d\r\n\n",__LINE__,err);
		UART_Transmit_String((uint8_t*)print_buffer);
		while(1);
	}
    
	_delay_ms(1000);

	while(1) {

		err = twi_read(RTC_ADDR,0x00,rtc_data,sizeof(rtc_data));
		if(err != TWI_OK){
			memset(print_buffer,0,sizeof(print_buffer));
			sprintf(print_buffer,"%d error %d\r\n\n",__LINE__,err);
			UART_Transmit_String((uint8_t*)print_buffer);
			while(1);
		}

		memset(print_buffer,0,sizeof(print_buffer));
			sprintf(print_buffer,"\r20%02x/%02x/%02x %02x:%02x:%02x",
			rtc_data[6],
			rtc_data[5],
			rtc_data[4],
			rtc_data[2],
			rtc_data[1],
			rtc_data[0]
			);
		UART_Transmit_String((unsigned char*)print_buffer);


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