#include "log.h"
#include "threads.h"
#include "queue.h"
#include <string.h>
#include <stdlib.h>

void* log_func (void* t){
  char logger_level[4][15]={"SENSOR_DATA","ERROR","INFO","ALERT"};
  char task_no[6][15]={"MAIN_TASK","SOCKET_TASK","TEMP_TASK","LIGHT_TASK","HUMIDITY_TASK","DECISION_TASK"};
  uint32_t count_msg=0;
  log_packet recv;  
  char logger_level_write[15]={'\0'};
  char task_write[15]={'\0'};
  char buffer[300]={'\0'};
  int8_t ret=0;
  int n=0;

  while(1){
    memset(&recv,0,sizeof(recv));
    pthread_cond_broadcast(&hb_logger);

    ret=pthread_mutex_lock(&log_mutex);
    if(ret){
      printf("\nError: log mutex lock failed in log func\n");
    }
      
    mq_getattr(mqdes_logger,&attr_logger);
    count_msg=attr_logger.mq_curmsgs;

    while(count_msg!=0){
      n=mq_receive(mqdes_logger,(int8_t*)&recv,4096,NULL);
        

      strcpy(logger_level_write, logger_level[recv.level]);
      strcpy(task_write, task_no[recv.task_ID]);
      sprintf(buffer,"\n\nLOG LEVEL    %s\nTASK ID      %s\nTIMESTAMP    %sLOG MESSAGE  %s\nLOG DATA     %s\n\n",logger_level_write,task_write,recv.current_time,recv.log_message,recv.data);
        
      FILE* fd=fopen(file_open,"a+");
      if(fd==NULL){
        printf("\nERROR: file open failed\n");
      }

      fwrite(buffer,1,strlen(buffer),fd);
      fclose(fd);
   
       memset(logger_level_write,'\0',sizeof(logger_level_write));
      memset(task_write,'\0',sizeof(task_write));
      memset(buffer,'\0',sizeof(buffer));

      mq_getattr(mqdes_logger,&attr_logger);
      count_msg=attr_logger.mq_curmsgs;
    }
    ret=pthread_mutex_unlock(&log_mutex);
    if(ret){
      printf("\nlog Mutex unlock failed in log func\n");
    } 

    if(exit_flag==1){
      break;
    }
    sleep(1); 
  }
  printf("\nEnd of logger func\n");
  pthread_exit(NULL);
}

