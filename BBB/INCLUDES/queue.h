/*****************************************************************************************
* Authors : Vishal Vishnani, Raghav Sankrantipati
* Date : 12/13/2017
* 
* File : queue.h
* Description : Header file for Message Queues
                -initilialize_queues()
******************************************************************************************/


#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <mqueue.h>
#include "log.h"
#include <stdlib.h>

#define MSG_SIZE 4096
#define NO_OF_MSG 10
#define QUEUE_LOGGER "/logqueue"
#define QUEUE_DECISION "/decqueue"
#define FLAG (O_RDWR | O_CREAT)

//Queue for Logger
struct mq_attr attr_logger;
mqd_t mqdes_logger;

//Queue for Decision
struct mq_attr attr_decision;
mqd_t mqdes_decision;

//Function to initialize queues
void init_queues(void);

//Function to close and delete queues
void cleanup_queues(void);

#endif
