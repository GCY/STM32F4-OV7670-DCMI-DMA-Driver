/*
 *	==========================================================================
 *   lcd_ili9341.c	
 *   (c) 2014, Petr Machala
 *
 *   Description:
 *   ILI9341 control file for STM32F4xx with SPI communication only.
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

#include "lcd_ili9341.h"

uint16_t ILI9341_x;
uint16_t ILI9341_y;
LCD_ILI931_Options_t ILI9341_Opts;
uint8_t ILI9341_INT_CalledFromPuts = 0;

void LCD_ILI9341_Init() {
   GPIO_InitTypeDef GPIO_InitDef;

   RCC_AHB1PeriphClockCmd(ILI9341_WRX_CLK, ENABLE);	  
   GPIO_InitDef.GPIO_Pin = ILI9341_WRX_PIN;
   GPIO_InitDef.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_InitDef.GPIO_Mode = GPIO_Mode_OUT;
   GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
   GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
   GPIO_Init(ILI9341_WRX_PORT, &GPIO_InitDef);

   RCC_AHB1PeriphClockCmd(ILI9341_CS_CLK, ENABLE);
   GPIO_InitDef.GPIO_Pin = ILI9341_CS_PIN;
   GPIO_Init(ILI9341_CS_PORT, &GPIO_InitDef);	

   RCC_AHB1PeriphClockCmd(ILI9341_RST_CLK, ENABLE);
   GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_UP;
   GPIO_InitDef.GPIO_Pin = ILI9341_RST_PIN;
   GPIO_Init(ILI9341_RST_PORT, &GPIO_InitDef);	

   ILI9341_CS_SET;

   LCD_SPI_Init();

   LCD_ILI9341_InitLCD();	

   ILI9341_x = ILI9341_y = 0;

   ILI9341_Opts.width = ILI9341_WIDTH;
   ILI9341_Opts.height = ILI9341_HEIGHT;
   ILI9341_Opts.orientation = LCD_ILI9341_Portrait;
}

void LCD_ILI9341_InitLCD(void) {
   // ILI9341_RST_RESET;
   ILI9341_RST_SET;

   LCD_ILI9341_SendCommand(ILI9341_RESET);
   LCD_ILI9341_Delay(2000000);

   LCD_ILI9341_SendCommand(ILI9341_POWERA);
   LCD_ILI9341_SendData(0x39);
   LCD_ILI9341_SendData(0x2C);
   LCD_ILI9341_SendData(0x00);
   LCD_ILI9341_SendData(0x34);
   LCD_ILI9341_SendData(0x02);
   LCD_ILI9341_SendCommand(ILI9341_POWERB);
   LCD_ILI9341_SendData(0x00);
   LCD_ILI9341_SendData(0xC1);
   LCD_ILI9341_SendData(0x30);
   LCD_ILI9341_SendCommand(ILI9341_DTCA);
   LCD_ILI9341_SendData(0x85);
   LCD_ILI9341_SendData(0x00);
   LCD_ILI9341_SendData(0x78);
   LCD_ILI9341_SendCommand(ILI9341_DTCB);
   LCD_ILI9341_SendData(0x00);
   LCD_ILI9341_SendData(0x00);
   LCD_ILI9341_SendCommand(ILI9341_POWER_SEQ);
   LCD_ILI9341_SendData(0x64);
   LCD_ILI9341_SendData(0x03);
   LCD_ILI9341_SendData(0x12);
   LCD_ILI9341_SendData(0x81);
   LCD_ILI9341_SendCommand(ILI9341_PRC);
   LCD_ILI9341_SendData(0x20);
   LCD_ILI9341_SendCommand(ILI9341_POWER1);
   LCD_ILI9341_SendData(0x23);
   LCD_ILI9341_SendCommand(ILI9341_POWER2);
   LCD_ILI9341_SendData(0x10);
   LCD_ILI9341_SendCommand(ILI9341_VCOM1);
   LCD_ILI9341_SendData(0x3E);
   LCD_ILI9341_SendData(0x28);
   LCD_ILI9341_SendCommand(ILI9341_VCOM2);
   LCD_ILI9341_SendData(0x86);
   LCD_ILI9341_SendCommand(ILI9341_MAC);
   LCD_ILI9341_SendData(0x48);
   LCD_ILI9341_SendCommand(ILI9341_PIXEL_FORMAT);
   LCD_ILI9341_SendData(0x55);
   LCD_ILI9341_SendCommand(ILI9341_FRC);
   LCD_ILI9341_SendData(0x00);
   LCD_ILI9341_SendData(0x18);
   LCD_ILI9341_SendCommand(ILI9341_DFC);
   LCD_ILI9341_SendData(0x08);
   LCD_ILI9341_SendData(0x82);
   LCD_ILI9341_SendData(0x27);
   LCD_ILI9341_SendCommand(ILI9341_3GAMMA_EN);
   LCD_ILI9341_SendData(0x00);
   LCD_ILI9341_SendCommand(ILI9341_COLUMN_ADDR);
   LCD_ILI9341_SendData(0x00);
   LCD_ILI9341_SendData(0x00);
   LCD_ILI9341_SendData(0x00);
   LCD_ILI9341_SendData(0xEF);
   LCD_ILI9341_SendCommand(ILI9341_PAGE_ADDR);
   LCD_ILI9341_SendData(0x00);
   LCD_ILI9341_SendData(0x00);
   LCD_ILI9341_SendData(0x01);
   LCD_ILI9341_SendData(0x3F);
   LCD_ILI9341_SendCommand(ILI9341_GAMMA);
   LCD_ILI9341_SendData(0x01);
   LCD_ILI9341_SendCommand(ILI9341_PGAMMA);
   LCD_ILI9341_SendData(0x0F);
   LCD_ILI9341_SendData(0x31);
   LCD_ILI9341_SendData(0x2B);
   LCD_ILI9341_SendData(0x0C);
   LCD_ILI9341_SendData(0x0E);
   LCD_ILI9341_SendData(0x08);
   LCD_ILI9341_SendData(0x4E);
   LCD_ILI9341_SendData(0xF1);
   LCD_ILI9341_SendData(0x37);
   LCD_ILI9341_SendData(0x07);
   LCD_ILI9341_SendData(0x10);
   LCD_ILI9341_SendData(0x03);
   LCD_ILI9341_SendData(0x0E);
   LCD_ILI9341_SendData(0x09);
   LCD_ILI9341_SendData(0x00);
   LCD_ILI9341_SendCommand(ILI9341_NGAMMA);
   LCD_ILI9341_SendData(0x00);
   LCD_ILI9341_SendData(0x0E);
   LCD_ILI9341_SendData(0x14);
   LCD_ILI9341_SendData(0x03);
   LCD_ILI9341_SendData(0x11);
   LCD_ILI9341_SendData(0x07);
   LCD_ILI9341_SendData(0x31);
   LCD_ILI9341_SendData(0xC1);
   LCD_ILI9341_SendData(0x48);
   LCD_ILI9341_SendData(0x08);
   LCD_ILI9341_SendData(0x0F);
   LCD_ILI9341_SendData(0x0C);
   LCD_ILI9341_SendData(0x31);
   LCD_ILI9341_SendData(0x36);
   LCD_ILI9341_SendData(0x0F);
   LCD_ILI9341_SendCommand(ILI9341_SLEEP_OUT);

   LCD_ILI9341_Delay(1000000);

   LCD_ILI9341_SendCommand(ILI9341_DISPLAY_ON);
   LCD_ILI9341_SendCommand(ILI9341_GRAM);
}


void LCD_ILI9341_SendCommand(uint8_t data) {
   ILI9341_WRX_RESET;
   ILI9341_CS_RESET;
   LCD_SPI_Send(ILI9341_SPI, data);
   ILI9341_CS_SET;
}

void LCD_ILI9341_SendData(uint8_t data) {
   ILI9341_WRX_SET;
   ILI9341_CS_RESET;
   LCD_SPI_Send(ILI9341_SPI, data);
   ILI9341_CS_SET;
}

void LCD_ILI9341_DrawPixel(uint16_t x, uint16_t y, uint16_t color) {
   LCD_ILI9341_SetCursorPosition(x, y, x, y);

   LCD_ILI9341_SendCommand(ILI9341_GRAM);
   LCD_ILI9341_SendData(color >> 8);
   LCD_ILI9341_SendData(color & 0xFF);
}


void LCD_ILI9341_SetCursorPosition(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
   LCD_ILI9341_SendCommand(ILI9341_COLUMN_ADDR);
   LCD_ILI9341_SendData(x1 >> 8);
   LCD_ILI9341_SendData(x1 & 0xFF);
   LCD_ILI9341_SendData(x2 >> 8);
   LCD_ILI9341_SendData(x2 & 0xFF);

   LCD_ILI9341_SendCommand(ILI9341_PAGE_ADDR);
   LCD_ILI9341_SendData(y1 >> 8);
   LCD_ILI9341_SendData(y1 & 0xFF);
   LCD_ILI9341_SendData(y2 >> 8);
   LCD_ILI9341_SendData(y2 & 0xFF);
}

void LCD_ILI9341_Fill(uint16_t color) {
   unsigned int n, i, j;
   i = color >> 8;
   j = color & 0xFF;
   LCD_ILI9341_SetCursorPosition(0, 0, ILI9341_Opts.width - 1, ILI9341_Opts.height - 1);

   LCD_ILI9341_SendCommand(ILI9341_GRAM);

   for (n = 0; n < ILI9341_PIXEL; n++) {
      LCD_ILI9341_SendData(i);
      LCD_ILI9341_SendData(j);
   }
}

void LCD_ILI9341_DisplayImage(uint16_t image[ILI9341_PIXEL]) {
   uint32_t n, i, j;

   LCD_ILI9341_SetCursorPosition(0, 0, ILI9341_Opts.width - 1, ILI9341_Opts.height - 1);

   LCD_ILI9341_SendCommand(ILI9341_GRAM);

   ILI9341_WRX_SET;
   ILI9341_CS_RESET;

   for (n = 0; n < ILI9341_PIXEL; n++) {
      i = image[n] >> 8;
      j = image[n] & 0xFF;

      LCD_SPI_Send(ILI9341_SPI, i);
      LCD_SPI_Send(ILI9341_SPI, j);
   }

   ILI9341_CS_SET;
}

void LCD_ILI9341_Delay(volatile unsigned int delay) {
   for (; delay != 0; delay--); 
}

void LCD_ILI9341_Rotate(LCD_ILI9341_Orientation_t orientation) {
   LCD_ILI9341_SendCommand(ILI9341_MAC);
   if (orientation == LCD_ILI9341_Orientation_Portrait_1) {
      LCD_ILI9341_SendData(0x58);
   } else if (orientation == LCD_ILI9341_Orientation_Portrait_2) {
      LCD_ILI9341_SendData(0x88);
   } else if (orientation == LCD_ILI9341_Orientation_Landscape_1) {
      LCD_ILI9341_SendData(0x28);
   } else if (orientation == LCD_ILI9341_Orientation_Landscape_2) {
      LCD_ILI9341_SendData(0xE8);
   }

   if (orientation == LCD_ILI9341_Orientation_Portrait_1 || orientation == LCD_ILI9341_Orientation_Portrait_2) {
      ILI9341_Opts.width = ILI9341_WIDTH;
      ILI9341_Opts.height = ILI9341_HEIGHT;
      ILI9341_Opts.orientation = LCD_ILI9341_Portrait;
   } else {
      ILI9341_Opts.width = ILI9341_HEIGHT;
      ILI9341_Opts.height = ILI9341_WIDTH;
      ILI9341_Opts.orientation = LCD_ILI9341_Landscape;
   }
}

void LCD_ILI9341_Puts(uint16_t x, uint16_t y, char *str, LCD_FontDef_t *font, uint16_t foreground, uint16_t background) {
   uint16_t startX = x;

   /* Set X and Y coordinates */
   ILI9341_x = x;
   ILI9341_y = y;

   while (*str) {
      //New line
      if (*str == '\n') {
	 ILI9341_y += font->FontHeight + 1;
	 //if after \n is also \r, than go to the left of the screen
	 if (*(str + 1) == '\r') {
	    ILI9341_x = 0;
	    str++;
	 } else {
	    ILI9341_x = startX;
	 }
	 str++;
	 continue;
      } else if (*str == '\r') {
	 str++;
	 continue;
      }

      LCD_ILI9341_Putc(ILI9341_x, ILI9341_y, *str++, font, foreground, background);
   }
}

