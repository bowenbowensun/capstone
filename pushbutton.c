#include "pushbutton.h"
#include "LED.h"

void InitializePushButtonPortPin (void)
{
    ENABLE_BUTTON_PULLR;
    SET_BUTTON_PULL_UP;
    SET_BUTTON_AS_AN_INPUT;
    ENABLE_INTERRUPT;
}

//#pragma vector = PORT1_VECTOR
//// Button interrupt service routine
//__interrupt void Button_routine(void)
//{
//    TA0CCR0 /= 2;
//    RESET_INTERRUPT_FLAG;
//}

void
__attribute__((interrupt(PORT1_VECTOR)))
Button_routine (void)
{
    TOGGLE_LED1;
    TOGGLE_LED2;
    RESET_INTERRUPT_FLAG;
}
