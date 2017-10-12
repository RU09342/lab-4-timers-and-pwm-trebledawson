#include <msp430.h> 
#define LED1 BIT0
#define BUTTON BIT2
int pwm = 500;
void led_setup(void);
void debounce_setup(void);
void pwm_setup(void);
void button_setup(void);

int main(void)
{
    WDTCTL = WDTPW + WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                                // to activate previously configured port settings
    led_setup();
    debounce_setup();
    pwm_setup();
    button_setup();

    __enable_interrupt();

    while(1)
    {}

}

#pragma vector=PORT1_VECTOR
__interrupt void PORT_1(void)
{
    TA1CTL = TASSEL_2 + MC_1 + TACLR;        // Start timer
    P1IFG &= ~BUTTON;      // Clear interrupt flag
    P1IES &= ~BUTTON;      // Set P1.3 to rising edge interrupt
}

#pragma vector=TIMER1_A0_VECTOR
__interrupt void Timer1_A0 (void)
{
    if (pwm < 1000)
    {
        pwm += 100; // Increase duty cycle by 10%
    }
    else
    {
        pwm = 0; // Set duty cycle to 0%
    }

    P1IE |= BUTTON;           // Enable interrupt
    TA0CCR1 = pwm;    // PWM starts at 50% and increase by PWM (100) then reset to zero
    TA1CTL = 0x00;          // Stop timer
}

/* Inline Functions */

void led_setup()
{
    P1SEL0 |= LED1;            // P1.6 set to GPIO
    P1DIR |= LED1;            // P1.6 as output
}

void debounce_setup()
{
    TA1CCTL0 = CCIE;            // CCR1 interrupt enabled
    TA1CCR0 = 10000;            // Set overflow to 10 ms for debouncing
}

void pwm_setup()
{
    TA0CCTL1 = OUTMOD_7;                // CCR1 reset/set mode
    TA0CCR0 = 999;                   // overflow every 1ms for pwm period
    TA0CCR1 = 500;                      // PWM starts at 50%
    TA0CTL = TASSEL_2 + MC_1 + TACLR;   // SMCLK, up mode
}

void button_setup()
{
    P1DIR &= ~BUTTON;                 // P1.3 set to input
    P1REN |= BUTTON;                  // Enable P1.3 resistor...
    P1OUT |= BUTTON;                  // ...to pullup resistor
    P1IE |= BUTTON;                   // Enable P1.3 interrupt
    P1IFG &= ~BUTTON;                 // Clear P1.3 interrupt flag
}
