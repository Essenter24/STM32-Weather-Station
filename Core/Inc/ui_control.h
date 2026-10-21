/**
  ******************************************************************************
  * @file    ui_control.h
  * @author  Lukasz Garlicki
  * @brief   Header file of UI control module
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef INC_UI_CONTROL_H_
#define INC_UI_CONTROL_H_

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/**
 * @brief Represents the current state of the LCD display.
 */
typedef enum
{
    STATE_COMPACT = 0,      /*!< Compact display state, showing core info without decimals */
    STATE_EDIT_TIME,        /*!< Time configuration state */
    STATE_EDIT_BRIGHTNESS,  /*!< Brigthness configuration state */
    STATE_CLOCK,            /*!< Clock display state */
    STATE_PRESSURE,         /*!< Pressure display state, with decimals */
    STATE_TEMPERATURE,      /*!< Temperature display state, with decimals */
    STATE_HUMIDITY,         /*!< Humidity display state, with decimals */
    STATE_INIT,             /*!< Initialization of sensors and ADC */
    STATE_BLANK             /*!< Blank display state */

} LCDState_t;


/* Variable ------------------------------------------------------------------*/

/* Current LCD brightness mode (either level of brightness or AUTO mode) */
extern uint8_t lcd_brightness_mode;
/* Current state of LCD */
extern LCDState_t lcd_current_state;

/**
  * @brief Update display with information corresponding to LCD state
  */
void UI_UpdateDisplay(void);
/**
  * @brief Validate whether set time follows the rules of 24h format
  * @param index Index of time element that is being changed and needs validation
  */
void UI_Check_Time(uint8_t index);
/**
  * @brief Process pressing of any of the functions and perform certain task, e.g.
  * change display state or allow for time/brightness modification
  */
void UI_Process_Buttons(void);

#endif /* INC_UI_CONTROL_H_ */
