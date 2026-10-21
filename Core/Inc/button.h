/**
  ******************************************************************************
  * @file    button.h
  * @author  Lukasz Garlicki
  * @brief   Header file of module used for handling buttons input
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_


/* Includes -------------------------------------*/
#include <stdint.h>
#include <gpio.h>


/**
  * @brief  Check whether button UP has been pressed
  * @retval Returns the state of button UP
  */
uint8_t Button_Up_State(void);
/**
  * @brief  Check whether button RIGHT has been pressed
  * @retval Returns the state of button RIGHT
  */
uint8_t Button_Right_State(void);
/**
  * @brief  Check whether button LEFT has been pressed
  * @retval Returns the state of button LEFT
  */
uint8_t Button_Left_State(void);
/**
  * @brief  Check whether button DOWN has been pressed
  * @retval Returns the state of button DOWN
  */
uint8_t Button_Down_State(void);
/**
  * @brief  Check whether button ENTER has been pressed
  * @retval Returns the state of button ENTER
  */
uint8_t Button_Enter_State(void);

#endif /* INC_BUTTON_H_ */
