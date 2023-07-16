/***********************************************************************
* Hardware Abstraction for AVR Microcontrollers TWI Driver            *
* @author Ahmed Elzoughby                                              *
* @date July 26, 2017                                                  *
* Purpose: Provide an easy interface to deal with digital I/O devices  *
* 																	   *
* Adapated by Kevin Harper for the ATMega328P 07/13/2023			   *
***********************************************************************/

#include "twi.h"

twi_error_t TWI_Master_Init(bool PUE) {
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
    if (!(return_val == INVALID_PSC)) {
        // TWAR Not needed by master modes
        //hw_reg8_write(TWBR, TWBR_VALUE);
        hw_reg8_write(TWBR, 0x01);
        // Enable TWI interface in MCU via TWI control register
        hw_bit_write(TWCR, TWEN, 1);
    }
    if (PUE) {
        hw_bit_write(BASE_C + OFFSET_DIR, 4, 1);
        hw_bit_write(BASE_C + OFFSET_DIR, 5, 1);
        // Optionally enable internal pullups for the SDA/SCL pins (PC4/PC5) 
        // by writing 1 to the corresponding PORT bits when DDR set to input
        // for said bits
        hw_bit_write(BASE_C + OFFSET_PORT, 4, 1);
        hw_bit_write(BASE_C + OFFSET_PORT, 5, 1);
    }
    return return_val;
}