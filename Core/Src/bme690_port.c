/**
  ******************************************************************************
  * @file    bme690_port.c
  * @author  Lukasz Garlicki
  * @brief   BME690 module allowing for handling I2C communication
  *         with the aforementioned sensor.
  *
  * This module implements the entire port protocol for
  *  I2C communication with BME690, and allows especially for
  * reading and writing to the sensor, as well as allowing to get
  * distinct measurements. Based on Bosch Sensortec recommendations for
  *  handling the communication (read, write and delay functions are made by the author)
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "bme69x.h"
#include "bme690_port.h"
#include "ui_control.h"

#include "bme69x_defs.h"
#include "circular_buffer.h"

/* Structs ------------------------------------------------------------------*/

/* Struct holding information about BME690 sensor*/
struct bme69x_dev bme;
/* Struct holding information about temperature, pressure and humidity oversampling in measurements */
struct bme69x_conf conf;
/* Struct holding technical information about data in sensor */
struct bme69x_data data;


/* Variables ------------------------------------------------------------------*/

/* Stores BME690 address for I2C communication. SDO pin of BME690 is connected to GND, thus
 * BME69X_I2C_ADDR_LOW is used for communication.
 */
static uint8_t dev_addr = BME69X_I2C_ADDR_LOW;
/* Stores the value of temperature */
static float temperature = 0.0f;
/* Stores the value of pressure */
static float pressure = 0.0f;
/* Stores the value of humidity */
static float humidity = 0.0f;
/* Buffer for holding last 8 measures of temperature, meant for filtering */
static circular_buffer_t temperature_filter_buffer;
/* Buffer for holding last 8 measures of pressure, meant for filtering */
static circular_buffer_t pressure_filter_buffer;
/* Buffer for holding last 8 measures of humidity, meant for filtering */
static circular_buffer_t humidity_filter_buffer;
/* Stores filtered value of temperature */
static float temperature_filtered = 0.0f;
/* Stores filtered value of pressure */
static float pressure_filtered = 0.0f;
/* Stores filtered value of humidity */
static float humidity_filtered = 0.0f;
/* Stores the tick of last measurements */
static uint32_t bme690_last_measurement = 0;

float BME690_GetTemperature(void)
{
    return temperature_filtered;
}
float BME690_GetPressure(void)
{
    return pressure_filtered;
}
float BME690_GetHumidity(void)
{
    return humidity_filtered;
}

/**
  * @brief  Read data from BME690 sensor in I2C communication mode
  * @param  reg_addr Desired register address to read from
  * @param  reg_data Pointer to desired place for holding read data
  * @param  len Number of bytes to read
  * @param intf_ptr Pointer to the I2C device address
  * @retval  BME69X_OK on success, otherwise an error code specified by Bosch
  */
static BME69X_INTF_RET_TYPE bme69x_i2c_read(uint8_t reg_addr, uint8_t * reg_data, uint32_t len, void * intf_ptr)
{
    if (intf_ptr == NULL || reg_data == NULL || len == 0)
    {
        return BME69X_E_COM_FAIL;
    }

    /* Retrieve the I2C device address passed to the Bosch driver's interface callback. */
    uint8_t dev_addr = *(uint8_t*)intf_ptr;

    /* Establish communication by I2C with sensor */
    LL_I2C_HandleTransfer(I2C1, dev_addr << 1, LL_I2C_ADDRSLAVE_7BIT, 1,
            LL_I2C_MODE_SOFTEND, LL_I2C_GENERATE_START_WRITE);
    while (!LL_I2C_IsActiveFlag_TXIS(I2C1))
    {
        if (LL_I2C_IsActiveFlag_NACK(I2C1))
        {
            LL_I2C_ClearFlag_NACK(I2C1);
            LL_I2C_GenerateStopCondition(I2C1);
            return BME69X_E_COM_FAIL;
        }
    }

    /* Transmit the register address to read from to the sensor */
    LL_I2C_TransmitData8(I2C1, reg_addr);

    while (!LL_I2C_IsActiveFlag_TC(I2C1))
    {
        if (LL_I2C_IsActiveFlag_NACK(I2C1))
        {
            LL_I2C_ClearFlag_NACK(I2C1);
            return BME69X_E_COM_FAIL;
        }
    }

    /* Handle the entire reading of data from sensor */

    LL_I2C_HandleTransfer(I2C1, dev_addr << 1, LL_I2C_ADDRSLAVE_7BIT, len,
            LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_READ);

    for (uint32_t data = 0; data < len; data++)
    {
        while (!LL_I2C_IsActiveFlag_RXNE(I2C1))
        {
            if (LL_I2C_IsActiveFlag_NACK(I2C1))
            {
                LL_I2C_ClearFlag_NACK(I2C1);
                return BME69X_E_COM_FAIL;
            }
        }
        reg_data[data] = LL_I2C_ReceiveData8(I2C1);
    }

    while (!LL_I2C_IsActiveFlag_STOP(I2C1)) { }
    LL_I2C_ClearFlag_STOP(I2C1);

    return BME69X_OK;

}

