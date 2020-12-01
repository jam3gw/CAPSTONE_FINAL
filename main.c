#include <bluetooth.h>
#include <msp430.h>
#include <motor.h>
#include <MSP430FR2xx_4xx/driverlib.h>
#include <sensor.h>

volatile unsigned char ReceivedValue = '\0';
int num_turns;

void motorTurns(int turns);

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD; // stop watchdog timer

    PMM_unlockLPM5();

    InitializePins(); // initializes bluetooth, sensor, and motor connection with msp

    _enable_interrupts();

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Comment this code out for non-timer testing
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * This code sets up Timer B
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
    // default DCO frequency is 2MHz, need to divide this into a lower frequency clock for SMCLK so that TB0CCR0 can contain a value to count up to
    CSCTL5 |= DIVM_7 | DIVS_3; // 2,000,000 / (128 * 8) = 1953.125. If TB0CCR0 is set to 2000 then we will get a timer interrupt roughly every 1 second
    TB0CTL |= MC_0 | TBCLR; // stop timer and clear it
    TB0CTL |= TBSSEL_2 | ID_0; // sets up Timer A0 w/ SMCLK, clock division by 1
    TB0CCTL0 |= CCIE; // enable timer interrupt
    TB0CCR0 = 0x4E20; // set value for counting up to until the Timer B0 interrupt occurs. We want to set it to 20,000 to give 10 seconds into motor attempt before we send failure message


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Uncomment for Hall Effect Sensor Testing
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */


// while(1)
//  {
//          check to see if PORT_1_VECTOR interrupt is triggered when magnet passes over the sensor
//   }


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Uncomment for Motor Testing
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

//  //   Sets up motor functionality
//  ENABLE_SLEEP; //set sleep high
//  LOW_NENBL; // set enable to low to turn on
//  ENABLE_CONFIG;
//  ENABLE_DIR;
//  LOW_M0;
//  LOW_M1;
//  DISABLE_STEP;
//
//  // Motor will spin until for loop expires, then it will shut off the motor
//  for (int i = 0; i < 100; i++){
//      DISABLE_STEP;
//      _delay_cycles(10000);
//      ENABLE_STEP;
//      _delay_cycles(10000);
//   }
//   DISABLE_STEP;
//   HIGH_NENBL;


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Uncomment for Timer B Testing
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

//        CSCTL5 |= DIVM_7 | DIVS_3; // 2,000,000 / (128 * 8) = 1953.125. If TB0CCR0 is set to 2000 then we will get a timer interrupt roughly every 1 second
//        TB0CTL |= MC_0 | TBCLR; // stop timer and clear it
//        TB0CTL |= TBSSEL_2 | ID_0; // sets up Timer A0 w/ SMCLK, clock division by 1
//        TB0CCTL0 |= CCIE; // enable timer interrupt

          // Timer B interrupt should occur every 1 second
//        TB0CCR0 = 0x07D0; // set value for counting up to until the Timer B0 interrupt occurs. We want to set it to 2000 to give 1 seconds into motor attempt before we send failure message
//          TB0CTL |= MC_1; // set Timer B to upmode
// while(1)
//  {
    // infinite while loop
//}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Uncomment for Bluetooth Testing
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Testing will involve being able to connect to the bluetooth, send a message from the phone, receive it here, and send back an acknowledgement message
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
// while(1)
//  {
//        while (ReceivedValue == '\0'); // wait until user connects to the device and sends a value
//        switch (ReceivedValue){
//            case '1':
//                ReceivedValue = '\0';
//                UARTSendString("Successfully Dispensed");
//                break;
//            case '2':
//                ReceivedValue = '\0';
//                UARTSendString("Successfully Dispensed");
//                break;
//            case '3':
//                ReceivedValue = '\0';
//                UARTSendString("Successfully Dispensed");
//                break;
//            case '4':
//                ReceivedValue = '\0';
//                UARTSendString("Successfully Dispensed");
//                break;
//            default:
//                UARTSendString("Please select a different choice");
//                ReceivedValue = '\0';
//                break;
//        }
//  }


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Main function
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
   while(1){
       ENABLE_SLEEP; //set sleep high
       LOW_NENBL; // set enable to low to turn on
       ENABLE_CONFIG;
       ENABLE_DIR;
       LOW_M0;
       LOW_M1;
       DISABLE_STEP;

        while (ReceivedValue == '\0'); // wait until user connects to the device and sends a value

        TB0CTL |= MC_1; // set Timer B to upmode
        switch (ReceivedValue){
            case '1':
                ReceivedValue = '\0';
                motorTurns(1);
                UARTSendString("Successfully Dispensed");
                break;
            case '2':

                ReceivedValue = '\0';
                motorTurns(2);
                UARTSendString("Successfully Dispensed");
                break;
            case '3':
                ReceivedValue = '\0';
                motorTurns(3);
                UARTSendString("Successfully Dispensed");
                break;
            case '4':
                ReceivedValue = '\0';
                motorTurns(4);
                UARTSendString("Successfully Dispensed");
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

    TB0CTL |= MC_0; // stops timer b
    TB0R &= 0; // Resets Timer B count to 0
    return;
}

// For UART interrupt (communication with Bluetooth)
#pragma vector = USCI_A0_VECTOR
__interrupt
void USCIAB0RX_ISR(void)
{
    ReceivedValue = UARTReceiveByte();   // read user input

    UCA0IFG &= ~UCRXIFG;
}

// For Hall Effect Sensor Interrupt
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    P1IFG &= ~SENSOR_BIT;    // Clear P1.1 IFG
    if (num_turns){
        num_turns--;
    } else {
        HIGH_NENBL;
    }
}

// For Timer B Interrupt
#pragma vector=TIMER0_B0_VECTOR
__interrupt void Timer_B(void)
{
    // send error message
    TB0CTL &= ~TBIFG; // clear Timer B0 Flag

    // stop motor from attempting to turn
    num_turns = 0;
    DISABLE_STEP;
    HIGH_NENBL;

    UARTSendString("Error Dispensing");
}

