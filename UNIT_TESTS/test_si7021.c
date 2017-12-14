/**********************************************************************
*@Filename:test_si7021.c
*
*@Description:This is a test for library of SI7021 sensor
*@Author:Sai Raghavendra Sankrantipati, Vishal Vishnani
*@Date:13/12/2017
*@Usage : Connect SI7021 to I2C 0 and check library functions to read and write registers
 **********************************************************************/
#include "drivers/si7021.h"

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

    int  reg_read;
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
    UARTprintf("\r\nUnit testing SI7021!");

	//testing init function
    reg_read = init_SI7021();
    if(reg_read > 0)
        UARTprintf("\r\nSuccess: Sensor Init");
    else
        UARTprintf("\r\nFailure: Sensor Init");

	//testing get humidity function
    reg_read = get_humidity();
    if(reg_read >= 0)
        UARTprintf("\r\nSuccess: Read Humidity");
    else
        UARTprintf("\r\nFailure: Read Humidity");

	//testing get temperature function
    reg_read = get_temperature();
    if(reg_read >= -20)
        UARTprintf("\r\nSuccess: Read Temperature");
    else
        UARTprintf("\r\nFailure: Read Temperature");

	//testing reset function
    reg_read = reset_SI7021();
    if(reg_read > 0)
        UARTprintf("\r\nSuccess: Reset");
    else
        UARTprintf("\r\nFailure: Reset");

    UARTprintf("\r\nComplete: SI7021 Unit Test");
}
