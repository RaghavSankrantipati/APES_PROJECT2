#include <stdio.h>
#include "threads.h"
#include "log.h"
#include <stdlib.h>
#include <string.h>
#include "queue.h"

void* decision_func(void* t){
  log_packet recv;
  uint32_t count_msg=0;
  int n=0;
  int data=0;
  int8_t ret=0;
  log_packet send;
  time_t curtime=0;
  int flag=0;
 
  while(1){
    flag=0;
    memset(&recv,0,sizeof(recv));
    memset(&send,0,sizeof(send));

    pthread_cond_broadcast(&hb_decision);

    if(exit_flag==1){
      break;
    }

    ret=pthread_mutex_lock(&dec_mutex);
    if(ret){
      printf("\nError: dec mutex lock failed in decision func\n");
    }

    mq_getattr(mqdes_decision,&attr_decision);
    count_msg=attr_decision.mq_curmsgs;


    while(count_msg){
      n=mq_receive(mqdes_decision,(int8_t*)&recv,4096,NULL);
    
      data=atoi(recv.data);
      if(recv.task_ID==2){
     
        if(data>40){
          strcpy(send.data,recv.data);
          send.level=ALERT;
          send.task_ID=DECISION_TASK;
          strcpy(send.log_message,"ALERT TEMP DATA");
          char* temp=ctime(&curtime);
          strcpy(send.current_time,temp);
          flag=1;
        }
      }
      else if(recv.task_ID==3){
        if(data>1800){
          strcpy(send.data,recv.data);
          send.level=ALERT;
          send.task_ID=DECISION_TASK;
          strcpy(send.log_message,"ALERT LIGHT DATA");
          char* temp=ctime(&curtime);
          strcpy(send.current_time,temp);

          flag=1;
        }
      }

      if(flag==1){
        if(exit_flag==1){
          break;
        }

        ret=pthread_mutex_lock(&log_mutex);
        if(ret){
          printf("\nError: log mutex lock failed in decision func\n");
        }

        if(mq_send(mqdes_logger,(const int8_t*)&send,sizeof(send),0)==-1){
          printf("\nError in mq_send logger in decision func\n");
          exit(1);
        }

        ret=pthread_mutex_unlock(&log_mutex);
        if(ret){
          printf("\nError: log mutex unlock failed in decision\n");
        }

         flag=0;      
      }
      mq_getattr(mqdes_decision,&attr_decision);
      count_msg=attr_decision.mq_curmsgs;
    }

    ret=pthread_mutex_unlock(&dec_mutex);
    if(ret){
      printf("\nError: dec mutex unlock failed in decison func\n");
    }
    if(exit_flag==1){
      break;
    }
    sleep(1);    
  }
  printf("\nEnd of decision function\n");
  pthread_exit(NULL);
}

