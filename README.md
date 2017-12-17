# Data Acquisition and Remotely Logged Home Automation System
Project 2 repository for APES ECEN 5013
A multithreaded home automation system in which, BeagleBone(BBG) is used as a remote logger system while TIVA acts as a sensor hub. The sensor hub collects data values from different sensors and sends them to BBG over UART communication

### Apparatus
* Beaglebone Green
* Tiva TM4C1294XL
* SI7021, Temperature sensor
* APDS9301, Light sensor
...Connect 2 sensors on I2C of TIVA. SI7021 on I2C0, APDS9301 on I2C2
...UART7 of Tiva is connected to UART1 of BBG

### Execution
* Use Code Composer Studio for the Tiva part of the project. Just build and run..
* Use make for BBG part

### Credit
[TIVA and FreeRTOS Demo](https://github.com/akobyl/TM4C129_FreeRTOS_Demo)
