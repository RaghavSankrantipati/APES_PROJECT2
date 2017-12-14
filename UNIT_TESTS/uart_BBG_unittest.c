/***************************************************************************************
* Authors : Vishal Vishnani, Raghav Sankrantipati
* Date : 12/13/2017
* 
* File : unittest_uart_BBB.c
* Description : Unit test file for uart for BBB
******************************************************************************************/

#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<termios.h>
#include<string.h>
#include<stdint.h>

#define FLAGS O_RDWR | O_NOCTTY | O_SYNC

int uart_init(void)
{
  int fd = 0;
  char *device = "/dev/ttyO4";
  if ((fd = open(device, FLAGS))<0){
    printf("\nERROR: UART FILE OPEN FAILED\n");
    return -1;
  }

  struct termios config_opt;
  tcgetattr(fd, &config_opt);
  config_opt.c_iflag &= ~(IGNBRK | BRKINT | INLCR | PARMRK | INPCK | ISTRIP | IXON);
  config_opt.c_oflag = 0;
  config_opt.c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN | ISIG);	
  cfsetispeed(&config_opt, B57600);
  cfsetospeed(&config_opt, B57600);
  tcsetattr(fd, TCSAFLUSH, &config_opt);
  return fd;
}



int main()
{
  printf("\n---------------------UART POSITIVE TEST FOR LOOPBACK ON BBG----------------------\n");   	
  int f=0;
  int count_byte = 0;
  f = uart_init();
  sleep(1);
  char send = 'a';
  char recv;
  tcflush(f, TCOFLUSH);
  count_byte = write(f, &send, 1);
  printf("\nBYTES SENT %d\n",count_byte);
  sleep(1);
  count_byte =  read(f, &recv, 1);
  printf("\nBYTES READ %d\n",count_byte);
  sleep(1);
  tcflush(f, TCOFLUSH);
  if(recv == send)
  {
    printf("\nUART LOOPBACK TEST: PASSED\n");
  }
  else
  {
    printf("\nUART LOOPBACK TEST: FAILURE\n");
  }
   
  close(f);
  return 0;
}
