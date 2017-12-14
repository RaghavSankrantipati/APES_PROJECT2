/*****************************************************************************************
* Authors : Vishal Vishnani, Raghav Sankrantipati
* Date : 12/13/2017
* 
* File : main.c
* Description : Source file for main thread
*****************************************************************************************/

/*Includes*/
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
#include <stdlib.h>
#include <stdio.h>
#include "timers.h"
#include "semphr.h"
#include "drivers/apds9301.h"
#include "drivers/si7021.h"
#include <string.h>
#include "utils/uartstdio.h"
#include "drivers/comm.h"

/*packet structure*/
typedef struct packet
{
    uint8_t log_id;
    uint8_t log_level;
    float data;
    char timestamp[25];
    char c;
}log_packet;

/*size of queue*/
#define QUEUE_LENGTH 10

/*enum for log level*/
typedef enum logger_level
{
    SENSOR_DATA = 0,
    ERROR = 1,
    INFO = 2,
    ALERT = 3,
    ERROR_LIGHT=4,
    ERROR_TEMP=5,
    ERROR_SOCKET=6
}log_level;


/*enum for task id*/
typedef enum task_id
{
  MAIN_TASK = 0,
  SOCKET_TASK = 1,
  TEMP_TASK = 2,
  LIGHT_TASK = 3,
  HUMIDITY_TASK=4,
  DECISION_TASK=5
}tid;


/* Task declarations*/

/*Task which reads value from light sensor periodically and adds to queue
* Also, sends heartbeat to heartbeat task at regular intervals*/
void lightTask(void *pvParameters);

/*Task which reads temeprtaure and humidity value from sensor periodically and adds them to queue
* Also, sends heartbeat to heartbeat task at regular intervals*/
void tempTask(void *pvParameters);

/*Task which send whatever data is present in queue through UART*/
void socketTask(void *pvParameters);

/*Task which checks if all other tasks are alive*/
void heartbeatTask(void *pvParameters);

/*Callback function for timer which sends signal to light and temp task to read sensor data*/
void vTimerCallback(void);

/*Queue*/
QueueHandle_t shared_queue;

/*Timer*/
TimerHandle_t xTimer;

/*semaphore*/

/*Binary sempahore for light task to read sensor data and add to queue*/
xSemaphoreHandle light_signal=0;

/*Binary semaphore for tempertaure task to read temperature and humidity data and add to queue*/
xSemaphoreHandle temp_signal=0;

/*Mutex for queue for mutual exclusion*/
xSemaphoreHandle queue_mutex=0;

/*Binary semaphore for heartbeat of light task*/
xSemaphoreHandle hb_light=0;

/*Binary semaphore for heartbeat of temperature/humidity rask*/
xSemaphoreHandle hb_temp=0;

/*Binary semaphore for heartbeat of socket task*/
xSemaphoreHandle hb_socket=0;

/*Set to 1 for exit*/
int exit_flag=0;

void vTimerCallBack(void* a){
    xSemaphoreGive(light_signal);
    xSemaphoreGive(temp_signal);
}

/*Main function*/
int main(void)
{
    /*Initialize system clock to 120 MHz*/
    uint32_t output_clock_rate_hz;
    output_clock_rate_hz = ROM_SysCtlClockFreqSet(
                                  (SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN |
                                   SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480),
                                  SYSTEM_CLOCK);
    ASSERT(output_clock_rate_hz == SYSTEM_CLOCK);

    /*Initialize the GPIO pins for the Launchpad*/
    PinoutSet(false, false);

    /*Set up the UART which is connected to the virtual COM port*/
    UARTStdioConfig(0, 115200, SYSTEM_CLOCK);

    /*Create a Queue*/
    shared_queue = xQueueCreate(QUEUE_LENGTH,sizeof(log_packet));
    if(shared_queue == NULL){
        UARTprintf("\r\nQueue not be created\r\n");
    }

    UARTprintf("\r\nHELLO!! INSIDE MAIN\r\n");


    /*create binary semaphores*/
    vSemaphoreCreateBinary(light_signal);
    vSemaphoreCreateBinary(temp_signal);
    vSemaphoreCreateBinary(hb_light);
    vSemaphoreCreateBinary(hb_temp);
    vSemaphoreCreateBinary(hb_socket);

    /*Create mutex*/
    queue_mutex=xSemaphoreCreateMutex();

    /*Create tasks*/
    xTaskCreate(lightTask, (const portCHAR *)"LIGHT_TASK",1024, NULL, 1, NULL);

    xTaskCreate(tempTask, (const portCHAR *)"TEMP_TASK",1024, NULL, 1, NULL);

    xTaskCreate(socketTask, (const portCHAR *)"SOCKET_TASK",1024, NULL, 1, NULL);

    xTaskCreate(heartbeatTask, (const portCHAR *)"HEARTBEAT_TASK",1024, NULL, 1, NULL);

    /*Create timer*/
    xTimer=xTimerCreate("Timer",2000,pdTRUE,(void*)0,vTimerCallBack);
    if(xTimer==NULL){
        UARTprintf("\r\nTimer Creation Failed\r\n");
    }
    else{
        if(xTimerStart(xTimer,0)!=pdPASS){
            UARTprintf("\r\nTimer Start Failed\r\n");
        }
    }

    vTaskStartScheduler();


    UARTprintf("\r\nEND OF MAIN\r\n");
    return 0;
}


