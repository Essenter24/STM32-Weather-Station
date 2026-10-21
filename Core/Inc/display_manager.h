/**
  ******************************************************************************
  * @file    display_manager.h
  * @author  Lukasz Garlicki
  * @brief   Header file of LCD display manager
  ******************************************************************************
  */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef INC_DISPLAY_MANAGER_H_
#define INC_DISPLAY_MANAGER_H_
/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/**
  * @brief Show LCD message for Compact Mode
  * @param temp Temperature value to be shown on screen (without decimals)
  * @param pressure Pressure value to be shown on screen (without decimals)
  * @param humidity Humidity value to be shown on screen (without decimals)
  * @param hours Current hour to be shown on screen
  * @param mins Current minutes to be shown on screen
  * @param secs Currents seconds to be shown on screen
  */
void LCD_ShowCompact(int16_t temp, uint16_t pressure, uint16_t humidity, uint8_t hours, uint8_t mins, uint8_t secs);
/**
  * @brief Show LCD message for Time Edit mode
  * @param new_time Array holding the value of new time (hours, mins and secs)
  * @param edited_digit Index of edited time digit
  * @param blink_hide Variable for handling blinking of selected digit
  */
void LCD_TimeEdit(const uint8_t new_time[4], uint8_t edited_digit, uint8_t blink_hide);
/**
  * @brief Show LCD message for Brightness Edit mode
  * @param brightness_mode Current brigthness mode
  */
void LCD_BrightnessEdit(uint8_t brightness_mode);
/**
  * @brief Show LCD message for Clock mode
  * @param hours Current hour to be shown on screen
  * @param mins Current minutes to be shown on screen
  * @param secs Currents seconds to be shown on screen
  */
void LCD_ShowClock(uint8_t hours, uint8_t mins, uint8_t secs);
/**
  * @brief Show LCD message for Temperature, Pressure or Humidity only mode
  * @param pTitle Pointer to the title desired to be shown in first line
  * @param value Value to be shown in second line
  * @param pUnit_str Pointer to the string containing unit of measure
  */
void LCD_ShowValue(const char * pTitle, float value, const char * pUnit_str);
/**
  * @brief Show LCD custom message
  * @param pLine1 Pointer to the message to be shown in first line
  * @param pLine2 Pointer to the message to be shown in second line
  */
void LCD_ShowCustomText(const char * pLine1, const char * pLine2);

#endif /* INC_DISPLAY_MANAGER_H_ */
