/*
 * comm.h
 *
 *  Created on: Dec 13, 2017
 *      Author: vishal
 */

#ifndef DRIVERS_COMM_H_
#define DRIVERS_COMM_H_

int uart_init();
void UARTIntHandler(void);
void UARTSend(const uint8_t *pui8Buffer, uint32_t ui32Count);

#endif /* DRIVERS_COMM_H_ */
