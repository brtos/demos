
/*********************************************************************************************************
*                                               BRTOS
*                                Brazilian Real-Time Operating System
*                            Acronymous of Basic Real-Time Operating System
*
*                              
*                                  Open Source RTOS under MIT License
*
*
*
*                                              OS Tasks
*
*
*   Author:   Gustavo Weber Denardin
*   Revision: 1.0
*   Date:     20/03/2009
*
*********************************************************************************************************/


#include "BRTOS.h"
#include "tasks.h"

#include "xhw_types.h"
#include "xsysctl.h"
#include "xgpio.h"
#include "xwdt.h"
#include "xhw_memmap.h"
#include "accel.h"
#include "xhw_uart.h"
#include "xcore.h"
#include "DAC.h"
#include "UARTx.h"


// Declara um ponteiro para o bloco de controle da Porta Serial
extern BRTOS_Mbox   *Data;



/* Includes ------------------------------------------------------------------*/
/* 16 bits Data Mode */
static const unsigned short SinusOutputData[256] = {
        2047U,  2097U,  2147U,  2198U,  2248U,  2298U,  2347U,  2397U,  2446U,  2496U,  2544U,  2593U,  2641U,  2689U,
        2737U,  2784U,  2830U,  2877U,  2922U,  2967U,  3012U,  3056U,  3099U,  3142U,  3184U,  3226U,  3266U,  3306U,
        3346U,  3384U,  3422U,  3458U,  3494U,  3530U,  3564U,  3597U,  3629U,  3661U,  3691U,  3721U,  3749U,  3776U,
        3803U,  3828U,  3852U,  3875U,  3897U,  3918U,  3938U,  3957U,  3974U,  3991U,  4006U,  4020U,  4033U,  4044U,
        4055U,  4064U,  4072U,  4079U,  4084U,  4088U,  4092U,  4093U,  4094U,  4093U,  4092U,  4088U,  4084U,  4079U,
        4072U,  4064U,  4055U,  4044U,  4033U,  4020U,  4006U,  3991U,  3974U,  3957U,  3938U,  3918U,  3897U,  3875U,
        3852U,  3828U,  3803U,  3776U,  3749U,  3721U,  3691U,  3661U,  3629U,  3597U,  3564U,  3530U,  3494U,  3458U,
        3422U,  3384U,  3346U,  3306U,  3266U,  3226U,  3184U,  3142U,  3099U,  3056U,  3012U,  2967U,  2922U,  2877U,
        2830U,  2784U,  2737U,  2689U,  2641U,  2593U,  2544U,  2496U,  2446U,  2397U,  2347U,  2298U,  2248U,  2198U,
        2147U,  2097U,  2047U,  1997U,  1947U,  1896U,  1846U,  1796U,  1747U,  1697U,  1648U,  1598U,  1550U,  1501U,
        1453U,  1405U,  1357U,  1310U,  1264U,  1217U,  1172U,  1127U,  1082U,  1038U,  995U,   952U,   910U,   868U,
        828U,   788U,   748U,   710U,   672U,   636U,   600U,   564U,   530U,   497U,   465U,   433U,   403U,   373U,
        345U,   318U,   291U,   266U,   242U,   219U,   197U,   176U,   156U,   137U,   120U,   103U,   88U,    74U,
        61U,    50U,    39U,    30U,    22U,    15U,    10U,    6U,     2U,     1U,     0U,     1U,     2U,     6U,
        10U,    15U,    22U,    30U,    39U,    50U,    61U,    74U,    88U,    103U,   120U,   137U,   156U,   176U,
        197U,   219U,   242U,   266U,   291U,   318U,   345U,   373U,   403U,   433U,   465U,   497U,   530U,   564U,
        600U,   636U,   672U,   710U,   748U,   788U,   828U,   868U,   910U,   952U,   995U,   1038U,  1082U,  1127U,
        1172U,  1217U,  1264U,  1310U,  1357U,  1405U,  1453U,  1501U,  1550U,  1598U,  1648U,  1697U,  1747U,  1796U,
        1846U,  1896U,  1947U,  1997U
};

#include "device.h"
void System_Time(void *param)
{
   // task setup
   uint16_t i = 0;
   uint16_t j = 0;
   
   (void)param;
   OSResetTime();
   DAC0_Init();

   #if (WATCHDOG == 1)
   /* Initialize the WDT */
   WDTimerInit(WDT_MODE_NORMAL | xWDT_INTERVAL_1K_32MS);
   WDTimerEnable();
   #endif
  
   // task main loop
   for (;;)
   {
	  DAC0_SetData(SinusOutputData[j++]);
	  if (j >= 256) j = 0;
	  DelayTask(10);

	  #if (WATCHDOG == 1)
      /* Feed WDT counter */
      WDTimerRestart();
	  #endif
      
      i++;
      
      if (i >= 100)
      {
        OSUpdateUptime();
        i = 0;
      }
   }
}




typedef struct _accel_data
{
	int16_t x;
	int16_t y;
	int16_t z;
} accel_data;

void Test_Task_1(void *param)
{
	/* task setup */
	accel_data data;

	(void)param;
	accel_init();
  
   /* task main loop */
   for (;;)
   {
      DelayTask(250);

      //accel_read();
      data.x = accel_x();
      data.y = accel_y();
      data.z = accel_z();

      (void)OSMboxPost(Data, (void*)&data);
   }
}

