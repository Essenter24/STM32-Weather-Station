/**
  ******************************************************************************
  * @file    adc_filter.h
  * @author  Lukasz Garlicki
  * @brief   Header file of ADC backlight module and filtering
  ******************************************************************************
  */


#ifndef INC_ADC_FILTER_H_
#define INC_ADC_FILTER_H_

/* Includes ------------------------------------------------------------------*/
#include "circular_buffer.h"


/* Defines ------------------------------------------------------------------*/
/* Define used for the program to know when to turn ADC on (the user turned on AUTO brightness mode) */
#define BRIGHTNESS_MODE_AUTO 125
/* Define used for representing maximum value of ADC (12-bit resolution) */
#define ADC_FULL_SCALE 4095


/**
  * @brief  Initialize ADC, circular buffer and start DMA conversion for ADC1
  */
void ADC_Init(void);
/**
  * @brief  Set brigthness of LCD screen using ADC measurements (AUTO) or manual settings
  */
void ADC_SetBrightness(void);
/**
  * @brief  Start AUTO mode (brightness of LCD screen changes depending on the ADC measurements)
  */
void ADC_StartAutoBrightness(void);
/**
  * @brief  Stop AUTO mode (brightness depends on user settings from now)
  */
void ADC_StopAutoBrightness(void);
/**
  * @brief  Get the value of PWM Duty Cycle
  * @retval Value of PWM Duty Cycle (value between 0 and ADC_FULL_SCALE (4095) )
  */
uint16_t ADC_GetPWMDutyCycle(void);
/**
  * @brief  Set the value of PWM Duty Cycle
  * @param  value Desired value of PWM Duty Cycle (between 0 and 4095)
  */
void ADC_SetPWMDutyCycle(uint16_t value);



#endif /* INC_ADC_FILTER_H_ */
