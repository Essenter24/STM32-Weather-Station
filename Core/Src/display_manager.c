/**
  ******************************************************************************
  * @file    display_manager.c
  * @author  Lukasz Garlicki
  * @brief   LCD display manager
  *
  * This module handles the entire LCD message printing in every LCD state,
  * allowing for showing either core information, clock information,
  * pressure, temperature or humidity information, as well as handling the
  * time and brightness edition screen.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "display_manager.h"
#include "lcd.h"
#include <stdio.h>

/* Variables ------------------------------------------------------------------*/
/* Array holding positions of digits available for editing in time configuration state */
static const uint8_t digit_position[4] = {2, 3, 7, 8};

static char lcd_buffer[32];

void LCD_ShowCompact(int16_t temp, uint16_t pressure, uint16_t humidity,
        uint8_t hours, uint8_t minutes, uint8_t seconds)
{

    LCD_SetCursor(0, 0);
    snprintf(lcd_buffer, sizeof(lcd_buffer), "C:%02i T:%02i W:%02i", pressure, temp, humidity);
    LCD_Print(lcd_buffer);

    LCD_SetCursor(0, 1);
    snprintf(lcd_buffer, sizeof(lcd_buffer), "G:%02i M:%02i S:%02i  ", hours, minutes, seconds);
    LCD_Print(lcd_buffer);
}

void LCD_TimeEdit(const uint8_t new_time[4], uint8_t edited_digit, uint8_t blink_hide)
{

    LCD_SetCursor(0, 0);
    LCD_Print("   USTAW CZAS   ");

    LCD_SetCursor(0, 1);
    snprintf(lcd_buffer, sizeof(lcd_buffer), "G:%i%i M:%i%i S:00",
            new_time[0], new_time[1], new_time[2], new_time[3]);

    /* 'Blinking' of currently selected digit */
    if (blink_hide && edited_digit < 4)
    {
        lcd_buffer[digit_position[edited_digit]] = ' ';
    }

    LCD_Print(lcd_buffer);
    LCD_SetCursor(digit_position[edited_digit], 1);
}

void LCD_BrightnessEdit(uint8_t brightness_mode)
{

    LCD_SetCursor(0, 0);
    LCD_Print("  USTAW JASNOSC ");

    LCD_SetCursor(0, 1);
    if (brightness_mode == 125)
    {
        LCD_Print("      AUTO      ");
    }
    else
    {
        snprintf(lcd_buffer, sizeof(lcd_buffer), "      %02i%%     ", brightness_mode);
        LCD_Print(lcd_buffer);
    }
}

void LCD_ShowClock(uint8_t hours, uint8_t minutes, uint8_t seconds)
{

    LCD_SetCursor(0, 0);
    LCD_Print("     ZEGAR      ");

    LCD_SetCursor(0, 1);
    snprintf(lcd_buffer, sizeof(lcd_buffer), "    %02i:%02i:%02i    ", hours, minutes, seconds);
    LCD_Print(lcd_buffer);
}

void LCD_ShowValue(const char * p_title, float value, const char * p_unit_str)
{

    LCD_SetCursor(0, 0);
    snprintf(lcd_buffer, sizeof(lcd_buffer), "   %-16s", p_title);
    LCD_Print(lcd_buffer);

    LCD_SetCursor(0, 1);
    snprintf(lcd_buffer, sizeof(lcd_buffer), "    %.1f %-4s    ", value, p_unit_str);
    LCD_Print(lcd_buffer);
}

void LCD_ShowCustomText(const char * p_line1, const char * p_line2)
{
    LCD_SetCursor(0, 0);
    LCD_Print(p_line1);
    LCD_SetCursor(0, 1);
    LCD_Print(p_line2);
}
