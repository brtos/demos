/*
 * OSDevConfig.h
 *
 *  Created on: 9 de ago de 2016
 *      Author: gustavo
 */

#ifndef CONFIG_OSDEVCONFIG_H_
#define CONFIG_OSDEVCONFIG_H_

#define MAX_INSTALLED_DEVICES 	5
#define AVAILABLE_DEVICES_TYPES	2

#define DRIVER_NAMES {"UART", "SPI", "I2C", "GPIO"}
typedef enum
{
	UART_TYPE = 0,
	SPI_TYPE,
	I2C_TYPE,
	GPIO_TYPE,
	END_TYPE
} Device_Types_t;


/* Drivers disponiveis */
void OSOpenUART(void *pdev, void *parameters);
void OSOpenGPIO(void *pdev, void *parameters);
#define DRIVER_LIST {{UART_TYPE, OSOpenUART}, {GPIO_TYPE, OSOpenGPIO}}

#endif /* CONFIG_OSDEVCONFIG_H_ */
