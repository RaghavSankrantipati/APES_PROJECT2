/**********************************************************************
*@Filename:apds9301.c
*
*@Description:This is a library for APDS 9301 sensor
*@Author:Sai Raghavendra Sankrantipati, Vishal Vishnani
*@Date:13/12/2017
*@Usage : Connect APDS 9301 to I2C 2 and use any of the library function to read and write registers
 **********************************************************************/

#include <stdarg.h>
#include <stdbool.h>
#include <math.h>
#include <stdint.h>
#include "inc/hw_i2c.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "apds9301.h"

/* Initialise sensor on I2C bus 2
 * */
uint8_t init_APDS9301(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C2);
    SysCtlPeripheralReset(SYSCTL_PERIPH_I2C2);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
    GPIOPinConfigure(GPIO_PN5_I2C2SCL);
    GPIOPinConfigure(GPIO_PN4_I2C2SDA);
    GPIOPinTypeI2CSCL(GPIO_PORTN_BASE, GPIO_PIN_5);
    GPIOPinTypeI2C(GPIO_PORTN_BASE, GPIO_PIN_4);
    //HWREG(I2C0_BASE + I2C_O_MCR) |= 0x01;
    I2CMasterInitExpClk(I2C2_BASE, 120000000U, false);
    //I2CMasterInitExpClk(I2C0_BASE, SYSTEM_CLOCK, false);
    //HWREG(I2C2_BASE + I2C_0_FIFOCTL) = 80008000;

    //send
    I2CMasterSlaveAddrSet(I2C2_BASE, APDS9301_ADDRESS, false);
    I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_BURST_SEND_START);
    I2CMasterDataPut(I2C2_BASE, COMMAND | CONTROL_REG);
    while(I2CMasterBusy(I2C2_BASE));
    I2CMasterSlaveAddrSet(I2C2_BASE, APDS9301_ADDRESS, false);
    I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
    I2CMasterDataPut(I2C2_BASE, POWER_UP);
    while(I2CMasterBusy(I2C2_BASE));

    //Recieve
    I2CMasterSlaveAddrSet(I2C2_BASE, APDS9301_ADDRESS, false);
    I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_SINGLE_SEND);
    I2CMasterDataPut(I2C2_BASE, COMMAND | CONTROL_REG);
    while(I2CMasterBusy(I2C2_BASE));
    I2CMasterSlaveAddrSet(I2C2_BASE, APDS9301_ADDRESS, true);
    I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);
    while(I2CMasterBusy(I2C2_BASE));
    return I2CMasterDataGet(I2C2_BASE);
}


/* Read data 0 low register of light*/
uint8_t read_data0low(void){

    I2CMasterSlaveAddrSet(I2C2_BASE, APDS9301_ADDRESS, false);
    I2CMasterDataPut(I2C2_BASE, COMMAND | DATA0L_REG);
    I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_SINGLE_SEND);
    while(I2CMasterBusy(I2C2_BASE));
    I2CMasterSlaveAddrSet(I2C2_BASE, APDS9301_ADDRESS, true);
    I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);
    while(I2CMasterBusy(I2C2_BASE));
    return I2CMasterDataGet(I2C2_BASE);

}
/* Read data 0 high register of light*/
uint8_t read_data0high(void){

    I2CMasterSlaveAddrSet(I2C2_BASE, APDS9301_ADDRESS, false);
    I2CMasterDataPut(I2C2_BASE, COMMAND | DATA0H_REG);
    I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_SINGLE_SEND);
    while(I2CMasterBusy(I2C2_BASE));
    I2CMasterSlaveAddrSet(I2C2_BASE, APDS9301_ADDRESS, true);
    I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);
    while(I2CMasterBusy(I2C2_BASE));
    return I2CMasterDataGet(I2C2_BASE);

}

/* Read data 1 low register of light*/
uint8_t read_data1low(void){

    I2CMasterSlaveAddrSet(I2C2_BASE, APDS9301_ADDRESS, false);
    I2CMasterDataPut(I2C2_BASE, COMMAND | DATA1L_REG);
    I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_SINGLE_SEND);
    while(I2CMasterBusy(I2C2_BASE));
    I2CMasterSlaveAddrSet(I2C2_BASE, APDS9301_ADDRESS, true);
    I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);
    while(I2CMasterBusy(I2C2_BASE));
    return I2CMasterDataGet(I2C2_BASE);

}

