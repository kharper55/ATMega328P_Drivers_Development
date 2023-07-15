/***********************************************************************
* Custom types for AVR Microcontrollers UART driver                    *
* @author Kevin Harper                                                 *
* @date July 14, 2023                                                  *
* Purpose: Provide an easy interface to deal with serial terminal      *
***********************************************************************/

#ifndef UART_TYPES_H_
#define UART_TYPES_H_

typedef enum UART_Dir {
    TX, 
    RX, 
    BOTH
} UART_Dir_t;

typedef enum UART_Parity {
    NONE = 0, 
    EVEN = 2, 
    ODD = 3
} UART_Parity_t;

// These flags correspond to bits 7:2 of USCR0A 
typedef enum UART_Flag {
    RXC = 7,  /*Receive complete*/
    TXC = 6,  /*Transmit complete*/
    UDRE = 5, /*Data register empty*/
    FE = 4,   /*Frame err*/
    DOR = 3,  /*Data ovverrun*/
    PE = 2    /*Parity err*/
} UART_Flag_t;

#endif //UART_TYPES_H_