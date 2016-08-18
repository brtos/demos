/*
 * PWMx.c
 *
 *  Created on: 18 de ago de 2016
 *      Author: gustavo
 */

#include "xhw_types.h"
#include "BRTOS.h"
#include "xsysctl.h"
#include "xgpio.h"
#include "xhw_memmap.h"
#include "xcore.h"
#include "xpwm.h"
#include "xhw_tpm.h"
#include "xhw_nvic.h"
#include "MKL25Z4.h"
#include "device.h"

static size_t PWM_Set(OS_Device_Control_t *dev, uint32_t request, uint32_t value){
	//unsigned long config = 0;
	size_t ret = 0;
	//uart_config_t *uart_conf = (uart_config_t *)dev->device->DriverData;

	return ret;
}

static size_t PWM_Get(OS_Device_Control_t *dev, uint32_t request){
	uint32_t ret;
	uart_config_t *uart_conf = (uart_config_t *)dev->device->DriverData;

	switch(request){
		case UART_BAUDRATE:
			ret = uart_conf->baudrate;
			break;
		case UART_PARITY:
			ret = uart_conf->parity;
			break;
		case UART_STOP_BITS:
			ret = uart_conf->stop_bits;
			break;
		case UART_QUEUE_SIZE:
			ret = uart_conf->queue_size;
			break;
		case UART_TIMEOUT:
			ret = uart_conf->timeout;
			break;
		default:
			ret = 0;
			break;
	}
	return ret;
}

static const device_api_t PWM_api ={
		.read = (Device_Control_read_t)NULL,
		.write = (Device_Control_write_t)NULL,
		.set = (Device_Control_set_t)PWM_Set,
		.get = (Device_Control_get_t)PWM_Get
};


void Init_PWM(unsigned long base, void *parameters){
	pwm_config_t *pwm_conf = (pwm_config_t *)parameters;
	int i = 0;
	mux_gpio_config_t *mux_gpio;
#ifdef PMW_0_MUX_PIN_LIST
	mux_gpio_config_t mux_gpio_0[] = PMW_0_MUX_PIN_LIST;
#endif
#ifdef PMW_1_MUX_PIN_LIST
	mux_gpio_config_t mux_gpio_1[] = PMW_1_MUX_PIN_LIST;
#endif
#ifdef PMW_2_MUX_PIN_LIST
	mux_gpio_config_t mux_gpio_2[] = PMW_2_MUX_PIN_LIST;
#endif

	//
    // Select the peripheral clock source
	int size;
    if (base == xPWMA_BASE)
    {
        xSysCtlPeripheralClockSourceSet(xSYSCTL_PWMA_MAIN, 1);
        xSysCtlPeripheralEnable(SYSCTL_PERIPH_PWMA);
		#ifdef PMW_0_MUX_PIN_LIST
        size = sizeof(mux_gpio_0)/sizeof(mux_gpio_config_t);
        mux_gpio = mux_gpio_0;
		#endif
    }
    if (base == xPWMB_BASE)
    {
        xSysCtlPeripheralClockSourceSet(xSYSCTL_PWMB_MAIN, 1);
        xSysCtlPeripheralEnable(SYSCTL_PERIPH_PWMB);
		#ifdef PMW_1_MUX_PIN_LIST
        size = sizeof(mux_gpio_1)/sizeof(mux_gpio_config_t);
        mux_gpio = mux_gpio_1;
		#endif
    }
    if (base == xPWMC_BASE)
    {
        xSysCtlPeripheralClockSourceSet(xSYSCTL_PWMC_MAIN, 1);
        xSysCtlPeripheralEnable(SYSCTL_PERIPH_PWMC);
		#ifdef PMW_2_MUX_PIN_LIST
        size = sizeof(mux_gpio_2)/sizeof(mux_gpio_config_t);
        mux_gpio = mux_gpio_2;
		#endif
    }

    unsigned long tmp;
    for(i=0;i<size;i++){
    	tmp = ((mux_gpio[i].pin & 0x00004000) >> 12);
    	switch(tmp){
    		case 0:
    	    	xSysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    			break;
    		case 1:
    	    	xSysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    			break;
    		case 2:
    	    	xSysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    			break;
    		case 3:
    	    	xSysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    			break;
    		case 4:
    	    	xSysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    			break;
    		default:
    			break;
    	}
        //
        // Mux configuration of GPIO Pin as PWM
        GPIOPinConfigure(mux_gpio[i].pin);

        GPIOPadConfigSet(mux_gpio[i].base, mux_gpio[i].pin_number, PORT_TYPE_DSE_HIGH);
        GPIOSlewRataConfigure(mux_gpio[i].base, mux_gpio[i].pin_number,PORT_TYPE_SRE_SLOW);
    }


    for(i=0;i<size;i++){
        // Set operation mode
        PWMInitConfigure(base, *(pwm_conf->channel_list),pwm_conf->mode);

        //
        // Set CNR, Prescale and Divider.
        xPWMFrequencySet(base, *(pwm_conf->channel_list), pwm_conf->frequency, pwm_conf->mode);

        //
        // Set CMR
        xPWMDutySet(base, *(pwm_conf->channel_list), *(pwm_conf->init_duty));

        //
        // Set output enable
        xPWMOutputEnable(base, *(pwm_conf->channel_list));

        pwm_conf->channel_list++;
        pwm_conf->init_duty++;
    }


    // Enable PWM TOF Interrupt
    //xPWMIntEnable(base, xPWM_CHANNEL3, PWM_INT_TOF);
    //xIntEnable(INT_TPM0);

    //
    // start pwm
    PWMStart(base);
}


void OSOpenPWM(void *pdev, void *parameters){
	OS_Device_Control_t *dev = pdev;
	switch(dev->device_number){
		case 0:
			Init_PWM(xPWMA_BASE, parameters);
			dev->device->base_address = xPWMA_BASE;
			break;
		case 1:
			Init_PWM(xPWMB_BASE, parameters);
			dev->device->base_address = xPWMB_BASE;
			break;
		case 2:
			Init_PWM(xPWMC_BASE, parameters);
			dev->device->base_address = xPWMC_BASE;
			break;
		default:
			break;
	}
	dev->api = &PWM_api;
}
