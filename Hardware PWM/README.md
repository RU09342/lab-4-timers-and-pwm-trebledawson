# Introduction
This project consisted of writing a program in C to control an MSP430 microprocessor to control the brightness of an LED by modulating the width of the on percentage of the period. This pulse-width modulation was controlled with hardware using pin outputs controlled by timer mode.

# Basic Functionality
The main() is modularized into various setup functions. The LED setup function initializes the LED output pins, the Button setup function initializes the button input pins, and the Timer setup function initializes the Timer A0 settings. 

# PWM Setup
A crucial component of the hardware PWM is the PWM setup function. By setting TA0CCTL1 to OUTMOD_7, the output pin specified in the LED setup function is controlled by Timer A0, and the behavior is determined by OUTMOD_7, which sets the output pin high when the timer counter reaches CCR0, and resets the output pin to low when the timer counter reaches CCR1. Because of this, all that is necessary to modify the duty cycle is changing CCR1. This is done in the interrupt vectors, whose behavior is similar to that of the Software PWM code.