/* Read data 1 high register of light*/
uint8_t read_data1high(void){

    I2CMasterSlaveAddrSet(I2C2_BASE, APDS9301_ADDRESS, false);
    I2CMasterDataPut(I2C2_BASE, COMMAND | DATA1H_REG);
    I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_SINGLE_SEND);
    while(I2CMasterBusy(I2C2_BASE));
    I2CMasterSlaveAddrSet(I2C2_BASE, APDS9301_ADDRESS, true);
    I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);
    while(I2CMasterBusy(I2C2_BASE));
    return I2CMasterDataGet(I2C2_BASE);

}

/*write to timing register. It returns 0x03*/
uint8_t write_timereg(void){
    //send
    I2CMasterSlaveAddrSet(I2C2_BASE, APDS9301_ADDRESS, false);
    I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_BURST_SEND_START);
    I2CMasterDataPut(I2C2_BASE, COMMAND | TIMING_REG);
    while(I2CMasterBusy(I2C2_BASE));
    I2CMasterSlaveAddrSet(I2C2_BASE, APDS9301_ADDRESS, false);
    I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
    I2CMasterDataPut(I2C2_BASE, MAX_GAIN | TIME_402ms);
    while(I2CMasterBusy(I2C2_BASE));

    //Recieve
    I2CMasterSlaveAddrSet(I2C2_BASE, APDS9301_ADDRESS, false);
    I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_SINGLE_SEND);
    I2CMasterDataPut(I2C2_BASE, COMMAND | TIMING_REG);
    while(I2CMasterBusy(I2C2_BASE));
    I2CMasterSlaveAddrSet(I2C2_BASE, APDS9301_ADDRESS, true);
    I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);
    while(I2CMasterBusy(I2C2_BASE));
    return I2CMasterDataGet(I2C2_BASE);
}

/*get calibrated luminosity */
float get_luminosity(void){
    uint16_t data0_low, data0_high, data1_high, data1_low = 0;
    float data0, data1;
    float luminosity = 0;

    data0_low = read_data0low();
    data0_high = read_data0high();
    data1_low = read_data1low();
    data1_high = read_data1high();

    data0 =  (data0_high <<8 | data0_low);
    data1 =  (data1_high <<8 | data1_low);

    float adc =  data1/data0;

	/* According to data sheet*/
    if(adc>0 && adc <= 0.5)
        return luminosity = (0.0304 * data0) - (0.062 * data1 * powf(adc, 1.4));
    else if(adc>0.5 && adc<=0.61)
        return luminosity = (0.0224 * data0) - (0.031 * data1);
    else if((adc>0.61)&&(adc<=0.80))
        return luminosity= (0.0128 * data0) - (0.0153 * data1);
    else if((adc>0.80) && (adc<=1.30))
        return luminosity= (0.00146 * data0) - (0.00112 * data1);
    else if(adc > 1.30)
        return luminosity=0;
    return -1;
}
/* @breif: Write to any register any value
	@param: reg is the register
	@param: value is the value of register
	*/
int write_reg(uint8_t reg, uint8_t value){

    I2CMasterSlaveAddrSet(I2C2_BASE, APDS9301_ADDRESS, false);
    I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_BURST_SEND_START);
    I2CMasterDataPut(I2C2_BASE, COMMAND | reg);
    while(I2CMasterBusy(I2C2_BASE));
    I2CMasterSlaveAddrSet(I2C2_BASE, APDS9301_ADDRESS, false);
    I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
    I2CMasterDataPut(I2C2_BASE, value);
    while(I2CMasterBusy(I2C2_BASE));
    return 1;
}
/* @breif: read from any register 
	@param: reg is the register
	@return:value of register
	*/
int read_reg(uint8_t reg){

    I2CMasterSlaveAddrSet(I2C2_BASE, APDS9301_ADDRESS, false);
    I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_SINGLE_SEND);
    I2CMasterDataPut(I2C2_BASE, COMMAND | reg);
    while(I2CMasterBusy(I2C2_BASE));
    I2CMasterSlaveAddrSet(I2C2_BASE, APDS9301_ADDRESS, true);
    I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);
    while(I2CMasterBusy(I2C2_BASE));
    return I2CMasterDataGet(I2C2_BASE);
}

