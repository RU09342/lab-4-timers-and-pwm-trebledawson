#include <msp430.h>
#define LED1 BIT0;
#define BUTTON BIT2;

void LEDSetup();
void ButtonSetup();
void TimerA0Setup();
void TimerA1Setup();
unsigned int freqCalc(unsigned int number);

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings
    LEDSetup(); // Initialize our LEDS
    ButtonSetup();  // Initialize our button
    TimerA0Setup(); // Initialize Timer0
    __enable_interrupt(); // Interrupt enable

    while(1)
    {}
}

#pragma vector=TIMER0_A0_VECTOR // When TA0R reaches CCR0
__interrupt void led_period(void)
{
    P1OUT |= LED1;   // Turn P1.6 LED on
}

#pragma vector=TIMER0_A1_VECTOR // When TA0R reaches CCR1
__interrupt void led_off(void)
{
    P1OUT &= ~LED1; // Turn P1.6 LED off
    TA0IV = 0;
}

#pragma vector=PORT1_VECTOR   // Define interrupt vector, in this case Port 1
__interrupt void button_press(void) // Define interrupt function that runs whenever interrupt is detected
{
    P1IE &= !BUTTON; // Turn off P1.3 interrupt
    unsigned int pwm = TA0CCR0 / 10;
    if (TA0CCR1 <= (pwm * 9))
    {
        TA0CCR1 += pwm;
    }
    else
    {
        TA0CCR1 = 1;
    }

    TA1CTL = TASSEL_1 + MC_1 + ID_3 + TACLR; // Set Timer A0 to ACLK, up mode
    TA1CCTL0 = CCIE;  // Compare interrupt enable
    TA1CCR0 = 1000;  // Delay
}

#pragma vector=TIMER1_A0_VECTOR
__interrupt void button_debounce_timer(void)
{
    TA1CTL = MC_0;            // Halt timer
    TA1R = 0;                 // Reset count register
    P1IE |= BUTTON;            // Enable interrupt code
    P1IFG = ~BUTTON;            // Disable interrupt flag to re-enable interrupt
}

void LEDSetup()
{

    P1DIR |= LED1;   // Set P1.6 to output
    P1OUT &= ~LED1;  // Initialize P1.6 to off
}

void ButtonSetup()
{
    P1DIR &= ~BUTTON;            // Set P1.3 to input
    P1REN |= BUTTON;            // Set P1.3 resistor enable...
    P1OUT |= BUTTON;            // ...to enable pullup resistor
    P1IE |= BUTTON;             // Set P1.3 to be interrupt
    P1IES |= BUTTON;            // Set P1.3 to falling edge interrupt
}

void TimerA0Setup()
{
    TA0CTL = TASSEL_2 + MC_1 + ID_3 + TACLR; // Use SMCLK in up mode
    //static const unsigned int desired = 4; // Set desired initial frequency
                                              // f = 1 kHz
    //TA0CCR0 = freqCalc(desired); // counts = SMCLK / (2*desired)
    TA0CCR0 = 1000;
    TA0CCTL0 = CCIE;               // Enable capture/compare interrupt
                                   // Out mode 7, reset/set
    TA0CCTL1 = CCIE ;
    TA0CCR1 = TA0CCR0 / 2;           // Initialize TA0CCR1 to 50% of TA0CCR0
                                     // i.e., 50% PWM
}

unsigned int freqCalc(unsigned int desired)
{
    return 65536 / desired; //
}
