#ifndef BLUETOOTH_H
#define BLUETOOTH_H

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

#define BLUETOOTH_EN_BIT              BIT5
#define BLUETOOTH_EN_PORT             P1OUT
#define BLUETOOTH_EN_DDR              P1DIR
#define SET_EN_AS_AN_OUTPUT           BLUETOOTH_EN_DDR |= BLUETOOTH_EN_BIT

#define BLUETOOTH_STATE_BIT              BIT0
#define BLUETOOTH_STATE_PORT             P2OUT
#define BLUETOOTH_STATE_DDR              P2DIR
#define SET_STATE_AS_AN_OUTPUT           BLUETOOTH_STATE_DDR |= BLUETOOTH_STATE_BIT


void InitializePins();
void UARTSendByte(unsigned char SendValue);
unsigned char UARTReceiveByte();
void UARTSendString(unsigned char *str);


#endif
