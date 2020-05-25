/*
*	==========================================================================
*   lcd_ili9341.h	
*   (c) 2014, Petr Machala
*
*   Description:
*   ILI9341 control library for STM32F4xx with SPI communication only.
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

#ifndef LCD_ILI9341_H
#define LCD_ILI9341_H

// Includes
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "lcd_spi.h"
#include "lcd_fonts.h"

// SPI set
#define ILI9341_SPI						SPI5

// CS pin
#define ILI9341_CS_CLK				RCC_AHB1Periph_GPIOC
#define ILI9341_CS_PORT				GPIOC
#define ILI9341_CS_PIN				GPIO_Pin_2

// WRX pin
#define ILI9341_WRX_CLK				RCC_AHB1Periph_GPIOD
#define ILI9341_WRX_PORT			GPIOD
#define ILI9341_WRX_PIN				GPIO_Pin_13

// Reset pin
#define ILI9341_RST_CLK				RCC_AHB1Periph_GPIOD
#define ILI9341_RST_PORT			GPIOD
#define ILI9341_RST_PIN				GPIO_Pin_12

#define ILI9341_RST_SET				GPIO_SetBits(ILI9341_RST_PORT, ILI9341_RST_PIN)
#define ILI9341_RST_RESET			GPIO_ResetBits(ILI9341_RST_PORT, ILI9341_RST_PIN)
#define ILI9341_CS_SET				GPIO_SetBits(ILI9341_CS_PORT, ILI9341_CS_PIN)
#define ILI9341_CS_RESET			GPIO_ResetBits(ILI9341_CS_PORT, ILI9341_CS_PIN)
#define ILI9341_WRX_SET				GPIO_SetBits(ILI9341_WRX_PORT, ILI9341_WRX_PIN)
#define ILI9341_WRX_RESET			GPIO_ResetBits(ILI9341_WRX_PORT, ILI9341_WRX_PIN)

// LCD settings
#define ILI9341_WIDTH 				240
#define ILI9341_HEIGHT				320
#define ILI9341_PIXEL				76800
 
// Colors
#define ILI9341_COLOR_WHITE			0xFFFF
#define ILI9341_COLOR_BLACK			0x0000
#define ILI9341_COLOR_RED				0xF800
#define ILI9341_COLOR_GREEN			0x07E0
#define ILI9341_COLOR_GREEN2		0xB723
#define ILI9341_COLOR_BLUE			0x001F
#define ILI9341_COLOR_BLUE2			0x051D
#define ILI9341_COLOR_YELLOW		0xFFE0
#define ILI9341_COLOR_ORANGE		0xFBE4
#define ILI9341_COLOR_CYAN			0x07FF
#define ILI9341_COLOR_MAGENTA		0xA254
#define ILI9341_COLOR_GRAY			0x7BEF //1111 0111 1101 1110
#define ILI9341_COLOR_BROWN			0xBBCA

// Commands
#define ILI9341_RESET					0x01
#define ILI9341_SLEEP_OUT			0x11
#define ILI9341_GAMMA					0x26
#define ILI9341_DISPLAY_OFF		0x28
#define ILI9341_DISPLAY_ON		0x29
#define ILI9341_COLUMN_ADDR		0x2A
#define ILI9341_PAGE_ADDR			0x2B
#define ILI9341_GRAM					0x2C
#define ILI9341_MAC						0x36
#define ILI9341_PIXEL_FORMAT	0x3A
#define ILI9341_WDB						0x51
#define ILI9341_WCD						0x53
#define ILI9341_RGB_INTERFACE	0xB0
#define ILI9341_FRC						0xB1
#define ILI9341_BPC						0xB5
#define ILI9341_DFC						0xB6
#define ILI9341_POWER1				0xC0
#define ILI9341_POWER2				0xC1
#define ILI9341_VCOM1					0xC5
#define ILI9341_VCOM2					0xC7
#define ILI9341_POWERA				0xCB
#define ILI9341_POWERB				0xCF
#define ILI9341_PGAMMA				0xE0
#define ILI9341_NGAMMA				0xE1
#define ILI9341_DTCA					0xE8
#define ILI9341_DTCB					0xEA
#define ILI9341_POWER_SEQ			0xED
#define ILI9341_3GAMMA_EN			0xF2
#define ILI9341_INTERFACE			0xF6
#define ILI9341_PRC						0xF7

// Select orientation for LCD
typedef enum {
	LCD_ILI9341_Orientation_Portrait_1,
	LCD_ILI9341_Orientation_Portrait_2,
	LCD_ILI9341_Orientation_Landscape_1,
	LCD_ILI9341_Orientation_Landscape_2
} LCD_ILI9341_Orientation_t;

// Orientation, Used private
typedef enum {
	LCD_ILI9341_Landscape,
	LCD_ILI9341_Portrait
} LCD_ILI9341_Orientation;

// LCD options, Used private
typedef struct {
	uint16_t width;
	uint16_t height;
	LCD_ILI9341_Orientation orientation; // 1 = portrait; 0 = landscape
} LCD_ILI931_Options_t;


// Select font
extern LCD_FontDef_t LCD_Font_7x10;
extern LCD_FontDef_t LCD_Font_11x18;
extern LCD_FontDef_t LCD_Font_16x26;

/*
* Initialize ILI9341 LCD
*/
extern void LCD_ILI9341_Init(void);

