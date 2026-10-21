/**
  ******************************************************************************
  * @file    bme690_port.h
  * @author  Lukasz Garlicki
  * @brief   Header file of BME690 measurements module.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef INC_BME690_PORT_H_
#define INC_BME690_PORT_H_



/**
  * @brief  Initialization of BME690 sensor and three buffers set to hold values of measurements
  */
void BME690_Init(void);
/**
  * @brief  Handling the reading BME690 data and filtering
  */
void Handle_BME690_Data(void);

/**
  * @brief  Get the value of filtered temperature
  */
float BME690_GetTemperature(void);
/**
  * @brief  Get the value of filtered pressure
  */
float BME690_GetPressure(void);
/**
  * @brief  Get the value of filtered humidity
  */
float BME690_GetHumidity(void);


#endif /* INC_BME690_PORT_H_ */
