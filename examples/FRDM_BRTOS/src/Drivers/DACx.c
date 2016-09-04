
#include "BRTOS.h"
#include "device.h"
#include "drivers.h"

#define NUM_DAC     1
static BRTOS_Sem   *DAC[NUM_DAC];
static BRTOS_Queue *DACQ[NUM_DAC];
static BRTOS_Mutex *DACRMutex[NUM_DAC];

#define NUM_DAC_CHAN   16
#define NUM_POINTS     256

uint16_t DAC_buffer[NUM_DAC_CHAN][NUM_POINTS];
uint16_t DAC_chan;
uint16_t DAC_chan_idx[NUM_DAC_CHAN];

struct DAC_signal
{
	int freq;
	int amp;
};

struct DAC_signal DAC_Signals[] = {{60,3}};

static int DAC_res = 8;
static int DAC_rate = 1000;

static void DAC_Config(int res, int rate)
{
	DAC_res = res;
	DAC_rate = rate;
}

static int DAC_Simulado(int chan, int val)
{
	uint16_t idx = DAC_chan_idx[chan];
	DAC_buffer[chan][idx++] = val;
	if(idx>=NUM_POINTS) idx = 0;
	DAC_chan_idx[chan] = idx;
	return 0;
}

static uint32_t DACHandler(void)
{
	uint8_t val;
	uint8_t chan = DAC_chan;
	if(NUM_DAC_CHAN < chan) return FALSE;
	OSRQueue((OS_QUEUE *)DACQ[0]->OSEventPointer, &val);
	DAC_Simulado(chan,val);
	return TRUE;
}

static int DAC_SetSample(uint8_t num, uint16_t val, ostick_t timeout)
{
	uint8_t chan = DAC_chan;
	if(NUM_DAC_CHAN < chan) return FALSE;
	DAC_Simulado(chan, val);
	return TRUE;
}

static void Init_DAC(void *parameters)
{
	dac_config_t *dac_conf = (dac_config_t *)parameters;

	ConfiguraInterruptHandler( INTERRUPT_DAC, DACHandler);

	assert(OSSemCreate(0, &DAC[0]) == ALLOC_EVENT_OK);
	assert(OSQueueCreate(dac_conf->queue_size, &DACQ[0]) == ALLOC_EVENT_OK);

	if (dac_conf->write_mutex == true)
	{
		OSMutexCreate (&DACRMutex[0], 0);
	}

	DAC_Config(dac_conf->resolution, dac_conf->samplerate);
}


static size_t DAC_Write(OS_Device_Control_t *dev, char *buf, size_t size ){
	size_t nbytes = 0;
	dac_config_t *dac_conf = (dac_config_t *)dev->device->DriverData;
	if(dac_conf->resolution > 8) size=size/2;
	while(nbytes < size)
	{
		if(dac_conf->polling_irq == DAC_IRQ)
		{
			uint8_t val = (*buf);
			if (OSWQueue((OS_QUEUE *)DACQ[dev->device_number]->OSEventPointer, val) != WRITE_BUFFER_OK) goto exit_on_error;
			if(dac_conf->resolution > 8)
			{
				val = (*(buf++));
				if (OSWQueue((OS_QUEUE *)DACQ[dev->device_number]->OSEventPointer, val) != WRITE_BUFFER_OK) goto exit_on_error;
			}
		}else
		{
			uint16_t val = 0;
			if(dac_conf->resolution > 8)
			{
				val = (uint16_t)(*buf<<8);
				val += (*(++buf));
			}else
			{
				val = (*buf);
			}
			if (DAC_SetSample(dev->device_number, val, dac_conf->timeout) != TRUE) goto exit_on_error;
		}
		buf++;
		nbytes++;
	}
exit_on_error:
	return nbytes;
}

static size_t DAC_Set(OS_Device_Control_t *dev, uint32_t request, uint32_t value){

	size_t ret = 0;
	dac_config_t *dac_conf = (dac_config_t *)dev->device->DriverData;

	switch(request){
		case DAC_SAMPLERATE:
			dac_conf->samplerate = value;
			break;
		case DAC_RESOLUTION:
			dac_conf->resolution = value;
			break;
		case DAC_QUEUE_SIZE:
			/* somente pode ser alterado se filas dinâmicas forem utilizadas. */
			break;
		case DAC_TIMEOUT:
			dac_conf->timeout = value;
			break;
		case CTRL_ACQUIRE_WRITE_MUTEX:
			if (DACRMutex[dev->device_number] != NULL)
			{
				ret = OSMutexAcquire(DACRMutex[dev->device_number],value);
			}
			break;
		case CTRL_RELEASE_WRITE_MUTEX:
			if (DACRMutex[dev->device_number] != NULL)
			{
				ret = OSMutexRelease(DACRMutex[dev->device_number]);
			}
			break;
		default:
			break;
	}

	return ret;
}

static size_t DAC_Get(OS_Device_Control_t *dev, uint32_t request){
	uint32_t ret;
	dac_config_t *dac_conf = (dac_config_t *)dev->device->DriverData;

	switch(request){
		case DAC_SAMPLERATE:
			ret = dac_conf->samplerate;
			break;
		case DAC_RESOLUTION:
			ret = dac_conf->resolution;
			break;
		case DAC_QUEUE_SIZE:
			ret = dac_conf->queue_size;
			break;
		case DAC_TIMEOUT:
			ret = dac_conf->timeout;
			break;
		default:
			ret = 0;
			break;
	}
	return ret;
}

static const device_api_t DAC_api ={
		.read = (Device_Control_read_t)NULL,
		.write = (Device_Control_write_t)DAC_Write,
		.set = (Device_Control_set_t)DAC_Set,
		.get = (Device_Control_get_t)DAC_Get
};

void OSOpenDAC(void *pdev, void *parameters)
{
	OS_Device_Control_t *dev = pdev;
	Init_DAC(parameters);
	dev->api = &DAC_api;
}



