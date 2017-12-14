/*
 * si7021.h
 *
 *  Created on: Dec 9, 2017
 *      Author: vishal
 */

#ifndef DRIVERS_SI7021_H_
#define DRIVERS_SI7021_H_

#define    SI7021_ADDRESS      0x40

#define    COMMAND_HUM         0xE5
#define    COMMAD_TEMP         0xE3
#define    COMMAND_REST        0xFE

float get_id(void);
float get_temperature(void);
float get_humidity(void);
uint8_t init_SI7021(void);
void reset_SI7021(void);

#endif /* DRIVERS_SI7021_H_ */