/*This task checks if all other tasks are alive using binary sempahore*/
void heartbeatTask(void *pvParameters){
    log_packet error_packet;
    memset((void*)&error_packet,0,sizeof(error_packet));
    while(1){

        if(xSemaphoreTake(hb_light,5000)!=pdPASS){
            UARTprintf("\r\nHeartbeat for light task failed\r\n");

            error_packet.data=0.0;
            time_t c = time(NULL);
            char* temp=ctime(&c);
            strcpy(error_packet.timestamp,temp);

            error_packet.log_level =(uint8_t)ERROR_LIGHT;
            error_packet.log_id = (uint8_t)MAIN_TASK;
            error_packet.c='\0';

            /* Send the message to the queue, waiting for 10 ticks for space to become
            available if the queue is already full. */
            if( xQueueSendToBack( shared_queue, &error_packet, 10 ) != pdPASS ){
                UARTprintf("\r\nQueue is full\r\n");
            }
            exit_flag=1;
        }

        if(xSemaphoreTake(hb_temp,5000)!=pdPASS){
            UARTprintf("\r\nHeartbeat for temp task failed\r\n");
            error_packet.data=0.0;
            time_t d = time(NULL);
            char* temp1=ctime(&d);
            strcpy(error_packet.timestamp,temp1);

            error_packet.log_level =(uint8_t)ERROR_TEMP;
            error_packet.log_id = (uint8_t)MAIN_TASK;
            error_packet.c='\0';

            /* Send the message to the queue, waiting for 10 ticks for space to become
            available if the queue is already full. */
            if( xQueueSendToBack( shared_queue, &error_packet, 10 ) != pdPASS ){
                UARTprintf("\r\nQueue is full\r\n");
            }

            exit_flag=1;
        }

        if(xSemaphoreTake(hb_socket,5000)!=pdPASS){
            UARTprintf("\r\nHeartbeat for socket task failed\r\n");
            exit_flag=1;
        }
        /*if(exit_flag==1){
         //   vTaskDelete(NULL);
//            break;
        }*/
    }
}

/*This task sends whatever data is present in the queue through UART*/
void socketTask(void *pvParameters){
    log_packet recv;

    uart_init();
    int i=0;
    for(;;){

        /* Wait for the maximum period for data to become available on the queue.
        The period will be indefinite if INCLUDE_vTaskSuspend is set to 1 in
        FreeRTOSConfig.h. */
        memset((void*)&recv,0,sizeof(recv));
        char data_str[50]={0};
        xSemaphoreGive(hb_socket);
        while(uxQueueSpacesAvailable(shared_queue) != QUEUE_LENGTH){
            memset(data_str,0,sizeof(data_str));
            if(xSemaphoreTake(queue_mutex,portMAX_DELAY)){
           //     UARTprintf("\r\nIn receive task to receive data\r\n");
                if( xQueueReceive( shared_queue, &recv, portMAX_DELAY ) != pdPASS ){
                    UARTprintf("\r\ERROR:Queue Receive\r\n");
                }
                else{
                    UARTprintf("\r\n%s",recv.timestamp);
                    sprintf(data_str,"%f",recv.data);
                    UARTprintf("\r\n%s\r\n",data_str);
                    UARTprintf("\r\nlog id %d\r\n",recv.log_id);
                    UARTprintf("\r\nlog level %d\r\n",recv.log_level);
                    UARTprintf("\r\n------------------------------------------------\r\n",recv.log_level);
                    UARTSend((uint8_t *)&recv,sizeof(recv));
                    for(i = 0; i<100000;i++);
                    //UARTprintf("\r\n%s\r\n",recv.log_message);
                }
         //       UARTprintf("\r\nReceive task completed\r\n");
            }
            xSemaphoreGive(queue_mutex);
        }
        if(exit_flag==1){
            vTaskDelete(NULL);
        }
    }
    //UARTprintf("\r\nEnd of socket task\r\n");
}

