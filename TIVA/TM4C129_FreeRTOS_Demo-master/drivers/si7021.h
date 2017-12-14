/**********************************************************************
*@Filename:si7021.c
*
*@Description:This is a header for library for SI7021 Humidity and temperature sensor
*@Author:Sai Raghavendra Sankrantipati, Vishal Vishnani
*@Date:13/12/2017
*@Usage : Connect SI7021 9301 to I2C 2 and use any of the library function to read and write registers
 **********************************************************************/

#ifndef DRIVERS_SI7021_H_
#define DRIVERS_SI7021_H_
#include<stdint.h>
#define    SI7021_ADDRESS      0x40

#define    COMMAND_HUM         0xE5
#define    COMMAD_TEMP         0xE3
#define    COMMAND_REST        0xFE

float get_id(void);
float get_temperature(void);
float get_humidity(void);
uint8_t init_SI7021(void);
int reset_SI7021(void);

#endif /* DRIVERS_SI7021_H_ */
