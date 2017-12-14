/**********************************************************************
*@Filename:test_comm.c
*
*@Description:This is a test for library of UART7 
*@Author:Sai Raghavendra Sankrantipati, Vishal Vishnani
*@Date:13/12/2017
*@Usage : Connect PC5 and PC4 as RX and TX and check library functions to send and recieve data over UART 7
 **********************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "comm.h"
#include "utils/uartstdio.h"

//Init UART7
int uart_init(){
    // Enable the GPIO port that is used for the on-board LED.
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);

	// Enable the GPIO pins for the LED (PN0)
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0);
	// Enable the peripherals.
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART7);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	// Enable processor interrupts.
    ROM_IntMasterEnable();
	// Set GPIO PC4 and PC5 as UART pins.
    GPIOPinConfigure(GPIO_PC4_U7RX);
    GPIOPinConfigure(GPIO_PC5_U7TX);
    ROM_GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5);
	// Configure the UART for 115,200, 8-N-1 operation.
    ROM_UARTConfigSetExpClk(UART7_BASE, 120000000, 57600,
                            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                             UART_CONFIG_PAR_NONE));

	// Enable the UART interrupt.
    ROM_IntEnable(INT_UART7);
    UARTIntRegister(UART7_BASE, UARTIntHandler);
    ROM_UARTIntEnable(UART7_BASE, UART_INT_RX | UART_INT_RT);
    return 1;
}

// The UART interrupt handler.
void UARTIntHandler(void)
{
    uint32_t ui32Status;
	 // Get the interrrupt status
    ui32Status = ROM_UARTIntStatus(UART7_BASE, true);
	 // Clear the asserted interrupts.
    ROM_UARTIntClear(UART7_BASE, ui32Status);
/*
	// Loop while there are characters in the receive FIFO.
    while(ROM_UARTCharsAvail(UART7_BASE))
    {
		// Blink the LED to show a character transfer is occuring.
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, GPIO_PIN_0);
		// Read the next character from the UART and write it back to the data.
        data = ROM_UARTCharGetNonBlocking(UART7_BASE);
        SysCtlDelay(120000000 / (1000 * 3));
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, 0);
    }
*/
}


// Send a string to the UART.
void UARTSend(const uint8_t *pui8Buffer, uint32_t ui32Count)
{
	// Loop while there are more characters to send.
    while(ui32Count--)
    {
		// Write the next character to the UART.
         ROM_UARTCharPutNonBlocking(UART7_BASE, *pui8Buffer++);
    }
}

