/*
 * OSDevConfig.h
 *
 *  Created on: 9 de ago de 2016
 *      Author: gustavo
 */

#ifndef CONFIG_OSDEVCONFIG_H_
#define CONFIG_OSDEVCONFIG_H_

#define MAX_INSTALLED_DEVICES 	5
#define AVAILABLE_DEVICES_TYPES	3

#define DRIVER_NAMES {"UART", "SPI", "I2C", "GPIO","AD","DA","PWM"}
typedef enum
{
	UART_TYPE = 0,
	SPI_TYPE,
	I2C_TYPE,
	GPIO_TYPE,
	AD_TYPE,
	DA_TYPE,
	PWM_TYPE,
	END_TYPE
} Device_Types_t;


/* Drivers disponiveis */
void OSOpenUART(void *pdev, void *parameters);
void OSOpenGPIO(void *pdev, void *parameters);
void OSOpenPWM(void *pdev, void *parameters);
#define DRIVER_LIST {{UART_TYPE, OSOpenUART}, {GPIO_TYPE, OSOpenGPIO},{PWM_TYPE, OSOpenPWM}}


/* Específico para o PWM */
#define PMW_0_MUX_PIN_LIST {{GPIOE_BASE,GPIO_PE29_TIM0CH2,GPIO_PIN_29},{GPIOE_BASE,GPIO_PE30_TIM0CH3,GPIO_PIN_30}}

#endif /* CONFIG_OSDEVCONFIG_H_ */
