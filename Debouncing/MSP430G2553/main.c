#include <msp430g2553.h>

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD; // Disable watchdog timer
    P1DIR |= BIT6;            // Set P1.6 to output
    P1DIR &= ~BIT3;            // Set P1.3 to input
    P1REN |= BIT3;            // Set P1.3 resistor enable...
    P1OUT |= BIT3;            // ...to pullup resistor
    P1IE |= BIT3;             // Set P1.3 as interrupt pin

    TA0CCTL0 = CCIE;          // Enable capture/compare interrupt
    __enable_interrupt();     // Enable interrupt code
    while(1)
    {}              // By default, do nothing continuously
}

#pragma vector=PORT1_VECTOR   // Define interrupt vector, in this case Port 1
__interrupt void button_press(void) // Define interrupt function that runs whenever interrupt is detected
{

    P1IE &= !BIT3; // Turn off P1.3 interrupt
    P1OUT ^= BIT6;            // Toggle P1.6 (LED)
    TA0CTL = TASSEL_1 + MC_1 + TACLR; // Set Timer A0 to ACLK, up mode
    TA0CCR0 = 1000;  // Count to ~10 ms
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void button_debounce_timer(void)
{
    TA0CTL = MC_0;            // Halt timer
    TA0R = 0;                 // Reset count register
    P1IE |= BIT3;             // Enable interrupt code
    P1IFG = ~BIT3;            // Disable interrupt flag to re-enable interrupt
}
