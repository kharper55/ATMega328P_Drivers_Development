/***********************************************************************
* Hardware Abstraction for AVR Microcontrollers TWI Driver            *
* @author Ahmed Elzoughby                                              *
* @date July 26, 2017                                                  *
* Purpose: Provide an easy interface to deal with digital I/O devices  *
* 																	   *
* Adapated by Kevin Harper for the ATMega328P 07/13/2023			   *
***********************************************************************/

#include "i2c.h"

volatile uint8_t status = 0xF8;

ISR(TWI_vect) {
    // Return 5 bit status code with appropriate mask of 3 LSBs
    status = hw_reg8_read(TWSR) & 0xF8;
}

// Setup clock by setting bit rate and prescalar bits
// Add in option to enable interrupts
void I2C_Init(bool PUE) {
    // Set bit rate value in TWBR register
    hw_reg8_write(TWBR, TWBR_VALUE);
    // Set PSC value with TWSR bits 1:0
    /*No PSC (div by 1)*/
    hw_bit_write(TWSR, TWPS1, 0);
    hw_bit_write(TWSR, TWPS0, 0);
        
    if (PUE) {
        // Optionally enable internal pullups for the SDA/SCL pins (PC4/PC5) 
        // by writing 1 to the corresponding PORT bits when DDR set to input
        // for said bits
        hw_bit_write(BASE_C + OFFSET_PORT, 4, 1);
        hw_bit_write(BASE_C + OFFSET_PORT, 5, 1);
    }
    // Enable TWI interface in MCU via TWI control register
    hw_bit_write(TWCR, TWEN, 1);
    hw_bit_write(TWCR, TWIE, 1);
}

static uint8_t I2C_Start() {

    uint16_t i = 0;

    hw_bit_write(TWCR, TWSTA, 1);
    hw_bit_write(TWCR, TWEN, 1);
    hw_bit_write(TWCR, TWIE, 1);
    hw_bit_write(TWCR, TWINT, 1);

    while(status != TWI_START) {
        i++;
        // If we have to wait too long, break out
        if (i >= TWI_TIMEOUT) {
            return TWI_ERROR_START;
        }
    }

    return TWI_OK;

}

static uint8_t I2C_Restart() {

    uint16_t i = 0;

    hw_bit_write(TWCR, TWSTA, 1);
    hw_bit_write(TWCR, TWEN, 1);
    hw_bit_write(TWCR, TWIE, 1);
    hw_bit_write(TWCR, TWINT, 1);

    while(status != TWI_RSTART) {
        i++;
        // If we have to wait too long, break out
        if (i >= TWI_TIMEOUT) {
            return TWI_ERROR_RSTART;
        }
    }

    return TWI_OK;

}

static uint8_t I2C_Addr_Write_ACK() {

    uint16_t i = 0;

    hw_bit_write(TWCR, TWEN, 1);
    hw_bit_write(TWCR, TWIE, 1);
    hw_bit_write(TWCR, TWINT, 1);

    while(status != TWIT_ADDR_ACK) {
        i++;
        // If we have to wait too long, break out
        if (i >= TWI_TIMEOUT) {
            return TWI_NACK;
        }
    }

    return TWI_OK;

}

static uint8_t I2C_Data_Write_ACK() {

    uint16_t i = 0;

    hw_bit_write(TWCR, TWEN, 1);
    hw_bit_write(TWCR, TWIE, 1);
    hw_bit_write(TWCR, TWINT, 1);

    while(status != TWIT_DATA_ACK) {
        i++;
        // If we have to wait too long, break out
        if (i >= TWI_TIMEOUT) {
            return TWI_NACK;
        }
    }

    return TWI_OK;

}

static uint8_t I2C_Addr_Read_ACK() {

    uint16_t i = 0;

    hw_bit_write(TWCR, TWEN, 1);
    hw_bit_write(TWCR, TWIE, 1);
    hw_bit_write(TWCR, TWINT, 1);

    while(status != TWIR_ADDR_ACK) {
        i++;
        // If we have to wait too long, break out
        if (i >= TWI_TIMEOUT) {
            return TWI_NACK;
        }
    }

    return TWI_OK;

}

