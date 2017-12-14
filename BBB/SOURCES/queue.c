#include "queue.h"

void cleanup_queues(void){
  mq_close(mqdes_logger);
  mq_close(mqdes_decision);
}

void init_queues(void){
  attr_logger.mq_maxmsg=NO_OF_MSG;
  attr_logger.mq_msgsize=sizeof(log_packet);
  attr_logger.mq_flags=0;

  attr_decision.mq_maxmsg=NO_OF_MSG;
  attr_decision.mq_msgsize=sizeof(log_packet);
  attr_decision.mq_flags=0;

  mq_unlink(QUEUE_LOGGER);
  mq_unlink(QUEUE_DECISION);

  mqdes_logger=mq_open(QUEUE_LOGGER, FLAG, 0644, &attr_logger);

  if(mqdes_logger==-1){
    printf("\nError mq_open logger\n");
    exit(1);
  }

  mqdes_decision=mq_open(QUEUE_DECISION, FLAG, 0644, &attr_decision);

  if(mqdes_decision==-1){
    printf("\nError mq_open decision\n");
    exit(1);
  }
}
