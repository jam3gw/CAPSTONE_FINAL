#include <bluetooth.h>
#include <motor.h>
#include <sensor.h>

void InitializePins()
{

    // Software reset enabled. USCI logic held in reset state.
    UCA0CTL1 |= UCSWRST;

    // Select USCI UART functionality.
    UCA0CTLW0 |= UCSSEL__SMCLK + UCRXEIE;
        // Baud Rate calculation
    UCA0BRW = 6;                              // 1000000/115200 = 8.68
    UCA0MCTLW |= UCOS16 | UCBRF_8 | 0x20;   //0xD600 is UCBRSx = 0xD6
                                                  // UCBRSx value = 0xD6 (See UG)
    UCA0BR1 = 0;

    P1SEL1 &= ~(BIT6 | BIT7);        // USCI_A0 UART operation
    P1SEL0 |= BIT6 | BIT7;

    SET_RECEIVE_AS_AN_INPUT;
    SET_TRANSMIT_AS_AN_OUTPUT;

//    SET_STATE_AS_AN_OUTPUT; // Bluetooth state pin (not needed)
//    SET_EN_AS_AN_OUTPUT; // Bluetooth enable pin (not needed)

    // Motor Configurations
    SET_SLEEP_AS_AN_OUTPUT;
    SET_NENBL_AS_AN_OUTPUT;
    SET_STEP_AS_AN_OUTPUT;
    SET_DIR_AS_AN_OUTPUT;
    SET_M0_AS_AN_OUTPUT;
    SET_M1_AS_AN_OUTPUT;
    SET_CONFIG_AS_AN_OUTPUT;
    SET_NFAULT_AS_AN_INPUT;


    // Hall Effect Sensor Configuration
    SET_SENSOR_AS_AN_INPUT;

    UCA0TXBUF = 0; // initialize transmit buffer to 0 (for UART communication with Bluetooth)

    UCA0CTL1 &= ~UCSWRST;   // Initialize eUSCI (gets out of Reset state)

    UCA0IE |= UCRXIE;   // Enable USCI_A0 RX interrupt

    P1IE |= SENSOR_BIT; // Enable HallEffectSensor Interrupt
    P1IES |= SENSOR_BIT; // make interrupt falling edge
    P1IFG &= ~SENSOR_BIT; // Clear interrupt flag
}

void UARTSendByte(unsigned char SendValue)
{
    while (!(UCA0IFG & UCTXIFG)); //wait to be ready
    UCA0TXBUF = SendValue;
    while ((UCA0STATW & UCBUSY));
}

unsigned char UARTReceiveByte()
{
    while (!(UCA0IFG & UCRXIFG)); //wait until ready to read
    unsigned char ReceiveValue = UCA0RXBUF;
    while ((UCA0STATW & UCBUSY));
    return ReceiveValue;
}

void UARTSendString(unsigned char *str)
{
    if (str != NULL) {
        while (*str != '\0') {
            UARTSendByte(*str);
            str++;
        }
    }
}
