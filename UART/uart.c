/***********************************************************************
* Hardware Abstraction for AVR Microcontrollers UART driver            *
* @author Kevin Harper                                                 *
* @date July 13, 2023                                                  *
* Purpose: Provide an easy interface to deal with serial terminal      *
***********************************************************************/

#include "uart.h"

/************************* UART Utility Stuff *************************/

// This forces asynchronous mode on USART0 with 8-bit data width, 1 stop bit
void UART_Init(UART_Dir_t dir, bool int_en, UART_Parity_t par) {
    // Setup baudrate with integer calculated in uart.h macros
    hw_reg8_write(UBRR0H, UBRRH_VALUE);
    hw_reg8_write(UBRR0L, UBRRL_VALUE);
    // Set UART direction
    switch (dir) {
        case(TX):
            hw_bit_write(USCR0B, TXEN0, 1); 
            break;
        case(RX):
            hw_bit_write(USCR0B, RXEN0, 1); 
            break;
        case(BOTH):
            hw_bit_write(USCR0B, TXEN0, 1); 
            hw_bit_write(USCR0B, RXEN0, 1); 
            break;
        //default: return ERROR?
    }
    // Optionally enable interrupts
    // This requires setting up the vector table and assigning space to store the ISR
    if (int_en) {
        if ((dir == TX) || BOTH) hw_bit_write(USCR0B, TXCIE0, 1);
        else if ((dir == RX) || BOTH) hw_bit_write(USCR0B, RXCIE0, 1);
        hw_bit_write(USCR0B, UDRIE0, 1);
    }
    // Set 8-bit data width, asynchronous mode, 1 stop-bit,
    // with user input parity
    hw_reg8_write(USCR0C, (0x06 | (par << 4)));
}

bool UART_Check_Flag(UART_Flag_t flag) {
    static bool return_val;
    switch(flag) {
        /* Check receive complete flag */
        case(RXC):
            return_val = (hw_bit_read(USCR0A, RXC) == HIGH)?true:false;
            break;
        /* Check transmit complete flag */
        case(TXC):
            return_val = (hw_bit_read(USCR0A, TXC) == HIGH)?true:false;
            break;
        /* Check UART data register empty flag */
        case(UDRE):
            return_val = (hw_bit_read(USCR0A, UDRE) == HIGH)?true:false;
            break;
        /* Check frame error flag */
        case(FE):
            return_val = (hw_bit_read(USCR0A, FE) == HIGH)?true:false;
            break;
        /* Check data overrun flag */
        case(DOR):
            return_val = (hw_bit_read(USCR0A, DOR) == HIGH)?true:false;
            break;
        /* Check parity error flag */
        case(PE):
            return_val = (hw_bit_read(USCR0A, PE) == HIGH)?true:false;
            break;
    }
    return return_val; 
}

// Rewrite this to be used in a loop that increments in base 10's to extract each digit 

// Sub function to convert a decimal represented as an unsigned integer 
// into it's equivalent ASCII character string
void decToASCII(uint8_t buffer[], uint8_t decimal) {
     //If the number to be converted is <= 2 digits wide, we can convert it 
     //(ie 0 -- 99; this is limited by the buffer array and requires deeper logic for larger integers)
     if (!(decimal >= 100) || sizeof(buffer)/sizeof(buffer[0]) > 2) {
          //Check if the number to be converted is smaller than 10
          if (decimal / 10 == 0) {
             //Set ones place equal to remainder of integer division of decimal / 10
             buffer[0] = decimal % 10 + '0';
             buffer[1] = '\0';
          }
          //The number to be converted is between 0 and 99
          else {
             //Set tens place equal to integer division of decimal / 10
             buffer[0] = decimal / 10 + '0';
             //Set ones place equal to remainder of integer division of decimal / 10
             buffer[1] = decimal % 10 + '0';
          }
     }
}

/************************ UART Receive Stuff **************************/

uint8_t UART_Read_Byte() {
    return 0;
}
// Does a function like this even make sense?
void UART_Read_String() {
    return 0;
}

/************************ UART Transmit Stuff *************************/

//  Direct from ATMega328P datasheet pg 150
void UART_Transmit_Byte(unsigned char data) {
    /* Wait for empty transmit buffer */
    while (!(hw_reg8_read(USCR0A) & (1 << UDRE0)));
    /* Put data into buffer, sends the data */
    hw_reg8_write(UDR0, data);
}

void UART_Transmit_String(unsigned char* str) {
    int i = 0;
    // While there are still bytes to transmit...
    while (str[i] != '\0') {
        // clear transmit complete flag
        //*UCSR0A |= (1 << TXC0);
        UART_Transmit_Byte(str[i]);
        i++;
    }
}

void UART_Transmit_NL(int num, bool cr) {
    for (int i = 0; i < num; i++) {
        UART_Transmit_Byte('\n');
    }
    if (cr) UART_Transmit_Byte('\r');
}