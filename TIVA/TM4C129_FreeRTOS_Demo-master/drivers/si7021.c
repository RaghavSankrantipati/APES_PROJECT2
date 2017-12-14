/**********************************************************************
*@Filename:si7021.c
*
*@Description:This is a library for SI7021 Humidity and temperature sensor
*@Author:Sai Raghavendra Sankrantipati, Vishal Vishnani
*@Date:13/12/2017
*@Usage : Connect SI7021 9301 to I2C 2 and use any of the library function to read and write registers
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
#include "si7021.h"

/* Initialise sensor on I2C bus 0
 * */
uint8_t init_SI7021(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);
    SysCtlPeripheralReset(SYSCTL_PERIPH_I2C0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    GPIOPinConfigure(GPIO_PB2_I2C0SCL);
    GPIOPinConfigure(GPIO_PB3_I2C0SDA);
    GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);
    GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);
    //HWREG(I2C0_BASE + I2C_O_MCR) |= 0x01;
    I2CMasterInitExpClk(I2C0_BASE, 120000000U, false);
    //I2CMasterInitExpClk(I2C0_BASE, SYSTEM_CLOCK, false);
    //HWREG(I2C0_BASE + I2C_O_FIFOCTL) = 80008000;
    return 1;
}

//returns humidity on successful operation
float get_humidity(void){

    uint8_t hum_msb, hum_lsb;
    I2CMasterSlaveAddrSet(I2C0_BASE, SI7021_ADDRESS, false);
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND);
    I2CMasterDataPut(I2C0_BASE, COMMAND_HUM);
    while(I2CMasterBusy(I2C0_BASE));
    I2CMasterSlaveAddrSet(I2C0_BASE, SI7021_ADDRESS, true);
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_START);
    while(I2CMasterBusy(I2C0_BASE));
    hum_msb = I2CMasterDataGet(I2C0_BASE);
    I2CMasterSlaveAddrSet(I2C0_BASE, SI7021_ADDRESS, true);
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
    while(I2CMasterBusy(I2C0_BASE));
    hum_lsb = I2CMasterDataGet(I2C0_BASE);

    float humidity = (((hum_msb * 256 + hum_lsb) * 125.0) / 65536.0) - 6;

    return humidity;
}

//returns temperature on successful operation
float get_temperature(void){

    uint8_t temp_msb, temp_lsb;
    I2CMasterSlaveAddrSet(I2C0_BASE, SI7021_ADDRESS, false);
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND);
    I2CMasterDataPut(I2C0_BASE, COMMAD_TEMP);
    while(I2CMasterBusy(I2C0_BASE));
    I2CMasterSlaveAddrSet(I2C0_BASE, SI7021_ADDRESS, true);
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_START);
    while(I2CMasterBusy(I2C0_BASE));
    temp_msb = I2CMasterDataGet(I2C0_BASE);
    I2CMasterSlaveAddrSet(I2C0_BASE, SI7021_ADDRESS, true);
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
    while(I2CMasterBusy(I2C0_BASE));
    temp_lsb = I2CMasterDataGet(I2C0_BASE);

    float temperature = (((temp_msb * 256 + temp_lsb) * 175.72) / 65536.0) - 46.85;

    return temperature;
}

/*Return id of SI7021 Sensor*/
float get_id(void){

    uint8_t id;
    I2CMasterSlaveAddrSet(I2C0_BASE, SI7021_ADDRESS, false);
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);
    I2CMasterDataPut(I2C0_BASE, 0xC9);
    while(I2CMasterBusy(I2C0_BASE));

    I2CMasterSlaveAddrSet(I2C0_BASE, SI7021_ADDRESS, false);
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
    I2CMasterDataPut(I2C0_BASE, 0xFC);
    while(I2CMasterBusy(I2C0_BASE));

    I2CMasterSlaveAddrSet(I2C0_BASE, SI7021_ADDRESS, true);
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);
    while(I2CMasterBusy(I2C0_BASE));
    id = I2CMasterDataGet(I2C0_BASE);

    return id;
}

//Reset SI7021 sensor
int reset_SI7021(void){

    I2CMasterSlaveAddrSet(I2C0_BASE, SI7021_ADDRESS, false);
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);
    I2CMasterDataPut(I2C0_BASE, COMMAND_REST);
    while(I2CMasterBusy(I2C0_BASE));
    return 1;
}


