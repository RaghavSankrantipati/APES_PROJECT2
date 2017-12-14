/**********************************************************************
*@Filename:test_comm.h
*
*@Description:This is a test for library of UART7 
*@Author:Sai Raghavendra Sankrantipati, Vishal Vishnani
*@Date:13/12/2017
*@Usage : Connect PC4 to PC5 0 and check library functions to send and recieve data over UART 7
 **********************************************************************/
#ifndef DRIVERS_COMM_H_
#define DRIVERS_COMM_H_

typedef struct packet_t{
    char element1[10];
    char element2[10];
    char element3[10];
    uint8_t ele1;
    uint8_t ele2;
    uint8_t ele3;
}packet_t;

typedef struct packet_t *packet_ptr;

int uart_init();
void UARTIntHandler(void);
void UARTSend(const uint8_t *pui8Buffer, uint32_t ui32Count);

#endif /* DRIVERS_COMM_H_ */