void LCD_ILI9341_GetStringSize(char *str, LCD_FontDef_t *font, uint16_t *width, uint16_t *height) {
   uint16_t w = 0;
   *height = font->FontHeight;
   while (*str++) {
      w += font->FontWidth;
   }
   *width = w;
}

void LCD_ILI9341_Putc(uint16_t x, uint16_t y, char c, LCD_FontDef_t *font, uint16_t foreground, uint16_t background) {
   uint32_t i, b, j;
   /* Set coordinates */
   ILI9341_x = x;
   ILI9341_y = y;
   if ((ILI9341_x + font->FontWidth) > ILI9341_Opts.width) {
      //If at the end of a line of display, go to new line and set x to 0 position
      ILI9341_y += font->FontHeight;
      ILI9341_x = 0;
   }
   for (i = 0; i < font->FontHeight; i++) {
      b = font->data[(c - 32) * font->FontHeight + i];
      for (j = 0; j < font->FontWidth; j++) {
	 if ((b << j) & 0x8000) {
	    LCD_ILI9341_DrawPixel(ILI9341_x + j, (ILI9341_y + i), foreground);
	 } else {
	    LCD_ILI9341_DrawPixel(ILI9341_x + j, (ILI9341_y + i), background);
	 }
      }
   }
   ILI9341_x += font->FontWidth;
}


