/**
  ******************************************************************************
  * @file    ui_control.c
  * @author  Lukasz Garlicki
  * @brief   Module handling the entire UI of weather station
  *
  * This module, being basically the core of the entire project,
  * is handling the entire UI task, that is showing the desired
  * information on LCD screen, validating the time in case of it
  * being edited as well as processing buttons
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "ui_control.h"
#include "button.h"
#include "adc_filter.h"
#include "bme690_port.h"
#include "adc_filter.h"
#include "display_manager.h"
#include "lcd.h"
#include "uart_transmit.h"

/* Definies ------------------------------------------------------------------*/
/* Time in milliseconds, used for software debouncing */
#define DEBOUNCING_TIME 200
/* Time in milliseconds, used for setting LCD refresh rate */
#define LCD_REFRESH_TIME 1000
/* Time in millisceonds, used for showing the currently edited digit in time edit */
#define BLINK_CHANGE_TIME 500


/* Variables ------------------------------------------------------------------*/
/* Extern handler of RTC, used to process HAL functions regarding RTC */
extern RTC_HandleTypeDef hrtc;

/* Current LCD state of display */
LCDState_t lcd_current_state = STATE_INIT;
/* Tick of last LCD update */
static uint16_t lcd_last_update = 0;

/* Current time */
static RTC_TimeTypeDef time;
/* Current date (used in code only to retrieve it alongside time due to ST advice) */
static RTC_DateTypeDef date;
/* Index of currently edited number in time edit mode */
static uint8_t edited_number = 0;
/* Placeholder for new time to set */
static uint8_t new_time[4] = {12, 0, 0, 0};
/* Position of digits in time to be edited */
const uint8_t digit_position[4] = {2, 3, 7, 8};
/* Last time of selected to edit digit blink in time edit mode*/
static uint32_t blink_last_time = 0;
/* PWM Duty (used for brightness settings) selected manually and to be set */
static uint16_t new_pwm_duty_cycle = 0;


void UI_Check_Time(uint8_t index){
    switch(index){
    case 0:
        if (new_time[0] > 2)
        {
            new_time[0] = 2;
        }
        if (new_time[0] == 2 && new_time[1] > 3)
        {
            new_time[1] = 3;
        }
    break;

    case 1:
        if (new_time[1] > 9)
        {
            new_time[1] = 9;
        }
        if (new_time[0] == 2 && new_time[1] > 3)
        {
            new_time[1] = 3;
        }
    break;

    case 2:
        if (new_time[2] > 5)
        {
            new_time[2] = 5;
        }
    break;

    case 3:
        if (new_time[3] > 9)
        {
            new_time[3] = 9;
        }
    break;
    }
}

