#include <msp430.h> 
#include <stdio.h>
#include "uart.h"
#include "pushbutton.h"
#include "timerA.h"
#include "LED.h"
#include "cmd_def.h"
#include "ble.h"

#define BUFFER_SIZE 256
#define TRUE 1
#define FALSE 0
#define UART 0

// Global variable
char CharacterHasNotBeenReceived;
char ReceivedCharacter = (char) 0;

// Function prototypes
void ConfigureClockModule(void);

void PrintString(char * StringToPrint, unsigned char PrintDeviceType)
{
    unsigned short i;                     // Used for indexing
    char StringBuffer[BUFFER_SIZE];    // Character buffer to hold each string to be printed

    for (i = 0; i < BUFFER_SIZE; i++) StringBuffer[i] = (char) NULL;
    sprintf(StringBuffer, StringToPrint);
    switch (PrintDeviceType) {
    case UART:
        WriteStringUART(StringBuffer);
        break;
    default: break;
    }
}

void PrintCharacter(char CharacterToPrint, unsigned char PrintDeviceType)
{
    switch (PrintDeviceType) {
    case UART:
        WriteCharacterUART(CharacterToPrint);
        break;
    default: break;
    }
}

void main(void)
{
    // Stop the watchdog timer, and configure the clock module.
    WDTCTL = WDTPW + WDTHOLD;
    ConfigureClockModule();

    // Initialize port pins associated with the LEDs, and then turn off LEDs.
    InitializeLEDPortPins();

    // Initialize the port pin associated with the pushbutton
    InitializePushButtonPortPin();

    // Configure timer A to generate the required interrupt.
    //ConfigureTimerA();

    //Initialize UART to receive message from BLE.
    InitializeUART();

    //Initialize Ble to link the send function pointer.
    InitializeBLE();

    _enable_interrupts();

    while (TRUE) {
        TOGGLE_LED1;
        //ble_cmd_system_hello();
        WriteCharacterUART('c');
        //PrintString("wahoowa", UART);
        __delay_cycles(1000000);
    }

}

void ConfigureClockModule(void)
{
    // Configure Digitally Controlled Oscillator (DCO) using factory calibrations.
    DCOCTL  = CALDCO_1MHZ;
    BCSCTL1 = CALBC1_1MHZ;
}
