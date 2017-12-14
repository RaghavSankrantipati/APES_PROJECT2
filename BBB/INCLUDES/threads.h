/*****************************************************************************************
* Authors : Vishal Vishnani, Raghav Sankrantipati
* Date : 12/13/2017
* 
* File : thread.h
* Description : Header file for Threads
*               -log_func()
*               -decision_func()
*               -socket_func()
*               -pthread_initialize()
*               -cleanup_threads()
*               -uart_init()
*               -signalhandler()
******************************************************************************************/


#ifndef THREADS_H
#define THREADS_H

#include <pthread.h>
#include <stdio.h>
#include "log.h"
#include <stdint.h>
#include <termios.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>
#include<unistd.h>


#define NUM_THREADS (3)

pthread_t threads[NUM_THREADS];

/*MUTEXES*/
pthread_mutex_t log_mutex;
pthread_mutex_t dec_mutex;
pthread_mutex_t heartbeat_mutex;

/*CONDITION VARIABLES*/
pthread_cond_t hb_logger;
pthread_cond_t hb_decision;
pthread_cond_t hb_socket;

/*Function which logs data to a file*/
void* log_func(void* t);

/*Function which logs an alert for a respective sensor*/
void* decision_func(void* t);

/*Function which receives data packet using UART*/
void* socket_func(void* t);

/*Function to destroy all mutexes and condition variables*/
void cleanup_threads(void);

/*Function to create threads, initialize mutex and condition variables*/
void pthread_initialize(void);

/*Fucntion to initialize uart*/
void uart_init(void);

struct termios *configure;

void tty_config(struct termios *con, int descriptor);
int fd;

/*Temporary packet structure to receive from UART*/
struct temp_packet
{
  uint8_t log_id;
  uint8_t log_level;
  float data;
  char timestamp[25];
  char c;
};

/*Signal handler for sigint*/
void siginthandler();
#endif
