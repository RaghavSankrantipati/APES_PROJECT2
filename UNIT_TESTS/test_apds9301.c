
/**********************************************************************
*@Filename:test_apds9301.c
*
*@Description:This is a test for library of APDS9301 sensor
*@Author:Sai Raghavendra Sankrantipati
*@Date:13/12/2017
*@Usage : Connect APDS 9301 to I2C 2 and check library functions to read and write registers
 **********************************************************************/
 
#include "drivers/apds9301.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <tiva_main.h>
#include "drivers/pinout.h"
#include "utils/uartstdio.h"


// TivaWare includes
#include "driverlib/sysctl.h"
#include "driverlib/debug.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"


void main(void){

    int reg_write, reg_read;
    uint32_t output_clock_rate_hz;

	// Initialize system clock to 120 MHz
    output_clock_rate_hz = ROM_SysCtlClockFreqSet(
                               (SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN |
                                SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480),
                               SYSTEM_CLOCK);
    ASSERT(output_clock_rate_hz == SYSTEM_CLOCK);
	// Initialize the GPIO pins for the Launchpad
    PinoutSet(false, false);
	// Set up the UART which is connected to the virtual COM port
    UARTStdioConfig(0, 57600, SYSTEM_CLOCK);
    UARTprintf("\r\nUnit testing APDS9301!");

	//testing init function
    reg_write = 0x33;
    reg_read = init_APDS9301();
    if(reg_write == reg_read)
        UARTprintf("\r\nSuccess: Sensor Init");
    else
        UARTprintf("\r\nFailure: Sensor Init");

	//testing get luminosity function
    reg_read =get_luminosity();
    if(reg_read >= 0)
        UARTprintf("\r\nSuccess: Sensing data");
    else
        UARTprintf("\r\nFailure: Sensing data");

	//testing control register by writing 0x03
    reg_write = 0x03;
    write_reg(CONTROL_REG, reg_write);
    reg_read = read_reg(CONTROL_REG);
    if(reg_read == 0x33)
        UARTprintf("\r\nSuccess: Control register");
    else
        UARTprintf("\r\nFailure: Control register");

	//testing timing register 
    reg_write = TIME_402ms | MAX_GAIN;
    write_reg(TIMING_REG, reg_write);
    reg_read = read_reg(TIMING_REG);
    if(reg_write == reg_read)
        UARTprintf("\r\nSuccess: Timing register");
    else
        UARTprintf("\r\nFailure: Timing register");

	//reading id register
    reg_read = read_reg(ID_REG);
    if( reg_read ==  80)
        UARTprintf("\r\nSuccess: Id register");
    else
        UARTprintf("\r\nFailure: Id register");

    UARTprintf("\r\nComplete: APDS9301 Unit Test");

}
