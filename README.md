# Weather Station STM32

An embedded weather station based on STM32 with self developed PCB engineered using KiCad. 

The system, as for now, offers measurements of humidity, pressure and temperature with simple moving average filtering, 
alongside visualizing the data on LCD screen based on HD44780 controller, as well as sending data in JSON format by UART transmission

## Main Functions
* **Measurements**
  * Temperature, humidity and pressure (BME690)
* **Visualization**
  * LCD 2x16 based on HD44780 controller
  * Brightness set either manually or automatically, based on light level (measured by photoresistor)
* **Connectivity**
  * UART DMA communication for sending data in JSON format
 
## Hardware
* Custom-designed PCB developed in KiCad
* MCU: STM32L476RG6
* Weather sensor: BME690

## Future plans
* Handling air quality control with BME690
* Moving the entire architecture to RTOS
* Improve UART communication reliability during LCD updates

## Known issues
* UART tends to send garbage data when the LCD is being refreshed - a flag is used to omit the refreshement of LCD when UART is being transmitted

## Project status
Project is under active development
