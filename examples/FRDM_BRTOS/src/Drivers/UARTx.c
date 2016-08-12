#include "UARTx.h"
#include "BRTOS.h"
#include "xsysctl.h"
#include "xgpio.h"
#include "xwdt.h"
#include "xhw_memmap.h"
#include "xuart.h"
#include "xhw_uart.h"
#include "xcore.h"
#include "MKL25Z4.h"
#include "device.h"


BRTOS_Sem   *SerialTX[3];
// Declara um ponteiro para o bloco de controle da Porta Serial
BRTOS_Queue  *SerialQ[3];


//BRTOS_Sem   *SerialTX1;
// Declara um ponteiro para o bloco de controle da Porta Serial
//BRTOS_Queue  *Serial1;

//BRTOS_Sem   *SerialTX2;
// Declara um ponteiro para o bloco de controle da Porta Serial
//BRTOS_Queue  *Serial2;

unsigned long USART0IntHandler(void *pvCBData,
        unsigned long ulEvent,
        unsigned long ulMsgParam,
        void *pvMsgData)
{
	char receive_byte;
	(void)pvCBData;
	(void)ulMsgParam;
	(void)pvMsgData;

	if ((ulEvent & UART_EVENT_RX) == UART_EVENT_RX)
	{
		receive_byte = xHWREGB(UART0_BASE + UART_012_D);

		if (OSQueuePost(SerialQ[0], receive_byte) == BUFFER_UNDERRUN)
		{
			// Problema: Estouro de buffer
			//OSCleanQueue(Serial0);
		}
	}

	if ((ulEvent & UART_EVENT_TC) == UART_EVENT_TC)
	{
		if ((xHWREGB(UART0_BASE + UART_012_C2) & UART_EVENT_TC) == UART_EVENT_TC)
		{
			UARTIntDisable(UART0_BASE, UART_INT_TC);
			(void)OSSemPost(SerialTX[0]);
		}
	}

	// ************************
	// Interrupt Exit
	// ************************
	OS_INT_EXIT_EXT();
	// ************************

	return 0;
}

unsigned long USART1IntHandler(void *pvCBData,
        unsigned long ulEvent,
        unsigned long ulMsgParam,
        void *pvMsgData)
{
	char receive_byte;
	(void)pvCBData;
	(void)ulMsgParam;
	(void)pvMsgData;

	if ((ulEvent & UART_EVENT_RX) == UART_EVENT_RX)
	{
		receive_byte = xHWREGB(UART1_BASE + UART_012_D);

		if (OSQueuePost(SerialQ[1], receive_byte) == BUFFER_UNDERRUN)
		{
			// Problema: Estouro de buffer
			//OSCleanQueue(Serial0);
		}
	}

	if ((ulEvent & UART_EVENT_TC) == UART_EVENT_TC)
	{
		if ((xHWREGB(UART1_BASE + UART_012_C2) & UART_EVENT_TC) == UART_EVENT_TC)
		{
			UARTIntDisable(UART1_BASE, UART_INT_TC);
			(void)OSSemPost(SerialTX[1]);
		}
	}

	// ************************
	// Interrupt Exit
	// ************************
	OS_INT_EXIT_EXT();
	// ************************

	return 0;
}

unsigned long USART2IntHandler(void *pvCBData,
        unsigned long ulEvent,
        unsigned long ulMsgParam,
        void *pvMsgData)
{
	char receive_byte;
	(void)pvCBData;
	(void)ulMsgParam;
	(void)pvMsgData;

	if ((ulEvent & UART_EVENT_RX) == UART_EVENT_RX)
	{
		receive_byte = xHWREGB(UART2_BASE + UART_012_D);

		if (OSQueuePost(SerialQ[2], receive_byte) == BUFFER_UNDERRUN)
		{
			// Problema: Estouro de buffer
			//OSCleanQueue(Serial0);
		}
	}

	if ((ulEvent & UART_EVENT_TC) == UART_EVENT_TC)
	{
		if ((xHWREGB(UART2_BASE + UART_012_C2) & UART_EVENT_TC) == UART_EVENT_TC)
		{
			UARTIntDisable(UART2_BASE, UART_INT_TC);
			(void)OSSemPost(SerialTX[2]);
		}
	}

	// ************************
	// Interrupt Exit
	// ************************
	OS_INT_EXIT_EXT();
	// ************************

	return 0;
}


