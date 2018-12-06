#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H

#include <msp430.h>

#define BUTTON_BIT                BIT3
#define BUTTON_PORT               P1OUT //used to set the value on the port, we are only reading here.
#define BUTTON_DDR                P1DIR //direction for input or output

#define ENABLE_BUTTON_PULLR       P1REN |= BUTTON_BIT
#define SET_BUTTON_PULL_UP        BUTTON_PORT |= BUTTON_BIT
#define SET_BUTTON_AS_AN_INPUT    BUTTON_DDR &= ~BUTTON_BIT
#define ENABLE_INTERRUPT          P1IE |= BUTTON_BIT
#define RESET_INTERRUPT_FLAG      P1IFG &= ~BUTTON_BIT  //reset the interrupt flag to listen to next interrupt.



// Prototypes
void InitializePushButtonPortPin(void);

#endif
