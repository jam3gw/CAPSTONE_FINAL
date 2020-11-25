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

    // default DCO frequency is 2MHz, need to divide this into a lower frequency clock for SMCLK so that TB0CCR0 can contain a value to count up to
    CSCTL5 |= DIVM_7 | DIVS_3; // 2,000,000 / (128 * 8) = 1953.125. If TB0CCR0 is set to 2000 then we will get a timer interrupt roughly every 1 second
    TB0CTL |= MC_0 | TBCLR; // stop timer and clear it
    TB0CTL |= TBSSEL_2 | ID_0; // sets up Timer A0 w/ SMCLK, clock division by 1
    TB0CCTL0 |= CCIE; // enable timer interrupt
    TB0CCR0 = 0x4E20; // set value for counting up to until the Timer B0 interrupt occurs. We want to set it to 20,000 to give 10 seconds into motor attempt before we send failure message


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

#pragma vector = USCI_A0_VECTOR
__interrupt
void USCIAB0RX_ISR(void)
{
    ReceivedValue = UARTReceiveByte();   // read user input

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
}

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