/*This task reads data from light sensor periodically and adds it to the queue
* Also sends heartbeat to heartbeat task at periodic intervals*/
void lightTask(void *pvParameters){
    init_APDS9301();
    write_timereg();
    log_packet send;
    float light_data=0;

    for(;;){
        /* Wait for the maximum period for data to become available on the queue.
        The period will be indefinite if INCLUDE_vTaskSuspend is set to 1 in
        FreeRTOSConfig.h. */
       xSemaphoreGive(hb_light);
        if(exit_flag==1){
            vTaskDelete(NULL);
        }
        if(xSemaphoreTake(light_signal,portMAX_DELAY)){
            if(xSemaphoreTake(queue_mutex,portMAX_DELAY)){
                //UARTprintf("\r\nTask1 received signal\r\n");
                memset((void*)&send,0,sizeof(log_packet));
                light_data=get_luminosity();
                send.data=light_data;
                time_t a = time(NULL);
                char* temp=ctime(&a);
                strcpy(send.timestamp,temp);

                send.log_level =(uint8_t)SENSOR_DATA;
                send.log_id = (uint8_t)LIGHT_TASK;
                send.c='\0';

                /* Send the message to the queue, waiting for 10 ticks for space to become
                available if the queue is already full. */
                if( xQueueSendToBack( shared_queue, &send, 10 ) != pdPASS ){
                    UARTprintf("\r\nQueue is full\r\n");
                }
               //UARTprintf("\r\nTask1 completed adding\r\n");
            }
            xSemaphoreGive(queue_mutex);
        }
    }
}


/*This task reads data from temperature sensor periodically and adds it to the queue
* Also sends heartbeat to heartbeat task at periodic intervals*/

void tempTask(void *pvParameters){
    init_SI7021();
    log_packet send;
    float humidity_data=0;
    float temp_data=0;

    for(;;){
        /* Wait for the maximum period for data to become available on the queue.
        The period will be indefinite if INCLUDE_vTaskSuspend is set to 1 in
        FreeRTOSConfig.h. */
        xSemaphoreGive(hb_temp);
        if(exit_flag==1){
            vTaskDelete(NULL);
        }
        if(xSemaphoreTake(temp_signal,portMAX_DELAY)){
            if(xSemaphoreTake(queue_mutex,portMAX_DELAY)){
   //             UARTprintf("\r\nTask2 received signal\r\n");

                memset((void*)&send,0,sizeof(log_packet));
                temp_data=get_temperature();
                send.data=temp_data;
                time_t a = time(NULL);
                char* temp=ctime(&a);
                strcpy(send.timestamp,temp);

                send.log_level = (uint8_t)SENSOR_DATA;
                send.log_id = (uint8_t)TEMP_TASK;
                send.c='\0';

                /* Send the message to the queue, waiting for 10 ticks for space to become
                available if the queue is already full. */
                if( xQueueSendToBack( shared_queue, &send, 10 ) != pdPASS ){
                  UARTprintf("\r\nQueue is full\r\n");
                }


                memset((void*)&send,0,sizeof(log_packet));
                humidity_data=get_humidity();

                send.data=humidity_data;
                a = time(NULL);
                char* temp1=ctime(&a);
                strcpy(send.timestamp,temp1);

                send.log_level =(uint8_t) SENSOR_DATA;
                send.log_id = HUMIDITY_TASK;

                send.c='\0';

                /* Send the message to the queue, waiting for 10 ticks for space to become
                available if the queue is already full. */
                if( xQueueSendToBack( shared_queue, &send, 10 ) != pdPASS ){
                    UARTprintf("\r\nQueue is full\r\n");
                }

 //               UARTprintf("\r\nTask2 completed adding\r\n");
            }
            xSemaphoreGive(queue_mutex);
        }
    }
}

/*  ASSERT() Error function
   failed ASSERTS() from driverlib/debug.h are executed in this function
*/
void __error__(char *pcFilename, uint32_t ui32Line){
    // Place a breakpoint here to capture errors until logging routine is finished
    while (1)
    {
    }
}