/*
* Init LCD pins
*
* Called private
*/
extern void LCD_ILI9341_InitLCD(void);

/*
* Send data to LCD via SPI
*
* Called private
*
* Parameters:
* 	- uint8_t data: data to be sent
*/
extern void LCD_ILI9341_SendData(uint8_t data);

/*
* Send command to LCD via SPI
*
* Called private
*
* Parameters:
* 	- uint8_t data: data to be sent
*/
extern void LCD_ILI9341_SendCommand(uint8_t data);

/*
* Simple delay
*
* Parameters:
* 	- volatile unsigned int delay: clock cycles
*/
extern void LCD_ILI9341_Delay(volatile unsigned int delay);

/*
* Set cursor position
*
* Called private
*/
extern void LCD_ILI9341_SetCursorPosition(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

/*
* Draw single pixel to LCD
*
* Parameters:
* 	- uint16_t x: X position for pixel
* 	- uint16_t y: Y position for pixel
* 	- uint16_t color: color of pixel
*/
extern void LCD_ILI9341_DrawPixel(uint16_t x, uint16_t y, uint16_t color);

/*
* Fill entire LCD with color
*
* Parameters:
* 	- uint16_t color: Color to be used in fill
*/
extern void LCD_ILI9341_Fill(uint16_t color);

/*
* Show selected QVGA image on entire LCD
*
* Parameters:
* 	- uint16_t image: Array of selected image in RB565 format
*/
extern void LCD_ILI9341_DisplayImage(uint16_t image[ILI9341_PIXEL]);

/*
* Rotate LCD
* Select orientation
*
* Parameters:
* 	- LCD_ILI9341_Orientation_t orientation
*		- LCD_ILI9341_Orientation_Portrait_1: no rotation
*		- LCD_ILI9341_Orientation_Portrait_2: rotate 180deg
*		- LCD_ILI9341_Orientation_Landscape_1: rotate 90deg
*		- LCD_ILI9341_Orientation_Landscape_2: rotate -90deg
*/
extern void LCD_ILI9341_Rotate(LCD_ILI9341_Orientation_t orientation);

/*
* Put single character to LCD
*
* Parameters:
* 	- uint16_t x: X position of top left corner
* 	- uint16_t y: Y position of top left corner
* 	- char c: character to be displayed
* 	- LCD_FontDef_t *font: pointer to used font
* 	- uint16_t foreground: color for char
* 	- uint16_t background: color for char background
*/
extern void LCD_ILI9341_Putc(uint16_t x, uint16_t y, char c, LCD_FontDef_t *font, uint16_t foreground, uint16_t background);

/*
* Put string to LCD
*
* Parameters:
* 	- uint16_t x: X position of top left corner of first character in string
* 	- uint16_t y: Y position of top left corner of first character in string
* 	- char *str: pointer to first character
* 	- LCD_FontDef_t *font: pointer to used font
* 	- uint16_t foreground: color for string
* 	- uint16_t background: color for string background
*/
extern void LCD_ILI9341_Puts(uint16_t x, uint16_t y, char *str, LCD_FontDef_t *font, uint16_t foreground, uint16_t background);

/*
* Get width and height of box with text
*
* Parameters:
* 	- char *str: pointer to first character
* 	- LCD_FontDef_t *font: pointer to used font
* 	- uint16_t *width: Pointer to variable to store width
* 	- uint16_t *height: ointer to variable to store height
*/
extern void LCD_ILI9341_GetStringSize(char *str, LCD_FontDef_t *font, uint16_t *width, uint16_t *height);

/*
* Draw line to LCD
*
* Parameters:
* 	- uint16_t x0: X coordinate of starting point
* 	- uint16_t y0: Y coordinate of starting point
* 	- uint16_t x1: X coordinate of ending point
* 	- uint16_t y1: Y coordinate of ending point
* 	- uint16_t color: line color
*/
extern void LCD_ILI9341_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);

/*
* Draw rectangle on LCD
*
* Parameters:
* - uint16_t x0: X coordinate of top left point
* - uint16_t y0: Y coordinate of top left point
* - uint16_t x1: X coordinate of bottom right point
* - uint16_t y1: Y coordinate of bottom right point
* - uint16_t color: rectangle color
*/
extern void LCD_ILI9341_DrawRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);

/*
* Draw filled rectangle on LCD
*
* Parameters:
* - uint16_t x0: X coordinate of top left point
* - uint16_t y0: Y coordinate of top left point
* - uint16_t x1: X coordinate of bottom right point
* - uint16_t y1: Y coordinate of bottom right point
* - uint16_t color: rectangle color
*/
extern void LCD_ILI9341_DrawFilledRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);

/*
* Draw circle on LCD
*
* Parameters:
* - int16_t x0: X coordinate of center circle point
* - int16_t y0: Y coordinate of center circle point
* - int16_t r: circle radius
* - uint16_t color: circle color
*/
extern void LCD_ILI9341_DrawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);

/*
* Draw filled on LCD
*
* Parameters:
* - int16_t x0: X coordinate of center circle point
* - int16_t y0: Y coordinate of center circle point
* - int16_t r: circle radius
* - uint16_t color: circle color
*/
extern void LCD_ILI9341_DrawFilledCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);

#endif