void LCD_ILI9341_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color) {
   /* Code by dewoller: https://github.com/dewoller */

   int16_t dx, dy, sx, sy, err, e2; 

   /* Check for overflow */
   if (x0 >= ILI9341_Opts.width) {
      x0 = ILI9341_Opts.width - 1;
   }
   if (x1 >= ILI9341_Opts.width) {
      x1 = ILI9341_Opts.width - 1;
   }
   if (y0 >= ILI9341_Opts.height) {
      y0 = ILI9341_Opts.height - 1;
   }
   if (y1 >= ILI9341_Opts.height) {
      y1 = ILI9341_Opts.height - 1;
   }

   dx = (x0 < x1) ? (x1 - x0) : (x0 - x1); 
   dy = (y0 < y1) ? (y1 - y0) : (y0 - y1); 
   sx = (x0 < x1) ? 1 : -1; 
   sy = (y0 < y1) ? 1 : -1; 
   err = ((dx > dy) ? dx : -dy) / 2; 

   while (1) {
      LCD_ILI9341_DrawPixel(x0, y0, color); 
      if (x0 == x1 && y0 == y1) {
	 break;
      }
      e2 = err; 
      if (e2 > -dx) {
	 err -= dy;
	 x0 += sx;
      } 
      if (e2 < dy) {
	 err += dx;
	 y0 += sy;
      } 
   }
}

