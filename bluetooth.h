#ifndef USCI_UART_H
#define USCI_UART_H

#include <msp430.h>
#include <stddef.h>


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * UC0RX USCI_A0 receive data input in UART mode
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * GPIO      :  P1.6
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
#define USCIA0_RECEIVE_BIT              BIT6
#define USCIA0_RECEIVE_PORT             P1IN
#define USCIA0_RECEIVE_DDR              P1DIR
#define SET_RECEIVE_AS_AN_INPUT         USCIA0_RECEIVE_DDR &= ~USCIA0_RECEIVE_BIT

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * UC0TX USCI_A0 transmit data output in UART mode
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * GPIO      :  P1.7
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
#define USCIA0_TRANSMIT_BIT             BIT7
#define USCIA0_TRANSMIT_PORT            P1OUT
#define USCIA0_TRANSMIT_DDR             P1DIR
#define SET_TRANSMIT_AS_AN_OUTPUT       USCIA0_TRANSMIT_DDR |= USCIA0_TRANSMIT_BIT


void InitializeUART();
void UARTSendByte(unsigned char SendValue);
unsigned char UARTReceiveByte();
void UARTSendString(unsigned char *str);


#endif
