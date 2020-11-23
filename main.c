#include <bluetooth.h>
#include <msp430.h>
#include <motor.h>
#include <MSP430FR2xx_4xx/driverlib.h>
#include <sensor.h>

volatile unsigned char ReceivedValue = '\0';
volatile bool loggedIn = false;
int num_turns;

void exitFunction(void);
void motorTurns(int turns);

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;
    //WDTCTL = WDTPW | WDTHOLD; // stop watchdog timer

    PMM_unlockLPM5();

//  /* Use Calibration values for 1MHz Clock DCO*/
//  DCOCTL = 0;
//  BCSCTL1 = CALBC1_1MHZ;
//  DCOCTL = CALDCO_1MHZ;
    //ConfigureClockModule();

    //initClockTo16MHz();
    InitializePins(); // initializes bluetooth, sensor, and motor connection with msp

    //USCIA0_RESET_PORT |= USCIA0_RESET_BIT;

    _enable_interrupts();


    // Hall Effect Sensor
    //__bis_SR_register(LPM3_bits | GIE); // Enter LPM3 w/interrupt
    //__no_operation();                   // For debug
//    while(1)
//        {
//            __bis_SR_register(LPM3_bits | GIE); // Enter LPM3 w/interrupt
//            __no_operation();                   // For debug
////            P1OUT ^= BIT0;                      // P1.0 = toggle
//        }

//    BLUETOOTH_EN_PORT |= BLUETOOTH_EN_BIT;
//    BLUETOOTH_STATE_PORT &= ~BLUETOOTH_STATE_BIT;

   while(1){
       ENABLE_SLEEP; //set sleep high
       LOW_NENBL; // set enable to low to turn on
       ENABLE_CONFIG;
       ENABLE_DIR;
       LOW_M0;
       LOW_M1;
       DISABLE_STEP;
           // read user input
//        while (!loggedIn);

//        UARTSendString("Please select what size you would like: (s for Small, m for Medium, l for Large, and x to Exit)\r\n");

        while (ReceivedValue == '\0');
//       ReceivedValue = '1';

        switch (ReceivedValue){
            case '1':
                ReceivedValue = '\0';
                motorTurns(1);
                UARTSendString("Successfully Dispensed");
                exitFunction();
                break;
            case '2':

                ReceivedValue = '\0';
                motorTurns(2);
                UARTSendString("Successfully Dispensed");
                exitFunction();
                break;
            case '3':
                ReceivedValue = '\0';
                motorTurns(3);
                UARTSendString("Successfully Dispensed");
                exitFunction();
                break;
            case '4':
                ReceivedValue = '\0';
                motorTurns(4);
                UARTSendString("Successfully Dispensed");
                exitFunction();
                break;
            default:
                UARTSendString("Please select a different choice");
                ReceivedValue = '\0';
                break;
        }

    }

}

void motorTurns(int turns){
    num_turns = turns;
    while(num_turns){
        DISABLE_STEP;
        _delay_cycles(10000);
        ENABLE_STEP;
        _delay_cycles(10000);
    }
    DISABLE_STEP;
    HIGH_NENBL;
    return;
}

void exitFunction() {
//    UARTSendString("Please disconnect your device...\r\n");
    return;
}

#pragma vector = USCI_A0_VECTOR
__interrupt
void USCIAB0RX_ISR(void)
{
    ReceivedValue = UARTReceiveByte();   // read user input
//    if ((ReceivedValue == 'N') && (!loggedIn)) {
//        loggedIn = true;
//        ReceivedValue = '\0';
//    }
    UCA0IFG &= ~UCRXIFG;
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    P1IFG &= ~SENSOR_BIT;    // Clear P1.1 IFG
    if (num_turns){
        num_turns--;
    } else {
        HIGH_NENBL;
    }
    //__bic_SR_register_on_exit(LPM3_bits);   // Exit LPM3
}

