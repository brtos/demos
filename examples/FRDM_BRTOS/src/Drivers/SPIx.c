#include "BRTOS.h"
#include "device.h"
#include "drivers.h"

#define NUM_SPI		1

static BRTOS_Sem   *SPITX[NUM_SPI];
static BRTOS_Queue *SPIQ[NUM_SPI];
static BRTOS_Mutex *SPIMutex[NUM_SPI];

char SPI_buffer;
static uint32_t SPIHandler(void)
{
	char c = SPI_buffer;
	OSQueuePost(SPIQ[0], c);
	return TRUE;
}


static void Init_SPI(void *parameters)
{
	spi_config_t *spi_conf = (spi_config_t *)parameters;

	ConfiguraInterruptHandler( INTERRUPT_SPI, SPIHandler);

	// todo: Configure SPI Baud
	//SPIConfigSet(spi_conf->baudrate, config);

	assert(OSSemCreate(0, &SPITX[0]) == ALLOC_EVENT_OK);
	assert(OSQueueCreate(spi_conf->queue_size, &SPIQ[0]) == ALLOC_EVENT_OK);

	if (spi_conf->mutex == true)
	{
		OSMutexCreate (&SPIMutex[0], 0);
	}
}

#define spi_rwchar(x)

static size_t SPI_Write(OS_Device_Control_t *dev, char *string, size_t size ){
	size_t nbytes = 0;

	while(size)
	{
		spi_rwchar((char)string);
		nbytes++;
		size--;
		string++;
	}
	return nbytes;
}

static size_t SPI_Read(OS_Device_Control_t *dev, char *string, size_t size ){
	size_t nbytes = 0;
	spi_config_t *spi_conf = (spi_config_t *)dev->device->DriverData;
	while(nbytes < size)
	{
		if (OSQueuePend(SPIQ[dev->device_number], (uint8_t*)string, spi_conf->timeout) != READ_BUFFER_OK) goto failed_rx;
		string++;
		nbytes++;
	}
failed_rx:
	return nbytes;
}

static size_t SPI_Set(OS_Device_Control_t *dev, uint32_t request, uint32_t value){

	size_t ret = 0;
	spi_config_t *spi_conf = (spi_config_t *)dev->device->DriverData;

	switch(request){
		case SPI_BAUDRATE:
			spi_conf->baudrate = value;
			break;
		case SPI_POLARITY:
			spi_conf->polarity = value;
			break;
		case SPI_QUEUE_SIZE:
			/* somente pode ser alterado se filar dinâmicas forem utilizadas. */
			break;
		case SPI_TIMEOUT:
			spi_conf->timeout = value;
			break;
		case CTRL_ACQUIRE_WRITE_MUTEX:
			if (SPIMutex[dev->device_number] != NULL){
				ret = OSMutexAcquire(SPIMutex[dev->device_number],value);
			}
			break;
		case CTRL_RELEASE_WRITE_MUTEX:
			if (SPIMutex[dev->device_number] != NULL){
				ret = OSMutexRelease(SPIMutex[dev->device_number]);
			}
			break;
		default:
			break;
	}

	return ret;
}

static size_t SPI_Get(OS_Device_Control_t *dev, uint32_t request){
	uint32_t ret;
	spi_config_t *spi_conf = (spi_config_t *)dev->device->DriverData;

	switch(request){
		case SPI_BAUDRATE:
			ret = spi_conf->baudrate;
			break;
		case SPI_POLARITY:
			ret = spi_conf->polarity;
			break;
		case SPI_QUEUE_SIZE:
			ret = spi_conf->queue_size;
			break;
		case SPI_TIMEOUT:
			ret = spi_conf->timeout;
			break;
		default:
			ret = 0;
			break;
	}
	return ret;
}

static const device_api_t SPI_api =
{
		.read = (Device_Control_read_t)SPI_Read,
		.write = (Device_Control_write_t)SPI_Write,
		.set = (Device_Control_set_t)SPI_Set,
		.get = (Device_Control_get_t)SPI_Get
};

void OSOpenSPI(void *pdev, void *parameters)
{
	OS_Device_Control_t *dev = pdev;
	Init_SPI(parameters);
	dev->api = &SPI_api;
}



