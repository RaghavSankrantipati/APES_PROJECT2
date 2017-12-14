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

pthread_mutex_t log_mutex;
pthread_mutex_t dec_mutex;
pthread_mutex_t heartbeat_mutex;

pthread_cond_t hb_logger;
pthread_cond_t hb_decision;
pthread_cond_t hb_socket;

void* log_func(void* t);
void* decision_func(void* t);
void* socket_func(void* t);
void cleanup_threads(void);
void pthread_initialize(void);

void uart_init(void);
struct termios *configure;
void tty_config(struct termios *con, int descriptor);
int fd;

struct temp_packet
{
  uint8_t log_id;
  uint8_t log_level;
  float data;
  char timestamp[25];
  char c;
};

void siginthandler();
#endif
