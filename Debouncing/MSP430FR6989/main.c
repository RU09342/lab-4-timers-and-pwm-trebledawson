#include <msp430.h>
#define LED1 BIT0;
#define BUTTON BIT2;
void main(void)
{
    WDTCTL = WDTPW + WDTHOLD; // Disable watchdog timer
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                                // to activate previously configured port settings
    P1DIR |= LED1;            // Set P1.6 to output
    P1DIR &= ~BUTTON;            // Set P1.3 to input
    P1REN |= BUTTON;            // Set P1.3 resistor enable...
    P1OUT |= BUTTON;            // ...to pullup resistor
    P1IE |= BUTTON;             // Set P1.3 as interrupt pin

    TA0CCTL0 = CCIE;          // Enable capture/compare interrupt
    __enable_interrupt();     // Enable interrupt code
    while(1)
    {}              // By default, do nothing continuously
}

#pragma vector=PORT1_VECTOR   // Define interrupt vector, in this case Port 1
__interrupt void button_press(void) // Define interrupt function that runs whenever interrupt is detected
{

    P1IE &= !BUTTON; // Turn off P1.3 interrupt
    P1OUT ^= LED1;            // Toggle P1.6 (LED)
    TA0CTL = TASSEL_2 + MC_1 + TACLR; // Set Timer A0 to ACLK, up mode
    TA0CCR0 = 10000;  // Count to ~10 ms
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void button_debounce_timer(void)
{
    TA0CTL = MC_0;            // Halt timer
    TA0R = 0;                 // Reset count register
    P1IE |= BUTTON;             // Enable interrupt code
    P1IFG = ~BUTTON;            // Disable interrupt flag to re-enable interrupt
}