void Init_UART0(void *parameters)
{
	uart_config_t *uart_conf = (uart_config_t *)parameters;
	xtEventCallback UART0_INT_HANDLE = USART0IntHandler;
	unsigned long config = 0;

	// Enable GPIO and UART Clock
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

	// Remap UART pin to GPIO Port UART0_RX --> PA2 UART0_TX --> PA1
	xSPinTypeUART(UART0RX, PA1);
	xSPinTypeUART(UART0TX, PA2);

	// Set UART clock
	SysCtlPeripheralClockSourceSet(SYSCTL_PERIPH_UART0_S_MCGPLLCLK_2);

	// Disable UART Receive/Transmit
	UARTDisable(UART0_BASE, UART_TX | UART_RX);

	config = UART_CONFIG_SAMPLE_RATE_DEFAULT;

	switch (uart_conf->parity){
		case UART_PAR_NONE:
			config |= UART_CONFIG_PAR_NONE;
			break;
		case UART_PAR_EVEN:
			config |= UART_CONFIG_PAR_EVEN | UART_CONFIG_WLEN_9;
			break;
		case UART_PAR_ODD:
			config |= UART_CONFIG_PAR_ODD | UART_CONFIG_WLEN_9;
			break;
		default:
			config |= UART_CONFIG_PAR_NONE;
			break;
	}

	switch (uart_conf->stop_bits){
		case UART_STOP_ONE:
			config |= UART_CONFIG_STOP_1;
			break;
		case UART_STOP_TWO:
			config |= UART_CONFIG_STOP_2;
			break;
		default:
			config |= UART_CONFIG_STOP_1;
			break;
	}

	// Configure UART Baud
	UARTConfigSet(UART0_BASE, uart_conf->baudrate, config);

	if (OSSemCreate(0, &SerialTX[0]) != ALLOC_EVENT_OK)
	{
	  // Oh Oh
	  // Não deveria entrar aqui !!!
	  while(1){};
	};

	if (OSQueueCreate(uart_conf->queue_size, &SerialQ[0]) != ALLOC_EVENT_OK)
	{
	  // Oh Oh
	  // Não deveria entrar aqui !!!
	  while(1){};
	};

	UARTIntEnable(UART0_BASE, UART_INT_R);
	UARTIntCallbackInit(UART0_BASE, UART0_INT_HANDLE);

	// Enable UART Receive and Transmit
	UARTEnable(UART0_BASE, UART_TX | UART_RX);

	xIntEnable(INT_UART0);
}



void Init_UART1(void *parameters)
{
	uart_config_t *uart_conf = (uart_config_t *)parameters;
	xtEventCallback UART1_INT_HANDLE = USART1IntHandler;
	unsigned long config = 0;

	// Enable GPIO and UART Clock
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

	// Remap UART pin to GPIO Port UART0_RX --> PA2 UART0_TX --> PA1
	xSPinTypeUART(UART1RX, PE1);
	xSPinTypeUART(UART1TX, PE0);

	// Set UART clock
	SysCtlPeripheralClockSourceSet(SYSCTL_PERIPH_UART0_S_MCGPLLCLK_2);

	// Disable UART Receive/Transmit
	UARTDisable(UART1_BASE, UART_TX | UART_RX);

	config = UART_CONFIG_SAMPLE_RATE_DEFAULT;

	switch (uart_conf->parity){
		case UART_PAR_NONE:
			config |= UART_CONFIG_PAR_NONE;
			break;
		case UART_PAR_EVEN:
			config |= UART_CONFIG_PAR_EVEN | UART_CONFIG_WLEN_9;
			break;
		case UART_PAR_ODD:
			config |= UART_CONFIG_PAR_ODD | UART_CONFIG_WLEN_9;
			break;
		default:
			config |= UART_CONFIG_PAR_NONE;
			break;
	}

	switch (uart_conf->stop_bits){
		case UART_STOP_ONE:
			config |= UART_CONFIG_STOP_1;
			break;
		case UART_STOP_TWO:
			config |= UART_CONFIG_STOP_2;
			break;
		default:
			config |= UART_CONFIG_STOP_1;
			break;
	}

	// Configure UART Baud
	UARTConfigSet(UART1_BASE, uart_conf->baudrate, config);


	if (OSSemCreate(0, &SerialTX[1]) != ALLOC_EVENT_OK)
	{
	  // Oh Oh
	  // Não deveria entrar aqui !!!
	  while(1){};
	};

	if (OSQueueCreate(uart_conf->queue_size, &SerialQ[1]) != ALLOC_EVENT_OK)
	{
	  // Oh Oh
	  // Não deveria entrar aqui !!!
	  while(1){};
	};

	UARTIntEnable(UART1_BASE, UART_INT_R);
	UARTIntCallbackInit(UART1_BASE, UART1_INT_HANDLE);

	// Enable UART Receive and Transmit
	UARTEnable(UART1_BASE, UART_TX | UART_RX);

	xIntEnable(INT_UART1);
}

