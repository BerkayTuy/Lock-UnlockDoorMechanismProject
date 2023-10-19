/*
 * Starter code for ECE 266 Lab 7, Sound Player (PWM), spring 2023
 *
 * pwmled.c: PWM LED functions
 *
 * Created by Zhao Zhang
 */

#include "pwmled.h"

/*
 * Initialize the timer PWM functions connected to the three sub-LEDs.
 *    Red: 		PF1, T0CCP1, Timer 0, sub-timer B
 *    Blue:		PF2, T1CCP0, Timer 1, sub-timer A
 *    Green:	PF3, T1CCP1, Timer 1, sub-time B
 */
void LedPwmInit()
{
    // Enable Timer 0 and Timer 1
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);

    // Connect pins to those timers
    GPIOPinTypeTimer(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
    GPIOPinConfigure(GPIO_PF1_T0CCP1);
    GPIOPinConfigure(GPIO_PF2_T1CCP0);
    GPIOPinConfigure(GPIO_PF3_T1CCP1);

    // Select PWM for Timer 0 sub-Timer B, and Timer 1 sub-Timers A & B
    TimerConfigure(TIMER0_BASE, (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_PWM));
    TimerConfigure(TIMER1_BASE,
                   (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PWM | TIMER_CFG_B_PWM));

    // Invert the PWM waveform, so that the Match register value is the pulse width.
    // Otherwise, the pulse width will be (Load value) - (Match value).
    // The inversion is done by enabling output inversion on the PWM pins.
    TimerControlLevel(TIMER0_BASE, TIMER_B, true /* output inversion */);
    TimerControlLevel(TIMER1_BASE, TIMER_BOTH, true /* output inversion */);

    // Enable the Timer 0's TimerB and Timer 1's TimerA and TimerB
    TimerEnable(TIMER0_BASE, TIMER_B);
    TimerEnable(TIMER1_BASE, TIMER_A | TIMER_B);
}

/*
 * Set color of the on-board LED by setting the intensity of the three primary colors
 */
void LedPwmSet(int pulse_period, int pulse_width_red, int pulse_width_blue, int pulse_width_green)
{
    static int prev_pulse_width_red = 0;
    static int prev_pulse_width_blue = 0;
    static int prev_pulse_width_green = 0;

    // Only update the PWM parameters if they have changed
    if (pulse_width_red != prev_pulse_width_red ||
        pulse_width_blue != prev_pulse_width_blue ||
        pulse_width_green != prev_pulse_width_green) {

        // Set the PWM parameters for red LED
        TimerLoadSet(TIMER0_BASE, TIMER_B, pulse_period - 1);
        TimerMatchSet(TIMER0_BASE, TIMER_B, pulse_width_red - 1);

        // Set the PWM parameters for blue LED
        TimerLoadSet(TIMER1_BASE, TIMER_A, pulse_period - 1);
        TimerMatchSet(TIMER1_BASE, TIMER_A, pulse_width_blue - 1);

        // Set the PWM parameters for green LED
        TimerLoadSet(TIMER1_BASE, TIMER_B, pulse_period - 1);
        TimerMatchSet(TIMER1_BASE, TIMER_B, pulse_width_green - 1);

        // Save the current PWM parameters for future comparison
        prev_pulse_width_red = pulse_width_red;
        prev_pulse_width_blue = pulse_width_blue;
        prev_pulse_width_green = pulse_width_green;
    }
}