void Test_Task_2(void *param)
{
	/* task setup */
	uint8_t leds = 0;
	uint8_t state = 0;
	accel_data *pt_ac;
	accel_data data_ac;
	OS_Device_Control_t *dev_gpiob;
	gpio_config_t gpiob;
	OS_Device_Control_t *dev_gpiod;
	gpio_config_t gpiod;

	(void)param;

	gpiob.used_pins_out = GPIO_PIN_18 | GPIO_PIN_19;
	gpiob.used_pins_in = 0;
	gpiob.irq_pins = 0;
	dev_gpiob = OSDevOpen("GPIOB", &gpiob);

	gpiod.used_pins_out = GPIO_PIN_0 | GPIO_PIN_1;
	gpiod.used_pins_in = 0;
	gpiod.irq_pins = 0;
	dev_gpiod = OSDevOpen("GPIOD", &gpiod);

	OSDevWrite(dev_gpiob,GPIO_PIN_18 | GPIO_PIN_19,1);
	OSDevWrite(dev_gpiod,GPIO_PIN_1,0);

	for(;;)
	{
		(void)OSMboxPend(Data, (void**)&pt_ac,0);
		data_ac = *pt_ac;

		if (!state)
		{
			state = 1;
			if ((data_ac.x > data_ac.y) && (data_ac.x > data_ac.z))
			{
				leds = 0;
			}else
			{
				if ((data_ac.y > data_ac.x) && (data_ac.y > data_ac.z))
				{
					leds = 1;
				}else
				{
					if ((data_ac.z > data_ac.x) && (data_ac.z > data_ac.y))
					{
						leds = 2;
					}else
					{
						leds = 3;
					}
				}
			}
		}else
		{
			state = 0;
			leds = 4;
		}

	      switch(leds)
	      {
	      	  case 0:
		    	  OSDevWrite(dev_gpiob,GPIO_PIN_18,0);
		    	  OSDevWrite(dev_gpiob,GPIO_PIN_19,1);
		    	  OSDevWrite(dev_gpiod,GPIO_PIN_1,1);
		    	  break;

	      	  case 1:
		    	  OSDevWrite(dev_gpiob,GPIO_PIN_18,1);
		    	  OSDevWrite(dev_gpiob,GPIO_PIN_19,0);
		    	  OSDevWrite(dev_gpiod,GPIO_PIN_1,1);
		    	  break;

	      	  case 2:
		    	  OSDevWrite(dev_gpiob,GPIO_PIN_18 | GPIO_PIN_19,1);
		    	  OSDevWrite(dev_gpiod,GPIO_PIN_1,0);
		    	  break;

	      	  case 3:
		    	  OSDevWrite(dev_gpiob,GPIO_PIN_18 | GPIO_PIN_19,0);
		    	  OSDevWrite(dev_gpiod,GPIO_PIN_1,0);
		    	  break;

	      	  case 4:
		    	  OSDevWrite(dev_gpiob,GPIO_PIN_18 | GPIO_PIN_19,1);
		    	  OSDevWrite(dev_gpiod,GPIO_PIN_1,1);
		    	  break;

	      	  default:
	      		  break;
	      }
	}
}

void SerialTask(void *param){
	char data;
	OS_Device_Control_t *uart;
	uart_config_t uart0;
	(void)param;

	uart0.baudrate = 115200;
	uart0.parity = UART_PAR_EVEN;
	uart0.polling_irq = UART_IRQ;
	uart0.queue_size = 128;
	uart0.stop_bits = UART_STOP_ONE;
	uart0.timeout = 10;
	uart = OSDevOpen("UART0", &uart0);

	OSDevWrite(uart,"Porta serial ativa!\n\r",21);

	while(1){
		if (OSDevRead(uart,&data,1) >= 1){
			OSDevWrite(uart,&data,1);
		}
		OSDevSet(uart,UART_TIMEOUT,INF_TIMEOUT);
	}
}



#if 0
#include "MKL25Z4.h"
unsigned short ADConvert(unsigned int canal)
{
  unsigned short return_ad = 0;
  ADC0_SC1A = canal;    /* Inicia a medida de um canal */
  while((ADC0_SC1A & ADC_SC1_COCO_MASK) == 0){};

  return_ad = ADC0_RA;
  ADC0_SC1A = 0x1F;
  return return_ad;
}

int16_t temperature = 0;
void AD_Task(void)
{
	/* task setup */
	int32_t digital_temp = 0;
	int32_t bandgap = 0;
	int32_t bandgap_vector = 0;
	uint16_t i = 0;

    // Enable Peripheral ADC
    xSysCtlPeripheralEnable(SYSCTL_PERIPH_ADC);

    //Set clocks for ADC - bus clock divide by 6
    xSysCtlPeripheralClockSourceSet(xSYSCTL_ADC0_MAIN,6);

    // ADC0, convert once, software trigger
    xADCConfigure(xADC0_BASE, xADC_MODE_SCAN_SINGLE_CYCLE, ADC_TRIGGER_PROCESSOR);
    ADCSampleTimeSet(xADC0_BASE, ADC_SAMPLE_TIME_LONG);
    ADCSingleResolutionSet(xADC0_BASE, ADC_SINGLE_RESOLUTION_12);

    // Calcula valor medio de aquisicao do BandGap
    for(i=0;i<64;i++)
    {
    	bandgap_vector += ADConvert(xADC_CTL_BG);

    	DelayTask(3);
    }

    bandgap = (uint32_t)(bandgap_vector >> 6);

   /* task main loop */
   for (;;)
   {
	   //////////////////////////////////////////////////////////////
	   // Adquire temperatura do core
	   //////////////////////////////////////////////////////////////

	   // Enable the adc, ADC channel temperature sensor
	   // Read the converted value
	   digital_temp = ADConvert(xADC_CTL_TS);
	   digital_temp += ADConvert(xADC_CTL_TS);
	   digital_temp = digital_temp >> 1;

	   digital_temp = (1000 * digital_temp)/bandgap;

	   temperature = 25 - (((digital_temp - 719)*1000)/1715);

	   DelayTask(100);
   }
}

#endif
