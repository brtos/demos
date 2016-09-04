#include "BRTOS.h"
#include "device.h"
#include "drivers.h"

#define NUM_I2C		1

static BRTOS_Sem   *I2CTX[NUM_I2C];
static BRTOS_Queue *I2CQ[NUM_I2C];
static BRTOS_Mutex *I2CMutex[NUM_I2C];


char I2C_buffer;
static uint32_t I2CHandler(void)
{
	char c = I2C_buffer;
	OSQueuePost(I2CQ[0], c);
	return TRUE;
}


static void Init_I2C(void *parameters)
{
	i2c_config_t *i2c_conf = (i2c_config_t *)parameters;

	ConfiguraInterruptHandler( INTERRUPT_I2C, I2CHandler);

	// todo: Configure I2C Baud
	//I2CConfigSet(i2c_conf->baudrate, config);

	assert(OSSemCreate(0, &I2CTX[0]) == ALLOC_EVENT_OK);
	assert(OSQueueCreate(i2c_conf->queue_size, &I2CQ[0]) == ALLOC_EVENT_OK);

	if (i2c_conf->mutex == true)
	{
		OSMutexCreate (&I2CMutex[0], 0);
	}
}

#define i2c_rwchar(x)

static size_t I2C_Write(OS_Device_Control_t *dev, char *string, size_t size ){
	size_t nbytes = 0;

	while(size)
	{
		i2c_rwchar((char)string);
		nbytes++;
		size--;
		string++;
	}
	return nbytes;
}

static size_t I2C_Read(OS_Device_Control_t *dev, char *string, size_t size ){
	size_t nbytes = 0;
	i2c_config_t *i2c_conf = (i2c_config_t *)dev->device->DriverData;
	while(nbytes < size)
	{
		if (OSQueuePend(I2CQ[dev->device_number], (uint8_t*)string, i2c_conf->timeout) != READ_BUFFER_OK) goto failed_rx;
		string++;
		nbytes++;
	}
failed_rx:
	return nbytes;
}

static size_t I2C_Set(OS_Device_Control_t *dev, uint32_t request, uint32_t value){
	unsigned long config = 0;
	size_t ret = 0;
	i2c_config_t *i2c_conf = (i2c_config_t *)dev->device->DriverData;

	switch(request){
		case I2C_BAUDRATE:
			i2c_conf->baudrate = value;
			break;
		case I2C_DIRECTION:
			i2c_conf->direction = value;
			break;
		case I2C_QUEUE_SIZE:
			/* somente pode ser alterado se filar dinâmicas forem utilizadas. */
			break;
		case I2C_TIMEOUT:
			i2c_conf->timeout = value;
			break;
		case CTRL_ACQUIRE_WRITE_MUTEX:
			if (I2CMutex[dev->device_number] != NULL){
				ret = OSMutexAcquire(I2CMutex[dev->device_number],value);
			}
			break;
		case CTRL_RELEASE_WRITE_MUTEX:
			if (I2CMutex[dev->device_number] != NULL){
				ret = OSMutexRelease(I2CMutex[dev->device_number]);
			}
			break;
		default:
			break;
	}

	return ret;
}

static size_t I2C_Get(OS_Device_Control_t *dev, uint32_t request){
	uint32_t ret;
	i2c_config_t *i2c_conf = (i2c_config_t *)dev->device->DriverData;

	switch(request){
		case I2C_BAUDRATE:
			ret = i2c_conf->baudrate;
			break;
		case I2C_DIRECTION:
			ret = i2c_conf->direction;
			break;
		case I2C_QUEUE_SIZE:
			ret = i2c_conf->queue_size;
			break;
		case I2C_TIMEOUT:
			ret = i2c_conf->timeout;
			break;
		default:
			ret = 0;
			break;
	}
	return ret;
}

static const device_api_t I2C_api ={
		.read = (Device_Control_read_t)I2C_Read,
		.write = (Device_Control_write_t)I2C_Write,
		.set = (Device_Control_set_t)I2C_Set,
		.get = (Device_Control_get_t)I2C_Get
};

void OSOpenI2C(void *pdev, void *parameters){
	OS_Device_Control_t *dev = pdev;
	Init_I2C(parameters);
	dev->api = &I2C_api;
}



