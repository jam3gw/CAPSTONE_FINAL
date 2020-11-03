#include <bluetooth.h>
#include <msp430.h>
#include <motor.h>
#include <HallEffectSensor.h>
#include <MSP430FR2xx_4xx/driverlib.h>

volatile unsigned char ReceivedValue = '\0';
volatile bool loggedIn = false;
int num_turns;

void exitFunction(void);

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
    InitializeUART(); // initializes blue tooth connection with msp

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

    // Motor Code
      num_turns = 3;

      ENABLE_SLEEP; //set sleep high
      LOW_NENBL; // set enable to low to turn on
      ENABLE_CONFIG;
      ENABLE_DIR;
      LOW_M0;
      LOW_M1;
      DISABLE_STEP;
//      int i;
//      for (i = 0; i < 5; i++ )
      while(num_turns){
          DISABLE_STEP;
          _delay_cycles(10000);
          ENABLE_STEP;
          _delay_cycles(10000);
      }
      DISABLE_STEP;
      HIGH_NENBL;

       return 0;

    // Bluetooth Code
   while(1){
           // read user input
        while (!loggedIn);

        UARTSendString("Please select what size you would like: (s for Small, m for Medium, l for Large, and x to Exit)\r\n");

        while (ReceivedValue == '\0');

        switch (ReceivedValue){
            case 's':
                UARTSendString("Dispensing a Small serving\r\n");
                ReceivedValue = '\0';
                exitFunction();
                break;
            case 'm':
                UARTSendString("Dispensing a Medium serving\n\r");
                ReceivedValue = '\0';
                exitFunction();
                break;
            case 'l':
                UARTSendString("Dispensing a Large serving\n\r");
                ReceivedValue = '\0';
                exitFunction();
                break;
            case 'x':
                exitFunction();
                break;
            default:
                UARTSendString("Please select a different choice\n\r");
                ReceivedValue = '\0';
                break;
        }

    }

}

void exitFunction() {
    UARTSendString("Disconnecting your device...\r\n");
    UARTSendString("AT");
    loggedIn = false;
    return;
}

#pragma vector = USCI_A0_VECTOR
__interrupt
void USCIAB0RX_ISR(void)
{
    ReceivedValue = UARTReceiveByte();   // read user input
    if ((ReceivedValue == 'N') && (!loggedIn)) {
        loggedIn = true;
        ReceivedValue = '\0';
    }
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