void Init_UART2(void *parameters)
{
	uart_config_t *uart_conf = (uart_config_t *)parameters;
	xtEventCallback UART2_INT_HANDLE = USART2IntHandler;
	unsigned long config = 0;

	// Enable GPIO and UART Clock
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART2);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

	// Remap UART pin to GPIO Port UART0_RX --> PA2 UART0_TX --> PA1
	xSPinTypeUART(UART2RX, PE23);
	xSPinTypeUART(UART2TX, PE22);

	// Set UART clock
	SysCtlPeripheralClockSourceSet(SYSCTL_PERIPH_UART0_S_MCGPLLCLK_2);

	// Disable UART Receive/Transmit
	UARTDisable(UART2_BASE, UART_TX | UART_RX);

	config = UART_CONFIG_SAMPLE_RATE_DEFAULT;

	switch (uart_conf->parity){
		case UART_PAR_NONE:
			config |= UART_CONFIG_PAR_NONE;
			break;
		case UART_PAR_EVEN:
			config |= UART_CONFIG_PAR_EVEN | UART_CONFIG_WLEN_9;
			break;
		case UART_PAR_ODD:
			config |= UART_CONFIG_PAR_ODD | UART_CONFIG_WLEN_9;
			break;
		default:
			config |= UART_CONFIG_PAR_NONE;
			break;
	}

	switch (uart_conf->stop_bits){
		case UART_STOP_ONE:
			config |= UART_CONFIG_STOP_1;
			break;
		case UART_STOP_TWO:
			config |= UART_CONFIG_STOP_2;
			break;
		default:
			config |= UART_CONFIG_STOP_1;
			break;
	}

	// Configure UART Baud
	UARTConfigSet(UART2_BASE, uart_conf->baudrate, config);


	if (OSSemCreate(0, &SerialTX[2]) != ALLOC_EVENT_OK)
	{
	  // Oh Oh
	  // Não deveria entrar aqui !!!
	  while(1){};
	};

	if (OSQueueCreate(uart_conf->queue_size, &SerialQ[2]) != ALLOC_EVENT_OK)
	{
	  // Oh Oh
	  // Não deveria entrar aqui !!!
	  while(1){};
	};

	UARTIntEnable(UART2_BASE, UART_INT_R);
	UARTIntCallbackInit(UART2_BASE, UART2_INT_HANDLE);

	// Enable UART Receive and Transmit
	UARTEnable(UART2_BASE, UART_TX | UART_RX);

	xIntEnable(INT_UART2);
}



size_t UART_Write(OS_Device_Control_t *dev, char *string, size_t size ){
	size_t nbytes = 0;
	while(size){
		xHWREGB(dev->device->base_address + UART_012_D) = (char)*string;
		UARTIntEnable(dev->device->base_address, UART_INT_TC);
		if (dev->device->base_address == UART0_BASE)
		{
			if (OSSemPend(SerialTX[dev->device_number],5) != OK) goto failed_tx;
			nbytes++;
		}
		size--;
		string++;
	}
failed_tx:
	return nbytes;
}

size_t UART_Read(OS_Device_Control_t *dev, char *string, size_t size ){
	size_t nbytes = 0;
	uart_config_t *uart_conf = (uart_config_t *)dev->device->DriverData;
	while(nbytes < size){
		if (OSQueuePend(SerialQ[dev->device_number], (uint8_t*)string, uart_conf->timeout) != READ_BUFFER_OK) goto failed_rx;
		string++;
		nbytes++;
	}
failed_rx:
	return nbytes;
}

size_t UART_Set(OS_Device_Control_t *dev, uint32_t request, uint32_t value){
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

size_t UART_Get(OS_Device_Control_t *dev, uint32_t request){
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

const device_api_t UART_api ={
		.read = (Device_Control_read_t)UART_Read,
		.write = (Device_Control_write_t)UART_Write,
		.set = (Device_Control_set_t)UART_Set,
		.get = (Device_Control_get_t)UART_Get
};

void OSOpenUART(void *pdev, void *parameters){
	OS_Device_Control_t *dev = pdev;
	switch(dev->device_number){
		case 0:
			Init_UART0(parameters);
			dev->device->base_address = UART0_BASE;
			break;
		case 1:
			Init_UART1(parameters);
			dev->device->base_address = UART1_BASE;
			break;
		case 2:
			Init_UART2(parameters);
			dev->device->base_address = UART2_BASE;
			break;
		default:
			break;
	}
	dev->api = &UART_api;
}




