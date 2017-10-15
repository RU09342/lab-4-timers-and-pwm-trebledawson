# Introduction
This project consisted of writing a program in C to control an MSP430 microprocessor to toggle an LED. A crucial difference between this button control and the Lab 3 Button Interrupt program is that this program utilizes a timer to prevent repeated triggers of the interrupt vector caused by mechanical bouncing of the button input.

# Basic Functionality
The introduction to the main() initializes the pin inputs and outputs, as well as enables the Timer A interrupt. 

# Button Interrupt
When the button is pressed, the interrupt vector disables the Port 1 interrupt flag before doing anything else. This prevents the bouncing of the button from repeatedly triggering the interrupt vector. After the interrupt flag has been disabled, the LED output is toggled and the debouncing timer is started. The timer counts to an arbitrary value (here chosen as 10,000 to correspond to a 10 ms delay for the 1 MHz clock frequency of the MSP430 SMCLK) before firing the timer interrupt.

# Timer Interrupt
When the timer counter register reaches the arbitrary delay value, the timer is halted and the timer counter is reset. Then, the Port 1 interrupt flag is re-enabled, and the Port 1 interrupt flag is reset. This allows the Port 1 interrupt vector to again be accessed with a press of the button.
