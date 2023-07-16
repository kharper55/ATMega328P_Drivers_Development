/***********************************************************************
* Hardware Abstraction for AVR Microcontrollers TWI Driver            *
* @author Ahmed Elzoughby                                              *
* @date July 26, 2017                                                  *
* Purpose: Provide an easy interface to deal with digital I/O devices  *
* 																	   *
* Adapated by Kevin Harper for the ATMega328P 07/13/2023			   *
***********************************************************************/

#include "twi.h"

uint8_t TWI_Get_Status() {
	uint8_t status;
	//mask 3 lower bits of the register
	status = hw_reg8_read(TWSR) & 0xF8;
	return status;
}

// Setup clock by setting bit rate and prescalar bits
// Add in option to enable interrupts
void I2C_Master_Init(bool PUE) {
    // Set PSC value with TWSR bits 1:0
    /*No PSC (div by 1)*/
    hw_bit_write(TWSR, TWPS1, 0);
    hw_bit_write(TWSR, TWPS0, 0);
        
    if (PUE) {
        // Enable pullups by removing the 1 bit from PUD 
        hw_bit_write(MCUCR, PUD, 0);
        hw_bit_write(BASE_C + OFFSET_DIR, 4, 0);
        hw_bit_write(BASE_C + OFFSET_DIR, 5, 0);
        // Optionally enable internal pullups for the SDA/SCL pins (PC4/PC5) 
        // by writing 1 to the corresponding PORT bits when DDR set to input
        // for said bits
        hw_bit_write(BASE_C + OFFSET_PORT, 4, 1);
        hw_bit_write(BASE_C + OFFSET_PORT, 5, 1);
    }
    
    //400khz clock
    hw_reg8_write(TWBR, TWBR_VALUE);
    // Enable TWI interface in MCU via TWI control register
    hw_bit_write(TWCR, TWEN, 1);
}

void I2C_Start() {
    //TWCR = ((1<<TWINT) | (1<<TWSTA) | (1<<TWEN));
    hw_bit_write(TWCR, TWSTA, 1);
    hw_bit_write(TWCR, TWEN, 1);
    hw_bit_write(TWCR, TWINT, 1);

    while (!(hw_bit_read(TWCR, TWINT)));
}

void I2C_Write(uint8_t data) {
    hw_reg8_write(TWDR, data);
    hw_bit_write(TWCR, TWEN, 1);
    hw_bit_write(TWCR, TWINT, 1);

    while(!(hw_bit_read(TWCR, TWINT)));

}

void I2C_Stop() {
    hw_bit_write(TWCR, TWINT, 1);
    hw_bit_write(TWCR, TWSTO, 1);
    hw_bit_write(TWCR, TWEN, 1);
}

twi_error_t TWI_Write_Slave_Address(uint8_t data) {
    // Put 7 bit slave address and direction control bit into TWDR (0 LSB)
    hw_reg8_write(TWDR, data << 1);
    // Clear TWINT Flags
    hw_bit_write(TWCR, TWINT, 1);
    //hw_bit_write(TWCR, TWEN, 1);
    // Wait for a complete data byte to be received
    while(!(hw_bit_read(TWCR, TWINT)));
    // Check TWI status code for acknowledgement
    while(TWI_Get_Status() != 0x18);

    return SUCCESS;
}

twi_error_t TWI_Write_Data(uint8_t data) {
    // Write data to be transmitted to TWDR
    hw_reg8_write(TWDR, data);
    // Clear TWINT Flag
    hw_bit_write(TWCR, TWINT, 1);
    hw_bit_write(TWCR, TWEN, 1);
    // Wait for a complete data byte to be received
    while(!(hw_bit_read(TWCR, TWINT)));
    // Check TWI status code for acknowledgement
    UART_Transmit_String((unsigned char*)"Waiting for ACK from slave");
	UART_Transmit_NL(1, false);
    while(TWI_Get_Status() != 0x28);
    
    return SUCCESS;
}

twi_error_t TWI_Stop() {
    // Clear TWINT Flag
    hw_bit_write(TWCR, TWINT, 1);
    // Put STOP condition on SDA
    hw_bit_write(TWCR, TWSTO, 1);
    // Wait for STOP condition to be transmitted
    while(!(hw_bit_read(TWCR, TWSTO)));
    
    return SUCCESS;
}