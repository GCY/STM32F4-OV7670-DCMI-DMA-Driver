/*
*	==========================================================================
*   lcd_spi.h	
*   (c) 2014, Petr Machala
*
*   Description:
*   SPI library for LCD display communication.
*   Optimized for 32F429IDISCOVERY board.
*
*   This program is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   any later version.
*   
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*  
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*	==========================================================================
*/

// Includes
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_spi.h"

/*
* SPI5 Init, uses pins:
* 	- Pins pack 1:
* 		- MOSI: PF9
* 		- MISO: PF8
* 		- SCK:  PF7
*/
extern void LCD_SPI_Init(void);

/*
* Send and receive data over SPI
*
* Parameters:
* 	- SPI_TypeDef* SPIx: Select SPI which will operate with data
* 	- uint8_t data: data to be sent
*
* Returns: Data returned from slave
*/
extern uint8_t LCD_SPI_Send(SPI_TypeDef* SPIx, uint8_t data);

/*
* Send and receive multiple data bytes over SPI
*
* Parameters:
* 	- SPI_TypeDef* SPIx: Select SPI which will operate with data
* 	- uint8_t dataOut: pointer to data to be sent out
*	- uint8_t dataIn: pointer to received data
*	- uint8_t count: number of bytes to send
*
* No returns
*/
extern void LCD_SPI_SendMulti(SPI_TypeDef* SPIx, uint8_t *dataOut, uint8_t *dataIn, uint16_t count);

/*
* Write multiple data via SPI
*
* Parameters:
* 	- SPI_TypeDef* SPIx: Select SPI which will operate with data
* 	- uint8_t dataOut: pointer to data to be sent out
*	- uint8_t count: number of bytes to send
*
* No returns
*/
extern void LCD_SPI_WriteMulti(SPI_TypeDef* SPIx, uint8_t *dataOut, uint16_t count);

/*
* Send and receive multiple data bytes over SPI
*
* Parameters:
* 	- SPI_TypeDef* SPIx: Select SPI which will operate with data
*	- uint8_t dataIn: pointer to received data
*	- uint8_t dummy: dummy byte to be sent to SPI
*	- uint8_t count: number of bytes to receive
*
* No returns
*/
extern void LCD_SPI_ReadMulti(SPI_TypeDef* SPIx, uint8_t *dataIn, uint8_t dummy, uint16_t count);

/*
* Increse SPI baudrate 
*/
extern void LCD_SPI_BaudRateUp(void);
