/*
 * ble.h
 *
 *  Created on: Dec 4, 2018
 *      Author: bowensun
 */

#include <msp430.h>
#include "cmd_def.h"
#include<stdlib.h>

#ifndef BLE_H_
#define BLE_H_


void send_api_packet(uint8 len1, uint8* data1, uint16 len2, uint8* data2);
int uart_tx(int len, unsigned char *data);
void InitializeBLE(void);

#endif /* BLE_H_ */
