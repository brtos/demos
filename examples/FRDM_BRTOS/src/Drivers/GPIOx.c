/*
 * GPIOx.c
 *
 *  Created on: 10 de ago de 2016
 *      Author: gustavo
 */

#include "xhw_types.h"
#include "BRTOS.h"
#include "xsysctl.h"
#include "xgpio.h"
#include "xhw_memmap.h"
#include "device.h"

static size_t GPIO_Write(OS_Device_Control_t *dev, void *string, size_t size ){
	uint32_t temp = (uint32_t)string;
	GPIOPinWrite(dev->device->base_address, temp, size);
	return 0;
}

static size_t GPIO_Read(OS_Device_Control_t *dev, char *string, size_t size ){
	uint32_t temp = (uint32_t)string;
	(void)size;
	return GPIOPinRead(dev->device->base_address,temp);
}

static size_t GPIO_Set(OS_Device_Control_t *dev, uint32_t request, uint32_t value){
	uart_config_t *uart_conf = (uart_config_t *)dev->device->DriverData;

	switch(request){
		case UART_BAUDRATE:
			break;
		case UART_PARITY:
			break;
		case UART_STOP_BITS:
			break;
		case UART_QUEUE_SIZE:
			break;
		case UART_TIMEOUT:
			uart_conf->timeout = value;
			break;
		default:
			break;
	}

	return 0;
}

static size_t GPIO_Get(OS_Device_Control_t *dev, uint32_t request){
	uint32_t ret;
	gpio_config_t *uart_conf = (gpio_config_t *)dev->device->DriverData;
	switch(request){
		default:
			ret = 0;
			break;
	}
	return ret;
}

const device_api_t GPIO_api ={
		.read = (Device_Control_read_t)GPIO_Read,
		.write = (Device_Control_write_t)GPIO_Write,
		.set = (Device_Control_set_t)GPIO_Set,
		.get = (Device_Control_get_t)GPIO_Get
};

void OSOpenGPIO(void *pdev, void *parameters){
	int i = 0;
	unsigned long pins;
	unsigned long mux = 0;
	gpio_config_t *gpio_conf = (gpio_config_t *)parameters;
	OS_Device_Control_t *dev = pdev;
	switch(dev->device_number){
		case 'A':
			SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
			dev->device->base_address = GPIOA_BASE;
			mux = 0x00000001;
			break;
		case 'B':
			SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
			dev->device->base_address = GPIOB_BASE;
			mux = 0x00001001;
			break;
		case 'C':
			SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
			dev->device->base_address = GPIOC_BASE;
			mux = 0x00002001;
			break;
		case 'D':
			SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
			dev->device->base_address = GPIOD_BASE;
			mux = 0x00003001;
			break;
		case 'E':
			SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
			dev->device->base_address = GPIOD_BASE;
			mux = 0x00004001;
			break;
		default:
			break;
	}

	// Config mux function
	pins = 1;
	for(i=0;i<=19;i++){
		if ((pins & gpio_conf->used_pins_out) == pins){
			GPIOPinConfigure(mux | (i << 4));
		}
		pins = pins << 1;
	}
	pins = 1;
	for(i=0;i<=19;i++){
		if ((pins & gpio_conf->used_pins_in) == pins){
			GPIOPinConfigure(mux | (i << 4));
		}
		pins = pins << 1;
	}
	GPIOPadConfigSet (dev->device->base_address, gpio_conf->used_pins_out, PORT_TYPE_DSE_HIGH);
	GPIOPinReset(dev->device->base_address, gpio_conf->used_pins_out);
	xGPIODirModeSet(dev->device->base_address, gpio_conf->used_pins_out, xGPIO_DIR_MODE_OUT);
	xGPIODirModeSet(dev->device->base_address, gpio_conf->used_pins_in, xGPIO_DIR_MODE_IN);

	dev->api = &GPIO_api;

}