void LCD_ILI9341_DrawRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color) {
   LCD_ILI9341_DrawLine(x0, y0, x1, y0, color); //Top
   LCD_ILI9341_DrawLine(x0, y0, x0, y1, color);	//Left
   LCD_ILI9341_DrawLine(x1, y0, x1, y1, color);	//Right
   LCD_ILI9341_DrawLine(x0, y1, x1, y1, color);	//Bottom
}

void LCD_ILI9341_DrawFilledRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color) {
   for (; y0 < y1; y0++) {
      LCD_ILI9341_DrawLine(x0, y0, x1, y0, color);
   }
}

void LCD_ILI9341_DrawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
   int16_t f = 1 - r;
   int16_t ddF_x = 1;
   int16_t ddF_y = -2 * r;
   int16_t x = 0;
   int16_t y = r;

   LCD_ILI9341_DrawPixel(x0, y0 + r, color);
   LCD_ILI9341_DrawPixel(x0, y0 - r, color);
   LCD_ILI9341_DrawPixel(x0 + r, y0, color);
   LCD_ILI9341_DrawPixel(x0 - r, y0, color);

   while (x < y) {
      if (f >= 0) {
	 y--;
	 ddF_y += 2;
	 f += ddF_y;
      }
      x++;
      ddF_x += 2;
      f += ddF_x;

      LCD_ILI9341_DrawPixel(x0 + x, y0 + y, color);
      LCD_ILI9341_DrawPixel(x0 - x, y0 + y, color);
      LCD_ILI9341_DrawPixel(x0 + x, y0 - y, color);
      LCD_ILI9341_DrawPixel(x0 - x, y0 - y, color);

      LCD_ILI9341_DrawPixel(x0 + y, y0 + x, color);
      LCD_ILI9341_DrawPixel(x0 - y, y0 + x, color);
      LCD_ILI9341_DrawPixel(x0 + y, y0 - x, color);
      LCD_ILI9341_DrawPixel(x0 - y, y0 - x, color);
   }
}

void LCD_ILI9341_DrawFilledCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
   int16_t f = 1 - r;
   int16_t ddF_x = 1;
   int16_t ddF_y = -2 * r;
   int16_t x = 0;
   int16_t y = r;

   LCD_ILI9341_DrawPixel(x0, y0 + r, color);
   LCD_ILI9341_DrawPixel(x0, y0 - r, color);
   LCD_ILI9341_DrawPixel(x0 + r, y0, color);
   LCD_ILI9341_DrawPixel(x0 - r, y0, color);
   LCD_ILI9341_DrawLine(x0 - r, y0, x0 + r, y0, color);

   while (x < y) {
      if (f >= 0) {
	 y--;
	 ddF_y += 2;
	 f += ddF_y;
      }
      x++;
      ddF_x += 2;
      f += ddF_x;

      LCD_ILI9341_DrawLine(x0 - x, y0 + y, x0 + x, y0 + y, color);
      LCD_ILI9341_DrawLine(x0 + x, y0 - y, x0 - x, y0 - y, color);

      LCD_ILI9341_DrawLine(x0 + y, y0 + x, x0 - y, y0 + x, color);
      LCD_ILI9341_DrawLine(x0 + y, y0 - x, x0 - y, y0 - x, color);
   }
}  
