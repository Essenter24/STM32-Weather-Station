/**
  ******************************************************************************
  * @file    button.c
  * @author  Lukasz Garlicki
  * @brief   Module used for handling buttons input
  *
  *
  * This module allows for simple check whether any of the corresponding buttons
  * has been pressed. Buttons deboucing is executed software-wise in
  * functions which read the button inputs
  ******************************************************************************
  */


/* Includes -------------------------------------*/
#include "button.h"

uint8_t Button_Up_State(void)
{
    return !LL_GPIO_IsInputPinSet(SW2_UP_GPIO_Port, SW2_UP_Pin);
}
uint8_t Button_Right_State(void)
{
    return !LL_GPIO_IsInputPinSet(SW5_RIGHT_GPIO_Port, SW5_RIGHT_Pin);
}
uint8_t Button_Left_State(void)
{
    return !LL_GPIO_IsInputPinSet(SW4_LEFT_GPIO_Port, SW4_LEFT_Pin);
}
uint8_t Button_Down_State(void)
{
    return !LL_GPIO_IsInputPinSet(SW3_DOWN_GPIO_Port, SW3_DOWN_Pin);
}
uint8_t Button_Enter_State(void)
{
    return !LL_GPIO_IsInputPinSet(SW6_ENTER_GPIO_Port, SW6_ENTER_Pin);
}