static uint8_t I2C_Data_Read_ACK(uint8_t ack) {

    uint16_t i = 0;

    if (ack != 0) {

        hw_bit_write(TWCR, TWEN, 1);
        hw_bit_write(TWCR, TWIE, 1);
        hw_bit_write(TWCR, TWEA, 1);
        hw_bit_write(TWCR, TWINT, 1);

        while(status != TWIR_DATA_ACK) {
            i++;
            // If we have to wait too long, break out
            if (i >= TWI_TIMEOUT) {
                return TWI_NACK;
            }
        }
    }
    else {

        hw_bit_write(TWCR, TWEN, 1);
        hw_bit_write(TWCR, TWIE, 1);
        hw_bit_write(TWCR, TWINT, 1);

        while(status != TWIR_DATA_NACK) {
            i++;
            // If we have to wait too long, break out
            if (i >= TWI_TIMEOUT) {
                return TWI_NACK;
            }
        }

    }
    return TWI_OK;

}

static void I2C_Stop() {

    hw_bit_write(TWCR, TWSTO, 1);
    hw_bit_write(TWCR, TWEN, 1);
    hw_bit_write(TWCR, TWIE, 1);
    hw_bit_write(TWCR, TWINT, 1);

}

uint8_t I2C_Read(uint8_t addr, uint8_t reg, uint8_t* data, uint16_t len) {

    uint16_t i = 0;
    uint8_t err = TWI_OK;

    err = I2C_Start();
    if (err != TWI_OK) {
        I2C_Stop();
        return err;
    }
    hw_reg8_write(TWDR, ((addr << 1) | 0)); // Write address/ID for slave device
                                            // that we wish to talk to

    err = I2C_Addr_Write_ACK();
    if (err != TWI_OK) {
        I2C_Stop();
        return err;
    }
    hw_reg8_write(TWDR, reg); // Write address of register for slave that we wish
                              // to begin writing at

    err = I2C_Data_Write_ACK();
    if (err != TWI_OK) {
        I2C_Stop();
        return err;
    }
    
    err = I2C_Restart();
    if (err != TWI_OK) {
        I2C_Stop();
        return err;
    }

    hw_reg8_write(TWDR, ((addr << 1) | 1)); // SLA+R
    err = I2C_Addr_Read_ACK();
    if (err != TWI_OK) {
        I2C_Stop();
        return err;
    }

    for (i = 0; i < (len - 1); i++) {
        err = I2C_Data_Read_ACK(1);
        if (err != TWI_OK) {
            I2C_Stop();
            return err;
        } 
        data[i] = hw_reg8_read(TWDR);
    }

    err = I2C_Data_Read_ACK(0);
    if (err != TWI_OK) {
        I2C_Stop();
        return err;
    } 
    data[i] = hw_reg8_read(TWDR);

    I2C_Stop();

    return err;

}

uint8_t I2C_Write(uint8_t addr, uint8_t reg, uint8_t* data, uint16_t len) {

    uint16_t i = 0;
    uint8_t err = TWI_OK;

    err = I2C_Start();
    if (err != TWI_OK) {
        I2C_Stop();
        return err;
    }
    hw_reg8_write(TWDR, ((addr << 1) | 0)); // Write address/ID for slave device
                                            // that we wish to talk to

    err = I2C_Addr_Write_ACK();
    if (err != TWI_OK) {
        I2C_Stop();
        return err;
    }
    hw_reg8_write(TWDR, reg); // Write address of register for slave that we wish
                              // to begin writing at

    err = I2C_Data_Write_ACK();
    if (err != TWI_OK) {
        I2C_Stop();
        return err;
    }

    for (i = 0; i < len; i++) {
        hw_reg8_write(TWDR, data[i]); 
        err = I2C_Data_Write_ACK(1);
        if (err != TWI_OK) {
            I2C_Stop();
            return err;
        } 
    }

    I2C_Stop();

    return err;

}
