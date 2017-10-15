# Introduction
This project consisted of writing a program in C to control an MSP430 microprocessor to control the brightness of an LED by modulating the width of the on percentage of the period. This pulse-width modulation was controlled with software by using timer interrupts.

# Basic Functionality
The main() is modularized into various setup functions. The LED setup function initializes the LED output pins, the Button setup function initializes the button input pins, and the Timer setup function initializes the Timer A0 settings. 

# Timer0 A0 interrupt
When the Timer 0 A0 timer counter reaches some arbitrary value (in this case set to TA0CCR0 = 1000), the LED output pin is set to high, turning on the LED and indicating the start of a new period.

# Timer0 A1 interrupt
When the Timer 0 A1 timer counter reaches some arbitrary value (by default, set to 50% of TA0CCR0), the LED output pin is set to low, turning off the LED and indicating the end of the "on" part of the period.

# Port 1 interrupt
The Port 1 interrupt vector controls the pulse width percentage of the period. Each time the button is pressed, if the current TA0CCR1 value is less than of equal to TA0CCR0, TA0CCR1 is increased by 10% of TA0CCR0. If TA0CCR1 is greater than TA0CCR0 (that is, if the LED is at 100% duty cycle), pressing the button resets the duty cycle to 0%.

The rest of the Port 1 interrupt code, as well as the entirety of the Timer 1 A0 interrupt vector, simply regulates the debouncing of the button.

