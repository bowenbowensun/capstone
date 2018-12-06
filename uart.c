#include "uart.h"
#include "cmd_def.h"

#define TRUE 1
#define FALSE 0

// Global variable
extern char CharacterHasNotBeenReceived;
extern char ReceivedCharacter;

/*
 * USCI_A0 initialization and reset for UART mode. See Section 15.3.1 of users' guide.
*/
void InitializeUART()
{
	// Select UART functionality, Rxd and Txd, for port pins (versus default I/O).
	USCIA0_RXD_P1SEL2 |= USCIA0_RXD_BIT;
	USCIA0_RXD_P1SEL |= USCIA0_RXD_BIT;

	USCIA0_TXD_P1SEL2 |= USCIA0_TXD_BIT;
	USCIA0_TXD_P1SEL |= USCIA0_TXD_BIT;

	// Software reset enabled. USCI logic held in reset state.
	UCA0CTL1 = UCSWRST;
	
	UCA0CTL0 = 0x00; // Initialize control register 0.
	/* UCPEN = 0x80 Async. Mode: Parity enable
	 * UCPAR = 0x40 Async. Mode: Parity     0:odd / 1:even
	 * UCMSB = 0x20 Async. Mode: MSB first  0:LSB / 1:MSB
	 * UC7BIT = 0x10 Async. Mode: Data Bits  0:8-bits / 1:7-bits
	 * UCSPB = 0x08 Async. Mode: Stop Bits  0:one / 1: two
	 * UCMODE1 = 0x04 Async. Mode: USCI Mode 1
	 * UCMODE0 = 0x02 Async. Mode: USCI Mode 0
	 * UCSYNC = 0x01 Sync-Mode  0:UART-Mode / 1:SPI-Mode
	 */

	// Use SMCLK
	UCA0CTL1 |= UCSSEL1; UCA0CTL1 &= ~UCSSEL0;
	/* UCSSEL1 = 0x80 USCI 0 Clock Source Select 1
	 * UCSSEL0 = 0x40 USCI 0 Clock Source Select 0
	 * 	00 = UCLK
	 *  01 = ACLK
	 *  10 = SMCLK
	 *  11 = SMCLK
	 * UCRXEIE = 0x20 RX Error interrupt enable
	 * UCBRKIE = 0x10 = Break interrupt enable
	 * UCDORM = 0x08 Dormant (Sleep) Mode
	 * UCTXADDR = 0x04 Send next Data as Address
	 * UCTXBRK = 0x02 Send next Data as Break
	 * UCSWRST = 0x01 USCI Software Reset
	 */
	
	UCA0BR1 = UCA0BR1_VALUE; UCA0BR0 = UCA0BR0_VALUE;
	UCA0MCTL = (UCBRF_0 + UCBRS_1);
	/*
	 * UCBRF3 = 0x80 USCI First Stage Modulation Select 3
	 * UCBRF2 = 0x40 USCI First Stage Modulation Select 2
	 * UCBRF1 = 0x20 USCI First Stage Modulation Select 1
	 * UCBRF0 = 0x10 USCI First Stage Modulation Select 0
	 * UCBRS2 = 0x08 USCI Second Stage Modulation Select 2
	 * UCBRS1 = 0x04 USCI Second Stage Modulation Select 1
	 * UCBRS0 = 0x02 USCI Second Stage Modulation Select 0
	 * UCOS16 = 0x01 USCI 16-times Oversampling enable
	 */
	
	// Software reset disabled. USCI logic released for operation.
    UCA0CTL1 &= ~UCSWRST;
	
	// Enable USCI_A0 RX interrupt
    // TX does not need an ISR
    IE2 |= UCA0RXIE;
}

// This routine turns off the UART and returns the I/O pins to GPIO functionality.
void ShutdownUART()
{
	// Wait for any remaining characters to transmit
	while (!(IFG2 & UCA0TXIFG));
	
	// Hold USCI module in reset
    UCA0CTL1 |= UCSWRST;
	
	// Pins back to digital I/O
	USCIA0_RXD_P1SEL2 &= ~USCIA0_RXD_BIT;
	USCIA0_RXD_P1SEL &= ~USCIA0_RXD_BIT;

	USCIA0_TXD_P1SEL2 &= ~USCIA0_TXD_BIT;
	USCIA0_TXD_P1SEL &= ~USCIA0_TXD_BIT;
}

void WriteCharacterUART(char data)
{
	// Wait for transmitter to finish current operation
	// TXIFG is set when the buffer is EMPTY
	while (!(IFG2 & UCA0TXIFG));
	
	// Then, assign input character to transmit buffer.
	UCA0TXBUF = data;
}

void WriteStringUART(char *pt)
{ 
	unsigned short i;
	
	/* Note that the correct operation of this function depends on the input
	 * string pointed to by pt to be a null-terminated string (that is, the last
	 * character in the string equals zero).
	 */
	for (i = 0; *(pt+i); i++) {
		WriteCharacterUART(*(pt+i));
	}
}

unsigned char resp_data[256]; // enough for BLE
unsigned char resp_hdr[256];

static void
__attribute__((__interrupt__(USCIAB0RX_VECTOR)))
USCI0RX_ISR(void)
{
    ReceivedCharacter = UCA0RXBUF;
    CharacterHasNotBeenReceived = FALSE;

    //The receive algorithm.
//    const struct ble_msg *BTMessage; //holds BLE message
//    struct ble_header BTHeader; //holds header of message
//    unsigned char data[256] = "\0";//holds payload of message
//
//    //clear the UART 5 interrupt flag
//    UARTIntClear(UART5_BASE, UART_INT_RX);
//
//    //read BLE message header
//    BTHeader.type_hilen = UARTCharGet(UART5_BASE);
//    BTHeader.lolen =  UARTCharGet(UART5_BASE);
//    BTHeader.cls = UARTCharGet(UART5_BASE);
//    BTHeader.command = UARTCharGet(UART5_BASE);
//
//    //wait for UART to finish reading header to ensure data
//    //is valid before using it in the code below
//    while(UARTBusy(UART5_BASE));
//
//    //read the payload of the BLE message
//    for(uint8_t i = 0; i < BTHeader.lolen; i++)
//        data[i] = UARTCharGet(UART5_BASE);
//
//    //find the appropriate message based on the header, which allows
//    //the ble112 library to call the appropriate handler
//    BTMessage = ble_get_msg_hdr(BTHeader);
//
//    //print error if the header doesn't match any known message header
//    if(!BTMessage)
//    {
//            //handle error here
//            return;
//    }
//    //call the handler for the received message, passing in the received payload data
//    BTMessage->handler(data);
}

