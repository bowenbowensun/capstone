/*
 * ble.c
 *
 *  Created on: Dec 4, 2018
 *      Author: bowensun
 */
#include "ble.h"
#include "cmd_def.h"
#include "uart.h"

int uart_tx(int len, unsigned char *data) {

    //BLE_Transmit(len);
    WriteCharacterUART(len);
    while (len) {
        WriteCharacterUART(*data);
        //BLE_Transmit(*data);
//        debug_Transmit(*data);
        data++;
        len--;

        }
    return 0;
}

void send_api_packet(uint8 len1, uint8* data1, uint16 len2, uint8* data2)
{
    // transmit complete packet via UART
    uart_tx(len1, data1);
    uart_tx(len2, data2);
}

void InitializeBLE(void) {
    bglib_output = send_api_packet;
}



