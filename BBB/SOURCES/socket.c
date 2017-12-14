#include <stdio.h>
#include <mqueue.h>
#include "log.h"
#include "threads.h"
#include "queue.h"
#include <stdlib.h>
#include <string.h>

char *device = "/dev/ttyO4";

void tty_config(struct termios *con, int descriptor)
{
  tcgetattr(descriptor, con);
  con->c_iflag &= ~(IGNBRK | BRKINT | ICRNL | INLCR | PARMRK | INPCK | ISTRIP | IXON);
  con->c_oflag = 0;
  con->c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN | ISIG);
  con->c_cc[VMIN] = 1;
  con->c_cc[VTIME] = 0;
  if(cfsetispeed(con, B57600) || cfsetospeed(con, B57600))
  {
    perror("ERROR in baud set\n");
  }


  if(tcsetattr(descriptor, TCSAFLUSH, con) < 0)
  {
    perror("ERROR in set attr\n");
  }
}


void uart_init(void)
{
   
  fd = open(device, O_RDWR | O_NOCTTY | O_SYNC);// | O_NDELAY);
  if(fd == -1)
  { 
    perror("ERROR opening file descriptor\n");
  }

  configure = (struct termios*)malloc(sizeof(struct termios));
  tty_config(configure, fd);

  if(tcsetattr(fd,TCSAFLUSH, configure) < 0)
  {
    printf("\nERROR: TC Set Attr\n");
  }
}




void* socket_func(void* t){
  log_packet recv;
  static int i=0;
  int data=0;
  time_t curtime=0;
  int8_t ret=0;
  uart_init();
  struct temp_packet rec;
  char str_data[6][50]={"INFO FROM MAIN TASK","INFO FROM SOCKET TASK","TEMPERATURE DATA","LIGHT DATA","HUMIDITY DATA","INFO FROM DECISION TASK"};
  char error_data[3][50]={"LIGHT THREAD NOT WORKING","TEMPERATURE THREAD NOT WORKING","SOCKET THREAD NOT WORKING TIVA"};
  int temp_flag=0;

  while(1){

    memset(&recv,0,sizeof(log_packet));
    memset(&rec,0,sizeof(rec));
    pthread_cond_broadcast(&hb_socket);

    if(exit_flag==1){
      break;
    }
    else{

      printf("\nWAITING FOR SENSOR DATA\n");
      int n=0;
      if((n=read(fd,&rec,sizeof(rec))) < 0){
        printf("\nRead Fail\n");
      }
      printf("\nlog level %d\n",rec.log_level);
      printf("\nlog id %d\n",rec.log_id);
      printf("\nDATA %f\n",rec.data);
      printf("\n%s\n",rec.timestamp);

      if(rec.log_level>3 && rec.log_level<7){
        recv.level=ERROR;
        recv.task_ID=rec.log_id;
        strcpy(recv.log_message,error_data[rec.log_level-4]);
        strcpy(recv.data,"0");
        strcpy(recv.current_time,rec.timestamp);
        temp_flag=1;
      }
      else{
        recv.level=rec.log_level;
        recv.task_ID=rec.log_id;
        strcpy(recv.log_message,str_data[rec.log_id]);
        sprintf(recv.data,"%f",rec.data);
        strcpy(recv.current_time,rec.timestamp);
      }
      
      if(exit_flag==1){
        break;
      }

      ret=pthread_mutex_lock(&log_mutex);
      if(ret){
        printf("\nError: log mutex lock failed in socket\n");
      }

      if(mq_send(mqdes_logger,(const int8_t*)&recv,sizeof(recv),0)==-1){
        printf("\nError in mq_send logger in socket func\n");
        exit(1);
      }

      ret=pthread_mutex_unlock(&log_mutex);
      if(ret){
        printf("\nError: log mutex unlock failed in socket\n");
      }

      if(temp_flag==1){
        siginthandler();
      }

      if(exit_flag==1){
        break;
      }

      ret=pthread_mutex_lock(&dec_mutex);
      if(ret){
        printf("\nError: dec mutex lock failed in socket\n");
      }

      if(mq_send(mqdes_decision,(const int8_t*)&recv,sizeof(recv),0)==-1){
        printf("\nError in mq_send dec in socket func\n");
        exit(1);
      }

      ret=pthread_mutex_unlock(&dec_mutex);
      if(ret){
        printf("\nError: dec mutex unlock failed in socket\n");
      }
      sleep(1);
    }
    
  }
  printf("\nEnd of socket func\n");
  pthread_exit(NULL);
}

