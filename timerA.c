#include "timerA.h"
#include "LED.h"

void ConfigureTimerA(void)
{
    //stop the timer and clear it
    TA0CTL = (MC0 | TACLR);
    //Choose SMCLK, select an input dividor and start in up mod
    TA0CTL = (TASSEL_2 | ID_2 | MC_1); //id is input dividor, lowers the frequency for timer
    //assign value to timer A0
    //capture / compare register 0
    TA0CCR0 = 62499;
    //enable capture/compare interrupt. not starting the timer.
    TA0CCTL0 |= CCIE;

}

void
__attribute__((interrupt(TIMER0_A0_VECTOR)))
isr_cc0_TA0 (void)
{
    TOGGLE_LED1;
    TOGGLE_LED2;
}

//#pragma vector = TIMER0_A0_VECTOR //this links to the interrupt, the following is just a function. // Timer a interrupt service routine
//__interrupt void
//TimerA0_routine(void)
//{
//    TOGGLE_LED1;
//    TOGGLE_LED2;
//}
