/***************************************************************************************
* Authors : Vishal Vishnani, Raghav Sankrantipati
* Date : 12/13/2017
* 
* File : unittest_queue.c
* Description : Unit test file for queues
******************************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h> 
#include <mqueue.h>
#include <unistd.h>
#include <semaphore.h>
#include <string.h>
#include <signal.h>
#include <sys/time.h>

typedef enum queue_t
{
  QUEUE_ADD_FAIL = 1,
  QUEUE_ADD_SUCCESS,
  QUEUE_REMOVE_SUCCESS,
  QUEUE_REMOVE_FAIL

}qt;

#define MSG_SIZE 4096
#define NO_OF_MGS 2
#define QUEUE_NAME "/queue"
#define FLAG (O_RDWR | O_CREAT | O_NONBLOCK)

int total_passes = 0;
int total_fails = 0;

//adding to full message queue
qt message_queue_add_full(void)
{
  //message queue
  struct mq_attr attr;
  mqd_t mqdes1;

  //setting attributes for the message queue
  attr.mq_maxmsg = NO_OF_MGS;
  attr.mq_msgsize = MSG_SIZE;
  attr.mq_flags = 0;
  
  //removes the queue name if already existing
  mq_unlink(QUEUE_NAME);

  //create a queue with mqdes1 as the queue descriptor
  mqdes1 = mq_open(QUEUE_NAME, FLAG , 0644, &attr);
 
  if(mqdes1 == -1)  
  {
    printf("\nERROR mq_open\n");
    exit(1);
  }

  
  char trial[] = "HELLO WPRLD";
  if(mq_send(mqdes1,(const char *)&trial, sizeof(trial),0) == -1)
  {
      return QUEUE_ADD_FAIL;
  }
  
  char tr[] = "HI";
  if(mq_send(mqdes1,(const char *)&tr, sizeof(tr),0) == -1)
  {
      return QUEUE_ADD_FAIL;
  }
  
  char tr1[] = "HI";
  if(mq_send(mqdes1,(const char *)&tr1, sizeof(tr1),0) == -1)
  {
      return QUEUE_ADD_FAIL;
 }

}

//adding to message queue success
qt message_queue_add_success(void)
{
  //message queue
  struct mq_attr attr;
  mqd_t mqdes1;

  //setting attributes for the message queue
  attr.mq_maxmsg = NO_OF_MGS;
  attr.mq_msgsize = MSG_SIZE;
  attr.mq_flags = 0;
  
  //removes the queue name if already existing
  mq_unlink(QUEUE_NAME);

  //create a queue with mqdes1 as the queue descriptor
  mqdes1 = mq_open(QUEUE_NAME, FLAG , 0644, &attr);

  //check for successful mq_open
  if(mqdes1 == -1)  
  {
    exit(1);
  }

  //get the attributes for the queue
  mq_getattr(mqdes1,&attr);

  char* trial = "HELLO WPRLD";

  //send the message to the queue and check for success
    if(mq_send(mqdes1,(const char *)&trial, sizeof(trial),0) == -1)
    {
      return QUEUE_ADD_FAIL;
    }
 
    //send the message to the queue and check for success
    if(mq_send(mqdes1,(const char *)&trial, sizeof(trial),0) == -1)
    {
      return QUEUE_ADD_FAIL;
    }
    else
    {
      return QUEUE_ADD_SUCCESS;
    } 
}

//removing from empty queue
qt message_queue_remove_empty(void)
{
  //message queue
  struct mq_attr attr;
  mqd_t mqdes1;

  //setting attributes for the message queue
  attr.mq_maxmsg = NO_OF_MGS;
  attr.mq_msgsize = MSG_SIZE;
  attr.mq_flags = 0;
  
  //removes the queue name if already existing
  mq_unlink(QUEUE_NAME);

  //create a queue with mqdes1 as the queue descriptor*/
  mqdes1 = mq_open(QUEUE_NAME, FLAG , 0644, &attr);

  //check for successful mq_open
  if(mqdes1 == -1)  
  {
    exit(1);
  }

  //get the attributes for the queue
  mq_getattr(mqdes1,&attr);

  
  char *receiver;  
  int n = mq_receive(mqdes1,(char*)&receiver,MSG_SIZE,NULL);
  
  if(n<0)
  {
    return QUEUE_REMOVE_FAIL;
  }
  else
  {
    return QUEUE_REMOVE_SUCCESS;
  }

   
}

//remove from  message queue success
qt message_queue_remove_success(void)
{
  //message queue
  struct mq_attr attr;
  mqd_t mqdes1;

  //setting attributes for the message queue
  attr.mq_maxmsg = NO_OF_MGS;
  attr.mq_msgsize = MSG_SIZE;
  attr.mq_flags = 0;
  
  //removes the queue name if already existing
  mq_unlink(QUEUE_NAME);

  //create a queue with mqdes1 as the queue descriptor
  mqdes1 = mq_open(QUEUE_NAME, FLAG , 0644, &attr);

  //check for successful mq_open
  if(mqdes1 == -1)  
  {
    printf("\nERROR mq_open\n");
    exit(1);
  }

  //get the attributes for the queue
  mq_getattr(mqdes1,&attr);

  char* trial = "HELLO WPRLD";

  //send the message to the queue and check for success
  if(mq_send(mqdes1,(const char *)&trial, sizeof(trial),0) == -1)
  {
    printf("\nERROR: mqsend\n");
  }
  
  char *receiver;  
  int n = mq_receive(mqdes1,(char*)&receiver,MSG_SIZE,NULL);
  
  if(n<0)
  {
    return QUEUE_REMOVE_FAIL;
  }
  else
  {
    return QUEUE_REMOVE_SUCCESS;
  }

   
}



int main()
{

  printf("\nUNIT TESTS\n");
  printf("\n------------------------------------------\n");

  uint32_t test_result;

  /******TEST TO ADD TO FULL QUEUE*******/
  test_result = message_queue_add_full();
  if(test_result == QUEUE_ADD_FAIL)
    printf("\nTEST TO ADD TO FULL QUEUE: PASS\n");
  else
    printf("\nTEST TO ADD TO FULL QUEUE - FAIL\n");
 
  /******TEST TO ADD TO QUEUE THAT IS NOT FULL*******/
  test_result = message_queue_add_success();
  if(test_result == QUEUE_ADD_SUCCESS)
    printf("\nTEST TO ADD TO QUEUE WHICH IS NOT FULL: PASS\n");
  else
    printf("\nTEST TO ADD TO QUEUE WHICH IS NOT FULL - FAIL\n");

  /******TEST TO REMOVE FROM EMPTY QUEUE*******/
  test_result = message_queue_remove_empty();
  if(test_result == QUEUE_REMOVE_FAIL)
    printf("\nTEST TO REMOVE FROM EMPTY QUEUE: PASS\n");
  else
    printf("\nTEST TO REMOVE FROM EMPTY QUEUE - FAIL\n");


  /******TEST TO REMOVE FROM NON EMPTY QUEUE*******/
  test_result = message_queue_remove_success();
  if(test_result == QUEUE_REMOVE_SUCCESS)
    printf("\nTEST TO REMOVE FROM EMPTY QUEUE: PASS\n");
  else
    printf("\nTEST TO REMOVE FROM EMPTY QUEUE - FAIL\n");
}
