/*
 * drivers.h
 *
 *  Created on: 02/09/2016
 *      Author: Avell
 */

#ifndef DRIVERS_DRIVERS_H_
#define DRIVERS_DRIVERS_H_


#define INTERRUPT_UART  2
#define INTERRUPT_SPI   3
#define INTERRUPT_I2C   4
#define INTERRUPT_ADC   5
#define INTERRUPT_DAC	6


typedef enum{
	ADC_POLLING,
	ADC_IRQ,
}adc_irq_t;

typedef enum{
	ADC_RES_8 = 8,
	ADC_RES_10 = 10,
	ADC_RES_12 = 12,
	ADC_RES_16 = 16
}adc_res_t;


typedef enum
{
	ADC_SAMPLERATE,
	ADC_RESOLUTION,
	ADC_QUEUE_SIZE,
	ADC_TIMEOUT
}adc_request_t;

typedef struct adc_config_t_{
	int 		samplerate;
	adc_res_t   resolution;
	adc_irq_t 	polling_irq;
	int 		queue_size;
	ostick_t 	timeout;
	bool		read_mutex;
}adc_config_t;

typedef enum{
	DAC_POLLING,
	DAC_IRQ,
}dac_irq_t;

typedef enum{
	DAC_RES_8 = 8,
	DAC_RES_10 = 10,
	DAC_RES_12 = 12,
	DAC_RES_16 = 16
}dac_res_t;


typedef enum
{
	DAC_SAMPLERATE,
	DAC_RESOLUTION,
	DAC_QUEUE_SIZE,
	DAC_TIMEOUT
}dac_request_t;

typedef struct dac_config_t_
{
	int 		samplerate;
	dac_res_t   resolution;
	dac_irq_t 	polling_irq;
	int 		queue_size;
	ostick_t 	timeout;
	bool		write_mutex;
}dac_config_t;

typedef enum{
	SPI_POLLING,
	SPI_IRQ,
}spi_irq_t;


typedef enum{
	SPI_MASTER,
	SPI_SLAVE,
}spi_mode_t;


typedef enum{
	SPI_BAUDRATE,
	SPI_POLARITY,
	SPI_QUEUE_SIZE,
	SPI_TIMEOUT
}spi_request_t;

typedef enum{
	SPI_P,
	SPI_N
}spi_pol_t;

typedef struct spi_config_t_{
	int 		baudrate;
	spi_pol_t 	polarity;
	spi_irq_t 	polling_irq;
	spi_mode_t  mode;
	int 		queue_size;
	ostick_t 	timeout;
	bool		mutex;
}spi_config_t;

typedef enum{
	I2C_POLLING,
	I2C_IRQ,
}i2c_irq_t;

typedef enum{
	I2C_MASTER,
	I2C_SLAVE,
}i2c_mode_t;

typedef enum{
	I2C_ADDR7,
	I2C_ADDR10,
}i2c_size_t;


typedef enum{
	I2C_BAUDRATE,
	I2C_DIRECTION,
	I2C_QUEUE_SIZE,
	I2C_TIMEOUT
}i2c_request_t;

typedef enum{
	I2C_R,
	I2C_W
}i2c_dir_t;

typedef struct i2c_config_t_{
	int 		baudrate;
	i2c_dir_t 	direction;
	i2c_irq_t 	polling_irq;
	i2c_mode_t  mode;
	i2c_size_t  addr_size;
	int         addr;
	int 		queue_size;
	ostick_t 	timeout;
	bool		mutex;
}i2c_config_t;


#endif /* DRIVERS_DRIVERS_H_ */