/**
  * @brief  Write data to BME690 sensor in I2C communication mode
  * @param  reg_addr Desired register address to write to
  * @param  reg_data Pointer to desired place for holding data to write
  * @param  len Number of bytes to write
  * @param intf_ptr Pointer to the I2C device address
  * @retval  BME69X_OK on success, otherwise an error code specified by Bosch
  */
static BME69X_INTF_RET_TYPE bme69x_i2c_write(uint8_t reg_addr, const uint8_t * reg_data, uint32_t len, void * intf_ptr)
{
    if (intf_ptr == NULL || reg_data == NULL || len == 0)
    {
        return BME69X_E_COM_FAIL;
    }
    /* Retrieve the I2C device address passed to the Bosch driver's interface callback. */
    uint8_t dev_addr = *(uint8_t*)intf_ptr;

    /* Establish communication by I2C with sensor */
    LL_I2C_HandleTransfer(I2C1, dev_addr << 1, LL_I2C_ADDRSLAVE_7BIT, 1 + len,
            LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_WRITE);

    while (!LL_I2C_IsActiveFlag_TXIS(I2C1))
    {
        if (LL_I2C_IsActiveFlag_NACK(I2C1))
        {
            LL_I2C_ClearFlag_NACK(I2C1);
            return BME69X_E_COM_FAIL;
        }
    }

    /* Transmit the register address to write to in sensor */
    LL_I2C_TransmitData8(I2C1, reg_addr);

    /* Proceed to write data to the register in sensor */
    for (uint32_t data = 0; data < len; data++)
    {
        while (!LL_I2C_IsActiveFlag_TXIS(I2C1))
        {
            if (LL_I2C_IsActiveFlag_NACK(I2C1))
            {
                LL_I2C_ClearFlag_NACK(I2C1);
                return BME69X_E_COM_FAIL;
            }
        }

        LL_I2C_TransmitData8(I2C1, reg_data[data]);

    }

    while (!LL_I2C_IsActiveFlag_STOP(I2C1)) { }


    LL_I2C_ClearFlag_STOP(I2C1);

    return BME69X_OK;
}


/**
  * @brief Delay function for sensor, measuring time in microseconds
  * @param period Length of delay in microseconds
  * @param intf_ptr Pointer to the I2C device address
  */
static void bme69x_delay_us(uint32_t period, void * intf_ptr)
{
	(void)intf_ptr;

	volatile uint32_t starting_ticks = DWT->CYCCNT;
	uint32_t required_ticks = period * (SystemCoreClock / 1000000U);

	while ((DWT->CYCCNT - starting_ticks) < required_ticks);
}


void BME690_Init(void)
{
	int8_t rslt;

    bme.read = bme69x_i2c_read;
    bme.write = bme69x_i2c_write;
    bme.delay_us = bme69x_delay_us;
    bme.intf = BME69X_I2C_INTF;
    bme.intf_ptr = &dev_addr;

    rslt = bme69x_init(&bme);
    if (rslt != BME69X_OK)
    {
        Error_Handler();
    }

    conf.filter = BME69X_FILTER_OFF;
    conf.odr = BME69X_ODR_NONE;
    conf.os_hum = BME69X_OS_4X;
    conf.os_pres = BME69X_OS_4X;
    conf.os_temp = BME69X_OS_8X;
    rslt = bme69x_set_conf(&conf, &bme);

    Circular_Buffer_Init(&temperature_filter_buffer);
    Circular_Buffer_Init(&pressure_filter_buffer);
    Circular_Buffer_Init(&humidity_filter_buffer);

}

static void BME690_ReadData(void)
{
    int8_t rslt;
	uint32_t del_period;
	uint8_t n_fields;

    rslt = bme69x_set_op_mode(BME69X_FORCED_MODE, &bme);

    del_period = bme69x_get_meas_dur(BME69X_FORCED_MODE, &conf, &bme);
    bme.delay_us(del_period, bme.intf_ptr);

    rslt = bme69x_get_data(BME69X_FORCED_MODE, &data, &n_fields, &bme);

    if ((rslt == BME69X_OK) && (n_fields > 0))
    {
        return;
    }
}


/**
  * @brief  Function meant for filtering BME690 temperature, humiditiy
  * and pressure measurements
  */
static void Filter_BME690_Data(void){
    Circular_Buffer_Update(&temperature_filter_buffer, temperature);
    temperature_filtered = Filter_Average(temperature_filter_buffer.data, temperature_filter_buffer.count);

    Circular_Buffer_Update(&pressure_filter_buffer, pressure);
    pressure_filtered = Filter_Average(pressure_filter_buffer.data, pressure_filter_buffer.count);

    Circular_Buffer_Update(&humidity_filter_buffer, humidity);
    humidity_filtered = Filter_Average(humidity_filter_buffer.data, humidity_filter_buffer.count);
}


/* TODO: Investigate the function in case of RTOS implementation */
void Handle_BME690_Data(void){
    if ((HAL_GetTick() - bme690_last_measurement >= 1000))
    {
        if (lcd_current_state == STATE_INIT)
        {
            lcd_current_state = STATE_COMPACT;
        }

        bme690_last_measurement = HAL_GetTick();

        BME690_ReadData();
        temperature = data.temperature;
        pressure = data.pressure / 100.0f;
        humidity = data.humidity;

        Filter_BME690_Data();
    }
}
