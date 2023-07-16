/***********************************************************************
* Hardware Abstraction for AVR Microcontrollers TWI Driver            *
* @author Ahmed Elzoughby                                              *
* @date July 26, 2017                                                  *
* Purpose: Provide an easy interface to deal with digital I/O devices  *
* 																	   *
* Adapated by Kevin Harper for the ATMega328P 07/13/2023			   *
***********************************************************************/

#include "twi.h"

// Setup clock using necessary registers and values
// Add in option to enable interrupts
twi_error_t TWI_Master_Init(twi_mode_t twi_mode, bool PUE) {
    // Set PSC value with TWSR bits 1:0
    static return_val = SUCCESS;
    switch(SCL_PSC) {
        case(1UL):
            hw_bit_write(TWSR, TWPS1, 0);
            hw_bit_write(TWSR, TWPS0, 0);
            break;
        case(4UL):
            hw_bit_write(TWSR, TWPS1, 0);
            hw_bit_write(TWSR, TWPS0, 1);
            break;
        case(16UL):
            hw_bit_write(TWSR, TWPS1, 1);
            hw_bit_write(TWSR, TWPS0, 0);
            break;
        case(64UL):
            hw_bit_write(TWSR, TWPS1, 1);
            hw_bit_write(TWSR, TWPS0, 1);
            break;
        default:
            return_val = INVALID_PSC;
    }
    if (PUE && (return_val != INVALID_PSC)) {
        hw_bit_write(BASE_C + OFFSET_DIR, 4, 1);
        hw_bit_write(BASE_C + OFFSET_DIR, 5, 1);
        // Optionally enable internal pullups for the SDA/SCL pins (PC4/PC5) 
        // by writing 1 to the corresponding PORT bits when DDR set to input
        // for said bits
        hw_bit_write(BASE_C + OFFSET_PORT, 4, 1);
        hw_bit_write(BASE_C + OFFSET_PORT, 5, 1);
    }
    if (return_val != INVALID_PSC) {
        // TWAR Not needed by master modes
        hw_reg8_write(TWBR, TWBR_VALUE);
        //hw_reg8_write(TWBR, 0x01);
    }

    return return_val;
}

// Send TWI start condition
twi_error_t TWI_Start(twi_mode_t twi_mode) {
    // Enable TWI interface in MCU via TWI control register
    hw_bit_write(TWCR, TWEN, 1);
    // Clear the TWINT flag by writing a one to the corresponding bit in
    // the TWCR 
    hw_bit_write(TWCR, TWINT, 1);
    // Set TWSTA bit to send start condition
    hw_bit_write(TWCR, TWSTA, 1);

    // Wait for start condition to be transmitted
    while(!(hw_bit_read(TWCR, TWINT)));
    // Read TWI status (5 most significant bits in TWSR) and ensure it indicates
    // that a start condition has been transmitted (see pg. 186 table 21-3)
    while((hw_reg8_read(TWSR) & 0xF8) != 0x08);

    return SUCCESS;
    
}

twi_error_t TWI_Read_Slave_Address(uint8_t data) {
    // Put 7 bit slave address and direction control bit into TWDR
    hw_reg8_write(TWDR, /*((0x3C << 1) | (0x00))*/data);
    // Clear TWINT Flag
    hw_bit_write(TWCR, TWINT, 1);
    // Wait for a complete data byte to be received
    while(!(hw_bit_read(TWCR, TWINT)));
    // Check TWI status code for acknowledgement
    while((hw_reg8_read(TWSR) /*& 0xF8*/) != 0x18);


    return SUCCESS;
}

twi_error_t TWI_Write_Data(uint8_t data) {
    // Write data to be transmitted to TWDR
    hw_reg8_write(TWDR, 0xFF);
    // Clear TWINT Flag
    hw_bit_write(TWCR, TWINT, 1);
    // Wait for a complete data byte to be received
    while(!(hw_bit_read(TWCR, TWINT)));
    // Check TWI status code for acknowledgement
    while((hw_reg8_read(TWSR) /*& 0xF8*/) != 0x28);
    
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