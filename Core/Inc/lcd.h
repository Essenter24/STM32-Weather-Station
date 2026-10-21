/**
  ******************************************************************************
  * @file    lcd.h
  * @author  Lukasz Garlicki
  * @brief   Header file of LCD driver
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef INC_LCD_H_
#define INC_LCD_H_

#include <stdint.h>
#include <gpio.h>

/**
  * @brief Write entire byte of data to LCD
  * @param byte Byte to write to the LCD
  * @param rs Setting the data (rs = 1) or command (rs = 0) mode in LCD
  */
void LCD_WriteByte(uint8_t byte, uint8_t rs);
/**
  * @brief Initialize LCD screen with datasheet steps
  */
void LCD_Init(void);
/**
  * @brief Clear LCD screen
  */
void LCD_Clear(void);
/**
  * @brief Write a string to the LCD
  * @param pStr Pointer to the string meant to be written to the LCD
  */
void LCD_Print(const char * pStr);
/**
  * @brief Set cursor in desired column and row of LCD screen
  * @param column Desired column
  * @param row Desired row
  */
void LCD_SetCursor(uint8_t column, uint8_t row);


#endif /* INC_LCD_H_ */
