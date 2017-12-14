/***************************************************************************************
* Authors : Vishal Vishnani, Raghav Sankrantipati
* Date : 12/13/2017
* 
* File : queue_tiva_unittest.c
* Description : Unit test file for queues for TIVA
******************************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "main.h"
#include "drivers/pinout.h"
#include "utils/uartstdio.h"
#include "driverlib/sysctl.h"
#include "driverlib/debug.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#define QUEUE_LENGTH 3

/*Handler for Queue*/
QueueHandle_t basic_queue;

// Main function
int main(void)
{
    // Initialize system clock to 120 MHz
    uint32_t output_clock_rate_hz;
    output_clock_rate_hz = ROM_SysCtlClockFreqSet(
                                  (SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN |
                                   SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480),
                                  SYSTEM_CLOCK);
    ASSERT(output_clock_rate_hz == SYSTEM_CLOCK);

    // Initialize the GPIO pins for the Launchpad
    PinoutSet(false, false);

    /*Create a Queue*/
    basic_queue = xQueueCreate(QUEUE_LENGTH,10);
    if(basic_queue == NULL)
    {
       UARTprintf("\r\nQueue not be created\r\n");
    }

    // Set up the UART which is connected to the virtual COM port
    UARTStdioConfig(0, 57600, SYSTEM_CLOCK);

    UARTprintf("\r\nHELLO!! INSIDE MAIN\r\n");

    char send[10]="Hello\0";
    char recv[10]={0};

    //Test 1: Length of empty queue
    if( uxQueueSpacesAvailable(basic_queue)==3 ){
        UARTprintf("\r\nTEST1: LENGTH EMPTY QUEUE TEST PASSED\r\n");
    }
    else{
      UARTprintf("\r\nTEST1: LENGTH EMPTY QUEUE TEST FAILED\r\n");
    }

    //Test2: Adding and removing from empty queue
    if( xQueueSendToBack( basic_queue, &send, strlen(send) ) != pdPASS ){
        UARTprintf("\r\nQueue is full\r\n");
    }

    if( xQueueReceive( basic_queue, &recv, portMAX_DELAY ) != pdPASS ){
        UARTprintf("\r\ERROR:Queue Receive\r\n");
    }

    int ret=strcmp(send,recv);
    if(ret){
        UARTprintf("\r\nTEST2: COMPARE ADD-REMOVE QUEUE FAILED\r\n");
    }
    else{
      UARTprintf("\r\nTEST2: COMPARE ADD-REMOVE QUEUE PASSED\r\n");
    }

    //TEST3: Spaces available after adding 2 elements
    if( xQueueSendToBack( basic_queue, &send, strlen(send) ) != pdPASS ){
        UARTprintf("\r\nQueue is full\r\n");
    }

    if( xQueueSendToBack( basic_queue, &send, strlen(send) ) != pdPASS ){
        UARTprintf("\r\nQueue is full\r\n");
    }

    if( uxQueueSpacesAvailable(basic_queue)==1 ){
        UARTprintf("\r\nTEST3: SPACES AVAILABLE QUEUE TEST PASSED\r\n");
    }
    else{
        UARTprintf("\r\nTEST3: SPACES AVAILABLE QUEUE TEST FAILED\r\n");
    }


    //TEST4: Adding more than queue length
    if( xQueueSendToBack( basic_queue, &send, strlen(send) ) != pdPASS ){
      UARTprintf("\r\nQueue is full\r\n");
    }

    if( xQueueSendToBack( basic_queue, &send, strlen(send) ) != pdPASS ){
      UARTprintf("\r\nTEST4: ADDING MORE THAN QUEUE LENGTH FAILED\r\n");
    }
    else{
        UARTprintf("\r\nTEST4: ADDING MORE THAN QUEUE LENGTH PASSED\r\n");
    }


    //TEST5:Removing from an Empty Queue
    if( xQueueReceive( basic_queue, &recv, portMAX_DELAY ) != pdPASS ){
      UARTprintf("\r\nERROR:Queue Receive\r\n");
    }

    if( xQueueReceive( basic_queue, &recv, portMAX_DELAY ) != pdPASS ){
      UARTprintf("\r\nERROR:Queue Receive\r\n");
    }

    if( xQueueReceive( basic_queue, &recv, portMAX_DELAY ) != pdPASS ){
      UARTprintf("\r\nERROR:Queue Receive\r\n");
    }

    if( xQueueReceive( basic_queue, &recv, portMAX_DELAY ) != pdPASS ){
        UARTprintf("\r\nTEST5: REMOVE FROM EMPTY QUEUE FAILED\r\n");
    }
    else{
        UARTprintf("\r\nTEST5: REMOVE FROM EMPTY QUEUE PASSED\r\n");
    }


    UARTprintf("\r\nEND OF MAIN\r\n");
    //return 0;
}

void __error__(char *pcFilename, uint32_t ui32Line){
// Place a breakpoint here to capture errors until logging routine is finished
    while (1)
    {
    }
}
