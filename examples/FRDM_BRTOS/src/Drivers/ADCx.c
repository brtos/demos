#include "BRTOS.h"
#include "device.h"
#include "drivers.h"

#define NUM_ADC     1
static BRTOS_Sem   *ADC[NUM_ADC];
static BRTOS_Queue *ADCQ[NUM_ADC];
static BRTOS_Mutex *ADCRMutex[NUM_ADC];

#define NUM_ADC_CHAN   16
uint16_t ADC_buffer[NUM_ADC_CHAN];
uint16_t ADC_chan;

struct adc_signal
{
	int freq;
	int amp;
};

struct adc_signal ADC_Signals[] = {{60,3}};

static int ADC_res = 8;
static int ADC_rate = 1000;

#include "math.h"
static uint16_t quantizacao (double valor, double amp, int n)
{
    double D = 2*amp/n;
    double x = valor + amp;
    double q = floor(x/D);
    uint16_t y;

    y = (uint16_t)((D/2 + D*q - amp)*(n-1)/amp);
    return y;
}

static void ADC_Config(int res, int rate)
{
	ADC_res = res;
	ADC_rate = rate;
}

static int ADC_Simulado(int canal)
{
	int freq = ADC_Signals[canal].freq;
    int amp = ADC_Signals[canal].amp;
    ostick_t t = OSGetTickCount();

	double y = amp*sin(2*M_PI*freq*t/10000);
	return quantizacao(y, amp,(1<<ADC_res));
}

static uint32_t ADCHandler(void)
{
	uint8_t chan = ADC_chan;
	if(NUM_ADC_CHAN < chan) return FALSE;	
	uint16_t val = ADC_buffer[chan];
	OSQueuePost(ADCQ[0], val);
	return TRUE;
}

static int ADC_GetSample(uint8_t num, uint8_t* buf, ostick_t timeout)
{
	static uint16_t valor;
	static int adc_state = 0;
	if(adc_state == 0)
	{
		valor = ADC_Simulado(num);
		*buf = (uint8_t)(valor>>8);
		adc_state = 1;
	}else
	{
		adc_state = 0;
		*buf = (uint8_t)valor;
	}
	return TRUE;
}

static void Init_ADC(void *parameters)
{
	adc_config_t *adc_conf = (adc_config_t *)parameters;

	ConfiguraInterruptHandler( INTERRUPT_ADC, ADCHandler);

	assert(OSSemCreate(0, &ADC[0]) == ALLOC_EVENT_OK);
	assert(OSQueueCreate(adc_conf->queue_size, &ADCQ[0]) == ALLOC_EVENT_OK);

	if (adc_conf->read_mutex == true)
	{
		OSMutexCreate (&ADCRMutex[0], 0);
	}

	ADC_Config(adc_conf->resolution, adc_conf->samplerate);
}


static size_t ADC_Read(OS_Device_Control_t *dev, char *buf, size_t size ){
	size_t nbytes = 0;
	adc_config_t *adc_conf = (adc_config_t *)dev->device->DriverData;
	while(nbytes < size)
	{
		if(adc_conf->polling_irq == ADC_IRQ)
		{
			if (OSQueuePend(ADCQ[dev->device_number], (uint8_t*)buf, adc_conf->timeout) != READ_BUFFER_OK) goto exit_on_error;
		}else
		{
			if (ADC_GetSample(dev->device_number, (uint8_t*)buf, adc_conf->timeout) != TRUE) goto exit_on_error;
		}
		buf++;
		nbytes++;
	}
exit_on_error:
	return nbytes;
}

static size_t ADC_Set(OS_Device_Control_t *dev, uint32_t request, uint32_t value){

	size_t ret = 0;
	adc_config_t *adc_conf = (adc_config_t *)dev->device->DriverData;

	switch(request){
		case ADC_SAMPLERATE:
			adc_conf->samplerate = value;
			break;
		case ADC_RESOLUTION:
			adc_conf->resolution = value;
			break;
		case ADC_QUEUE_SIZE:
			/* somente pode ser alterado se filas dinâmicas forem utilizadas. */
			break;
		case ADC_TIMEOUT:
			adc_conf->timeout = value;
			break;
		case CTRL_ACQUIRE_READ_MUTEX:
			if (ADCRMutex[dev->device_number] != NULL)
			{
				ret = OSMutexAcquire(ADCRMutex[dev->device_number],value);
			}
			break;
		case CTRL_RELEASE_READ_MUTEX:
			if (ADCRMutex[dev->device_number] != NULL)
			{
				ret = OSMutexRelease(ADCRMutex[dev->device_number]);
			}
			break;
		default:
			break;
	}

	return ret;
}

static size_t ADC_Get(OS_Device_Control_t *dev, uint32_t request){
	uint32_t ret;
	adc_config_t *adc_conf = (adc_config_t *)dev->device->DriverData;

	switch(request){
		case ADC_SAMPLERATE:
			ret = adc_conf->samplerate;
			break;
		case ADC_RESOLUTION:
			ret = adc_conf->resolution;
			break;
		case ADC_QUEUE_SIZE:
			ret = adc_conf->queue_size;
			break;
		case ADC_TIMEOUT:
			ret = adc_conf->timeout;
			break;
		default:
			ret = 0;
			break;
	}
	return ret;
}

static const device_api_t ADC_api ={
		.read = (Device_Control_read_t)ADC_Read,
		.write = (Device_Control_write_t)NULL,
		.set = (Device_Control_set_t)ADC_Set,
		.get = (Device_Control_get_t)ADC_Get
};

void OSOpenADC(void *pdev, void *parameters)
{
	OS_Device_Control_t *dev = pdev;
	Init_ADC(parameters);
	dev->api = &ADC_api;
}



