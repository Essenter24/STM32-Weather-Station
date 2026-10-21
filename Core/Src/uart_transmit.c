/**
  ******************************************************************************
  * @file    uart_transmit.c
  * @author  Lukasz Garlicki
  * @brief   UART transmission of environmental measures
  *
  * This module allows for handling the UART transmission, transmitting
  * time of the measure and the environtmental parameters to PC or
  * other embedded system.
  ******************************************************************************
  */

/* TODO: Debug for potential transfer errors and garbage data message */

/* Includes ------------------------------------------------------------------*/
#include "uart_transmit.h"
#include "rtc.h"
#include "bme690_port.h"
#include <stdio.h>

/* Variables  ----------------------------------------------------------------*/
/* Container of UART message */
static char uart_message[256];
/* Extern handler of RTC, used for handling RTC data using HAL library */
extern RTC_HandleTypeDef hrtc;
/* UART transmission request flag */
static volatile uint8_t uart_transmission_request = 0;
/* UART transmission running flag */
static volatile uint8_t uart_transmission_running = 0;
/* Length of UART message to be transmitted */
static uint16_t length = 0;

/**
  * @brief  Prepare JSON message for UART, containing current time and environmental parameters
  */
static void UART_PrepareMessage(void)
{
    RTC_TimeTypeDef time = {0};
    RTC_DateTypeDef date = {0};

    uint8_t uart_hours = 0;
    uint8_t uart_minutes = 0;
    uint8_t uart_seconds = 0;

    int16_t uart_temperature = 0;
    uint16_t uart_pressure = 0;
    uint16_t uart_humidity = 0;

    if (HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN) == HAL_OK)
    {
        HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);
    }

    uart_hours = time.Hours;
    uart_minutes = time.Minutes;
    uart_seconds = time.Seconds;

    /* Multiplying by 10 to avoid floating-point operations directly on MCU,
     * UART receiving module in Python or Rust can easily divide it by 10
     * and work on the original data
     */
    uart_temperature = BME690_GetTemperature() * 10;
    uart_pressure = BME690_GetPressure() * 10;
    uart_humidity = BME690_GetHumidity() * 10;

    length = snprintf(uart_message, sizeof(uart_message),
            "{\"hours\":%u,\"minutes\":%u,\"seconds\":%u,"
            "\"temperature\":%i,\"pressure\":%i,\"humidity\":%i}\r\n",
            uart_hours, uart_minutes, uart_seconds,
                uart_temperature, uart_pressure, uart_humidity);

    if (length >= sizeof(uart_message))
    {
        length = sizeof(uart_message) - 1;
    }
}



void UART_Transmit(void)
{

    if (uart_transmission_running || !uart_transmission_request)
    {
        return;
    }


    uart_transmission_request = 0;
    uart_transmission_running = 1;

    UART_PrepareMessage();


    /* DMA configuration for UART */
    LL_DMA_DisableChannel(DMA2, LL_DMA_CHANNEL_6);

    LL_DMA_ConfigAddresses(DMA2, LL_DMA_CHANNEL_6, (uint32_t)uart_message,
            LL_LPUART_DMA_GetRegAddr(LPUART1, LL_LPUART_DMA_REG_DATA_TRANSMIT), LL_DMA_DIRECTION_MEMORY_TO_PERIPH);

    LL_DMA_SetDataLength(DMA2, LL_DMA_CHANNEL_6, length);

    LL_LPUART_ClearFlag_TC(LPUART1);
    LL_DMA_ClearFlag_GI6(DMA2);

    LL_DMA_EnableChannel(DMA2, LL_DMA_CHANNEL_6);


    LL_LPUART_EnableIT_TC(LPUART1);
    LL_LPUART_EnableDMAReq_TX(LPUART1);
}


void UART_DMA_TxCallback(void)
{
    /* Handling end of transfer */
    if (LL_DMA_IsActiveFlag_TC6(DMA2))
    {
        LL_DMA_ClearFlag_GI6(DMA2);

        LL_DMA_DisableChannel(DMA2, LL_DMA_CHANNEL_6);
        LL_LPUART_DisableDMAReq_TX(LPUART1);

    }
    /* Handling error */
    else if (LL_DMA_IsActiveFlag_TE6(DMA2))
    {
        LL_DMA_ClearFlag_GI6(DMA2);

        LL_DMA_DisableChannel(DMA2, LL_DMA_CHANNEL_6);
        LL_LPUART_DisableDMAReq_TX(LPUART1);

    }
}

void UART_TIM3_RequestTransmission(void)
{
    if (LL_TIM_IsActiveFlag_UPDATE(TIM3))
    {
        LL_TIM_ClearFlag_UPDATE(TIM3);
        uart_transmission_request = 1;
    }
}


void UART_LPUART1_TransferCompleteCallback(void)
{
    if (LL_LPUART_IsActiveFlag_TC(LPUART1))
    {
        LL_LPUART_ClearFlag_TC(LPUART1);

        LL_LPUART_DisableIT_TC(LPUART1);

        uart_transmission_running = 0;
    }
}

uint8_t LPUART1_IsBusy(void)
{
    return (uart_transmission_running || uart_transmission_request);
}


