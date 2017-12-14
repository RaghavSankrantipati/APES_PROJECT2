/*****************************************************************************************
* Authors : Vishal Vishnani, Raghav Sankrantipati
* Date : 12/13/2017
* 
* File : log.h
* Description : Header file for Log
******************************************************************************************/

#ifndef LOG_H
#define LOG_H

#define DATA_SIZE 200

#include <unistd.h>
#include <stdio.h>
#include <signal.h>

sig_atomic_t exit_flag;
char file_open[100];

//enum for log level
typedef enum logger_level{
  SENSOR_DATA = 0,
  ERROR = 1,
  INFO = 2,
  ALERT = 3,
  ERROR_LIGHT = 4,
  ERROR_TEMP = 5,
  ERROR_SOCKET = 6
}log_level;


//enum for task id
typedef enum task_id{
  MAIN_TASK = 0,
  SOCKET_TASK = 1,
  TEMP_TASK = 2,
  LIGHT_TASK = 3,
  HUMIDITY_TASK=4,
  DECISION_TASK=5
}tid;


//packet structure
typedef struct packet{
  char current_time[50];
  log_level level;
  tid task_ID;
  char log_message[DATA_SIZE];
  char data[50];
}log_packet;


#endif
