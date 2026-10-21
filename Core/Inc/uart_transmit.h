/**
  ******************************************************************************
  * @file    uart_transmit.h
  * @author  Lukasz Garlicki
  * @brief   Header file of UART transmission handler for sending
  *         environmental sensor data
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef INC_UART_TRANSMIT_H_
#define INC_UART_TRANSMIT_H_


/* Includes ------------------------------------------------------------------*/
#include <stdint.h>


/**
  * @brief  Callback function at the end of DMA UART transmission
  */
void UART_DMA_TxCallback(void);
/**
  * @brief TIM3 interrupt function triggering UART transmission
  */
void UART_TIM3_RequestTransmission(void);
/**
  * @brief LPUART1 interrupt callback at the then of data transmission
  */
void UART_LPUART1_TransferCompleteCallback(void);
/**
  * @brief  UART transmission, used for sending JSON message with weather parameters
  */
void UART_Transmit(void);

/**
 * @brief Get the state of LPUART1 transmission
 * @retval Returns 1, if the LPUART1 is currently busy, returns 0 otherwise
 */
uint8_t LPUART1_IsBusy(void);


#endif /* INC_UART_TRANSMIT_H_ */
