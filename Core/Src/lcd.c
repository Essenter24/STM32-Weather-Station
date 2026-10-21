/**
  ******************************************************************************
  * @file    lcd.c
  * @author  Lukasz Garlicki
  * @brief   LCD Driver
  *
  *
  * This module brings all of the required functions, based on HD44780 driver
  * datasheet, required to operate LCD in 4-bit mode.
  *
  * Pin writings are done straight on registers to ensure fastest execution
  ******************************************************************************
  */

#include "lcd.h"

/**
  * @brief Delay function for LCD, measuring delay in microseconds
  * @param us Desired amount of microseconds to wait
  */
static void LCD_Delay(uint32_t us)
{
	volatile uint32_t starting_ticks = DWT->CYCCNT;
	uint32_t required_ticks = us * (SystemCoreClock / 1000000U);

	while ((DWT->CYCCNT - starting_ticks) < required_ticks);
}
/**
  * @brief Writing 4-bit data to the LCD in 4-bit mode
  * @param data Bits meant to be written to the LCD
  */
static void LCD_Send4Bits(uint8_t data)
{
	if (data & 0x001)
	{
	    LCD_DB4_GPIO_Port->BSRR = LCD_DB4_Pin;
	}
	else
	{
	    LCD_DB4_GPIO_Port->BRR = LCD_DB4_Pin;
	}
	if (data & 0x002)
	{
        LCD_DB5_GPIO_Port->BSRR = LCD_DB5_Pin;
	}
	else
	{
	    LCD_DB5_GPIO_Port->BRR = LCD_DB5_Pin;
	}
	if (data & 0x004)
	{
	    LCD_DB6_GPIO_Port->BSRR = LCD_DB6_Pin;
	}
	else
	{
	    LCD_DB6_GPIO_Port->BRR = LCD_DB6_Pin;
	}
	if (data & 0x008)
	{
	    LCD_DB7_GPIO_Port->BSRR = LCD_DB7_Pin;
	}
	else
	{
	    LCD_DB7_GPIO_Port->BRR = LCD_DB7_Pin;
	}

	LCD_E_GPIO_Port->BSRR = LCD_E_Pin;
	LCD_Delay(5);
	LCD_E_GPIO_Port->BRR = LCD_E_Pin;
	LCD_Delay(60);
}

void LCD_WriteByte(uint8_t byte, uint8_t rs){

    /* Selecting either command or data mode
     * rs = 1 -> data,
     * rs = 0 -> command */
	if (rs)
	{
	    LCD_RS_GPIO_Port->BSRR = LCD_RS_Pin;
	}
	else
	{
		LCD_RS_GPIO_Port->BRR = LCD_RS_Pin;
	}


	/* Sending 4 the most significant bits first */
	LCD_Send4Bits(byte >> 4);

	/* Sending 4 the least significant bits second */
	LCD_Send4Bits(byte & 0x0F);
}

void LCD_Init(void)
{
    /* All of these steps are recommended by HD44780 datasheet for LCD initialization */
	LCD_Delay(50000);

	LCD_Send4Bits(0x003);
	LCD_Delay(5000);

	LCD_Send4Bits(0x003);
	LCD_Delay(100);

	LCD_Send4Bits(0x003);
	LCD_Delay(2000);

	LCD_Send4Bits(0x002);
	LCD_Delay(1000);

	LCD_WriteByte(0x028, 0);
	LCD_WriteByte(0x008, 0);
	LCD_WriteByte(0x001, 0);
	LCD_Delay(1000);
	LCD_WriteByte(0x006, 0);
	LCD_Delay(1000);
	LCD_WriteByte(0x0C, 0);

}

void LCD_Clear(void)
{
	LCD_WriteByte(0x001, 0);
	LCD_Delay(3000);
}

void LCD_Print(const char * p_str)
{
	if (p_str == NULL)
    {
	    return;
    }
	while (*p_str)
	{
		LCD_WriteByte(*p_str, 1);
		p_str++;
	}
}

void LCD_SetCursor(uint8_t column, uint8_t row)
{
    /* Hard-coded offset for rows (allowing for technically handling 4x16 LCD) */
	static const uint8_t row_index[] = {0x00, 0x40, 0x14, 0x54};

	if(row >= 4 || column >= 16)
	{
	    return;
	}

	LCD_WriteByte(0x80 + column + row_index[row], 0);
}

