/**
  ******************************************************************************
  * @file    adc_filter.c
  * @author  Lukasz Garlicki
  * @brief   ADC measurement filtering and automatic LCD backlight control.
  *
  * This module allows for ADC Initialization, alongside adjusting
  * LCD brightness using filtered ADC data, as well as providing
  * possibilities to set the brightness manually.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "adc_filter.h"
#include "main.h"

/* Variables  ----------------------------------------------------------------*/

/* Stores information whether the brightness is in automatic mode */
uint8_t lcd_brightness_mode = BRIGHTNESS_MODE_AUTO;
/* Stores numerical value of PWM duty cycle manually set, between 0 and 4095 (12-bit resolution) */
static uint16_t pwm_duty_cycle = 0;
/* Stores raw data from ADC conversion */
static volatile uint32_t adc_data = 0;
/* Stores filtered value of ADC conversion */
static uint32_t adc_data_filtered;
/* Buffer for holding last 8 ADC conversions, meant for filtering ADC data */
static circular_buffer_t adc_filter_buffer;

void ADC_Init(void)
{
    Circular_Buffer_Init(&adc_filter_buffer);

    /* ADC Initialization suggested by ST in Reference Manual */
    if (LL_ADC_IsEnabled(ADC1))
    {
        LL_ADC_REG_StopConversion(ADC1);
        LL_ADC_Disable(ADC1);
        while (LL_ADC_IsEnabled(ADC1));
    }

    if (LL_ADC_IsDeepPowerDownEnabled(ADC1))
    {
        LL_ADC_DisableDeepPowerDown(ADC1);
    }

    if (!LL_ADC_IsInternalRegulatorEnabled(ADC1))
    {
        LL_ADC_EnableInternalRegulator(ADC1);
        LL_mDelay(50);

    }

    if (!LL_ADC_IsDeepPowerDownEnabled(ADC1)
            && LL_ADC_IsInternalRegulatorEnabled(ADC1)
            && !LL_ADC_IsEnabled(ADC1))
    {
        LL_ADC_StartCalibration(ADC1, LL_ADC_SINGLE_ENDED);
        while (LL_ADC_IsCalibrationOnGoing(ADC1)) {  };

        LL_mDelay(50);
    }

    /* DMA Initialization for ADC1, following instructions suggested by ST in Reference Manual */

    LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_1);
    LL_DMA_ConfigAddresses(DMA1, LL_DMA_CHANNEL_1, LL_ADC_DMA_GetRegAddr(ADC1, LL_ADC_DMA_REG_REGULAR_DATA),
            (uint32_t)&adc_data, LL_DMA_DIRECTION_PERIPH_TO_MEMORY);

    LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_1, 1);

    LL_DMA_ClearFlag_GI1(DMA1);

    LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_1);

    /* Clearing the flag of ADC readiness to avoid false flag information */
    if (LL_ADC_IsActiveFlag_ADRDY(ADC1))
    {
        LL_ADC_ClearFlag_ADRDY(ADC1);
    }

    LL_ADC_Enable(ADC1);
    /* Waiting for ADRDY flag to ensure conversion starts on ready ADC module */
    while (!LL_ADC_IsActiveFlag_ADRDY(ADC1)) { };
    LL_ADC_REG_StartConversion(ADC1);

}

void ADC_SetBrightness(void)
{
    if (lcd_brightness_mode == BRIGHTNESS_MODE_AUTO)
    {
        Circular_Buffer_Update(&adc_filter_buffer, adc_data);
        adc_data_filtered = (uint16_t)Filter_Average(adc_filter_buffer.data, adc_filter_buffer.count);

        /* Check whether the destinated brightness of LCD out of ADC measure won't be too low - limited to
         * 10% of the full scale. */
        if(adc_data_filtered < (ADC_FULL_SCALE / 10))
            adc_data_filtered = (ADC_FULL_SCALE / 10);
         LL_TIM_OC_SetCompareCH1(TIM1, adc_data_filtered);
    }
    else
    {
        /* Used for manually set PWM duty cycle in other modes than AUTO */
        LL_TIM_OC_SetCompareCH1(TIM1, pwm_duty_cycle);
    }
}

void ADC_StartAutoBrightness(void)
{
    LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_1);
    lcd_brightness_mode = BRIGHTNESS_MODE_AUTO;
}
void ADC_StopAutoBrightness(void)
{
    LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_1);
    lcd_brightness_mode = 100;
    pwm_duty_cycle = ADC_FULL_SCALE;
}

uint16_t ADC_GetPWMDutyCycle(void)
{
    return pwm_duty_cycle;
}

void ADC_SetPWMDutyCycle(uint16_t value)
{
    pwm_duty_cycle = value;
}
