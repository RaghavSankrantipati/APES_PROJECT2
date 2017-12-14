#include <pthread.h>
#include "threads.h"
#include <stdio.h>
#include "log.h"
#include "queue.h"
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>

void siginthandler(){
  printf("\nIn SIGINT handler\n");
  int8_t ret=0;

  log_packet packet;
  time_t curtime;
  memset(&packet,0,sizeof(packet));
  packet.level=ERROR;
  packet.task_ID=MAIN_TASK;
  strcpy(packet.log_message,"EXIT GRACEFULLY");
  strcpy(packet.data,"0");
  curtime=time(NULL);
  char* temp=ctime(&curtime);
  strcpy(packet.current_time,temp);


  ret=pthread_mutex_trylock(&log_mutex);
  if(ret==0){

    if(mq_send(mqdes_logger,(const int8_t*)&packet,sizeof(packet),0)==-1){
      printf("\nError in mq_send logger in sig handler\n");
      exit(1);
    }

    ret=pthread_mutex_unlock(&log_mutex);
    if(ret){
      printf("\nError: log mutex unlock failed in sighandler\n");
    }
  }
  printf("\nEnd of sigint handler\n");
  exit_flag=1;
}





void pthread_initialize(void){
  int8_t ret=1;

  ret=pthread_mutex_init(&log_mutex,NULL);
  if(ret){
    printf("\nError in log mutex init\n");
  }

  ret=pthread_mutex_init(&dec_mutex,NULL);
  if(ret){
    printf("\nError in dec mutex init\n");

  }

  ret=pthread_mutex_init(&heartbeat_mutex,NULL);
  if(ret){
    printf("\nError in heartbeat mutex init\n");

  }

  ret=pthread_cond_init(&hb_logger,NULL);
  if(ret){
    printf("\nError in logger cond init\n");
  }

  ret=pthread_cond_init(&hb_decision,NULL);
  if(ret){
    printf("\nError in decision cond init\n");
  }

  ret=pthread_cond_init(&hb_socket,NULL);
  if(ret){
    printf("\nError in socket cond init\n");
  }

  ret=pthread_create(&threads[0],NULL,log_func,NULL);
  if(ret){
    printf("\nError in pthread create 0\n");
  }


  ret=pthread_create(&threads[1],NULL,decision_func,NULL);
  if(ret){
    printf("\nError in pthread create 1\n");
  }

  ret=pthread_create(&threads[2],NULL,socket_func,NULL);
  if(ret){
    printf("\nError in pthread create 2\n");
  }

}



void cleanup_threads(void){
  int8_t ret=1;
  ret=pthread_mutex_destroy(&log_mutex);
  if(ret){
    printf("\nError in log mutex destroy\n");
  }

  ret=pthread_mutex_destroy(&dec_mutex);
  if(ret){
    printf("\nError in dec mutex destroy\n");
  }


  ret=pthread_mutex_destroy(&heartbeat_mutex);
  if(ret){
    printf("\nError in heartbeat mutex destroy\n");
  }


  ret=pthread_cond_destroy(&hb_logger);
  if(ret){
    printf("\nError in logger cond destroy\n");
  }


  ret=pthread_cond_destroy(&hb_decision);
  if(ret){
    printf("\nError in decision cond destroy\n");
  }


  ret=pthread_cond_destroy(&hb_socket);
  if(ret){
    printf("\nError in socket cond destroy\n");
  }

}


