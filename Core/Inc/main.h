/* USER CODE BEGIN Header */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

#include "stm32l4xx_ll_adc.h"
#include "stm32l4xx_ll_dma.h"
#include "stm32l4xx_ll_i2c.h"
#include "stm32l4xx_ll_lpuart.h"
#include "stm32l4xx_ll_rcc.h"
#include "stm32l4xx_ll_tim.h"
#include "stm32l4xx_ll_system.h"
#include "stm32l4xx_ll_gpio.h"
#include "stm32l4xx_ll_exti.h"
#include "stm32l4xx_ll_bus.h"
#include "stm32l4xx_ll_cortex.h"
#include "stm32l4xx_ll_utils.h"
#include "stm32l4xx_ll_pwr.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LCD_DB7_Pin LL_GPIO_PIN_0
#define LCD_DB7_GPIO_Port GPIOC
#define LCD_DB6_Pin LL_GPIO_PIN_1
#define LCD_DB6_GPIO_Port GPIOC
#define LCD_DB5_Pin LL_GPIO_PIN_2
#define LCD_DB5_GPIO_Port GPIOC
#define LCD_DB4_Pin LL_GPIO_PIN_3
#define LCD_DB4_GPIO_Port GPIOC
#define LCD_RS_Pin LL_GPIO_PIN_0
#define LCD_RS_GPIO_Port GPIOA
#define LCD_E_Pin LL_GPIO_PIN_1
#define LCD_E_GPIO_Port GPIOA
#define SW2_UP_Pin LL_GPIO_PIN_2
#define SW2_UP_GPIO_Port GPIOA
#define SW2_UP_EXTI_IRQn EXTI2_IRQn
#define SW3_DOWN_Pin LL_GPIO_PIN_3
#define SW3_DOWN_GPIO_Port GPIOA
#define SW3_DOWN_EXTI_IRQn EXTI3_IRQn
#define SW4_LEFT_Pin LL_GPIO_PIN_4
#define SW4_LEFT_GPIO_Port GPIOA
#define SW4_LEFT_EXTI_IRQn EXTI4_IRQn
#define SW5_RIGHT_Pin LL_GPIO_PIN_5
#define SW5_RIGHT_GPIO_Port GPIOA
#define SW5_RIGHT_EXTI_IRQn EXTI9_5_IRQn
#define SW6_ENTER_Pin LL_GPIO_PIN_6
#define SW6_ENTER_GPIO_Port GPIOA
#define SW6_ENTER_EXTI_IRQn EXTI9_5_IRQn
#define D1_Pin LL_GPIO_PIN_4
#define D1_GPIO_Port GPIOC
#define D2_Pin LL_GPIO_PIN_5
#define D2_GPIO_Port GPIOC
#define ADC_Pin LL_GPIO_PIN_0
#define ADC_GPIO_Port GPIOB
#define UART_RX_Pin LL_GPIO_PIN_10
#define UART_RX_GPIO_Port GPIOB
#define UART_TX_Pin LL_GPIO_PIN_11
#define UART_TX_GPIO_Port GPIOB
#define D3_Pin LL_GPIO_PIN_6
#define D3_GPIO_Port GPIOC
#define D4_Pin LL_GPIO_PIN_7
#define D4_GPIO_Port GPIOC
#define BME690_SCL_Pin LL_GPIO_PIN_6
#define BME690_SCL_GPIO_Port GPIOB
#define BME690_SDA_Pin LL_GPIO_PIN_7
#define BME690_SDA_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