void UI_Process_Buttons(void){
    static uint32_t last_button_time = 0;

    uint8_t up = Button_Up_State();
    uint8_t right = Button_Right_State();
    uint8_t left = Button_Left_State();
    uint8_t down = Button_Down_State();
    uint8_t enter = Button_Enter_State();

    if ((HAL_GetTick() - last_button_time) < DEBOUNCING_TIME)
    {
        return;
    }

    if (!up && !right && !left && !down && !enter)
    {
        return;
    }
    else
    {
        LCD_Clear();
        last_button_time = HAL_GetTick();
        lcd_last_update = 0;
    }

    switch(lcd_current_state){
    case STATE_COMPACT:
        if (down)
        {
            lcd_current_state = STATE_EDIT_TIME;

            new_time[0] = time.Hours / 10;
            new_time[1] = time.Hours % 10;
            new_time[2] = time.Minutes / 10;
            new_time[3] = time.Minutes % 10;
            edited_number = 0;
        }
        else if (right)
        {
            lcd_current_state = STATE_CLOCK;
        }
        else if (left)
        {
            lcd_current_state = STATE_HUMIDITY;
        }



    break;

    case STATE_EDIT_TIME:
        if (right)
        {
            blink_last_time = HAL_GetTick();
            if (edited_number < 3)
            {
                edited_number++;
            }

            last_button_time = HAL_GetTick();
            blink_last_time = 0;
        }
        else if (left)
        {
            blink_last_time = HAL_GetTick();

            if (edited_number > 0)
            {
                edited_number--;
            }

            last_button_time = HAL_GetTick();
            blink_last_time = 0;
        }
        else if (up)
        {
            blink_last_time = HAL_GetTick();
            new_time[edited_number]++;

            UI_Check_Time(edited_number);
            last_button_time = HAL_GetTick();
        }
        else if (down)
        {
            blink_last_time = HAL_GetTick();

            if (new_time[edited_number] > 0)
            {
                new_time[edited_number]--;
            }

            UI_Check_Time(edited_number);
            last_button_time = HAL_GetTick();
        }
        else if (enter)
        {
            time.Hours = new_time[0] * 10 + new_time[1];
            time.Minutes = new_time[2] * 10 + new_time[3];

            HAL_RTC_SetTime(&hrtc, &time, RTC_FORMAT_BIN);

            lcd_current_state = STATE_EDIT_BRIGHTNESS;
            last_button_time = HAL_GetTick();
        }
    break;

    case STATE_EDIT_BRIGHTNESS:
            if (enter)
            {
                lcd_current_state = STATE_COMPACT;
            }
            if (up)
            {
                if (lcd_brightness_mode == 100)
                {
                    ADC_StartAutoBrightness();
                }
                else if (lcd_brightness_mode == BRIGHTNESS_MODE_AUTO)
                {
                    lcd_brightness_mode = BRIGHTNESS_MODE_AUTO;
                }
                else
                {
                    lcd_brightness_mode += 10;
                    new_pwm_duty_cycle = ADC_GetPWMDutyCycle() + ADC_FULL_SCALE / 10;
                    ADC_SetPWMDutyCycle(new_pwm_duty_cycle);
                }
            }
            if (down)
            {
                if (lcd_brightness_mode == BRIGHTNESS_MODE_AUTO)
                {
                    ADC_StopAutoBrightness();
                }
                else if( lcd_brightness_mode == 0)
                {
                    lcd_brightness_mode = 0;
                    ADC_SetPWMDutyCycle(0);
                }
                else
                {
                    lcd_brightness_mode -= 10;
                    new_pwm_duty_cycle = ADC_GetPWMDutyCycle() - ADC_FULL_SCALE / 10;
                    ADC_SetPWMDutyCycle(new_pwm_duty_cycle);
                }

            }
            last_button_time = HAL_GetTick();
    break;

    case STATE_CLOCK:
        if(down)
        {
            lcd_current_state = STATE_EDIT_TIME;

            new_time[0] = time.Hours / 10;
            new_time[1] = time.Hours % 10;
            new_time[2] = time.Minutes / 10;
            new_time[3] = time.Minutes % 10;
            edited_number = 0;
        }

        if (right)
        {
            lcd_current_state = STATE_PRESSURE;
        }
        else if (left)
        {
            lcd_current_state = STATE_COMPACT;
        }

        last_button_time = HAL_GetTick();
    break;

    case STATE_PRESSURE:
            if (right)
            {
                lcd_current_state = STATE_TEMPERATURE;
            }
            else if (left)
            {
                lcd_current_state = STATE_CLOCK;
            }

            last_button_time = HAL_GetTick();
    break;

    case STATE_TEMPERATURE:
            if (right)
            {
                lcd_current_state = STATE_HUMIDITY;
            }
            else if (left)
            {
                lcd_current_state = STATE_PRESSURE;
            }

            last_button_time = HAL_GetTick();
    break;

    case STATE_HUMIDITY:
            if (right)
            {
                lcd_current_state = STATE_COMPACT;
            }
            else if (left)
            {
                lcd_current_state = STATE_TEMPERATURE;
            }

            last_button_time = HAL_GetTick();
    break;

    case STATE_BLANK:
    break;

    case STATE_INIT:
    break;

    };
}


void UI_UpdateDisplay(void)
{
    if ((HAL_GetTick() - lcd_last_update < LCD_REFRESH_TIME) || (LPUART1_IsBusy()))
    {
        return;
    }

    lcd_last_update = HAL_GetTick();

    ADC_SetBrightness();



    static uint8_t blink_hide = 0;
    static uint32_t blink_last_time = 0;

    if (HAL_GetTick() - blink_last_time >= BLINK_CHANGE_TIME)
    {
        blink_hide = !blink_hide;
        blink_last_time = HAL_GetTick();
    }
    if (Button_Up_State() || Button_Down_State())
    {
        blink_hide = 0;
        blink_last_time = HAL_GetTick();
    }

    switch (lcd_current_state)
    {
        case STATE_COMPACT:
            if (HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN) == HAL_OK)
            {
                HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);
            }

            LCD_ShowCompact((int)BME690_GetTemperature(),
                                (int)BME690_GetPressure(),
                                (int)BME690_GetHumidity(),
                                time.Hours, time.Minutes, time.Seconds);
        break;

        case STATE_EDIT_TIME:
            LCD_TimeEdit(new_time, edited_number, blink_hide);
        break;

        case STATE_EDIT_BRIGHTNESS:
            LCD_BrightnessEdit(lcd_brightness_mode);
        break;

        case STATE_CLOCK:

            if (HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN) == HAL_OK)
            {
                HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);
            }

            LCD_ShowClock(time.Hours, time.Minutes, time.Seconds);
        break;

        case STATE_PRESSURE:
            LCD_ShowValue("CISNIENIE", BME690_GetPressure(), "hPa");
        break;

        case STATE_TEMPERATURE:
            /*\xDF means degree in HD44780 */
            LCD_ShowValue("TEMPERATURA", BME690_GetTemperature(), "\xDF""C");
        break;

        case STATE_HUMIDITY:
            LCD_ShowValue("WILGOTNOSC", BME690_GetHumidity(), "%");
        break;

        case STATE_BLANK:
            LCD_Clear();
        break;

        case STATE_INIT:
            LCD_ShowCustomText("STACJA POGODOWA", "INICJALIZACJA...");
    }
}
