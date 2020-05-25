/*
 *	==========================================================================
 *   lcd_spi.c	
 *   (c) 2014, Petr Machala
 *
 *   Description:
 *   SPI file for LCD display communication.
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

#include "lcd_spi.h"

void LCD_SPI_Init(void) {
   GPIO_InitTypeDef GPIO_InitStruct;
   SPI_InitTypeDef SPI_InitStruct;

   //Common settings for all pins
   GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
   GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
   GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
   GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;

   //Enable clock
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
   //Pinspack nr. 1        SCK          MISO         MOSI
   GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
   GPIO_Init(GPIOF, &GPIO_InitStruct);

   GPIO_PinAFConfig(GPIOF, GPIO_PinSource7, GPIO_AF_SPI5);
   GPIO_PinAFConfig(GPIOF, GPIO_PinSource8, GPIO_AF_SPI5);
   GPIO_PinAFConfig(GPIOF, GPIO_PinSource9, GPIO_AF_SPI5);

   RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI5, ENABLE);

   SPI_StructInit(&SPI_InitStruct);
   SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
   SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
   SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
   SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
   SPI_InitStruct.SPI_Mode = SPI_Mode_Master;	
   SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
   SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
   SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
   SPI_Init(SPI5, &SPI_InitStruct);
   SPI_Cmd(SPI5, ENABLE);
}

uint8_t LCD_SPI_Send(SPI_TypeDef* SPIx, uint8_t data) {
   //Fill output buffer with data
   SPIx->DR = data;
   //Wait for transmission to complete
   while (!SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE));
   //Wait for received data to complete
   while (!SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE));
   //Wait for SPI to be ready
   while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_BSY));
   //Return data from buffer
   return SPIx->DR;
}

void LCD_SPI_SendMulti(SPI_TypeDef* SPIx, uint8_t *dataOut, uint8_t *dataIn, uint16_t count) {
   uint8_t i;
   for (i = 0; i < count; i++) {
      dataIn[i] = LCD_SPI_Send(SPIx, dataOut[i]);
   }
}

void LCD_SPI_WriteMulti(SPI_TypeDef* SPIx, uint8_t *dataOut, uint16_t count) {
   uint16_t i;
   for (i = 0; i < count; i++) {
      LCD_SPI_Send(SPIx, dataOut[i]);
   }
}

void LCD_SPI_ReadMulti(SPI_TypeDef* SPIx, uint8_t *dataIn, uint8_t dummy, uint16_t count) {
   uint16_t i;
   for (i = 0; i < count; i++) {
      dataIn[i] = LCD_SPI_Send(SPIx, dummy);
   }
}

void LCD_SPI_BaudRateUp(void){
   SPI_InitTypeDef SPI_InitStruct;

   SPI_Cmd(SPI5, DISABLE);

   RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI5, ENABLE);

   SPI_StructInit(&SPI_InitStruct);
   SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
   SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
   SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
   SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
   SPI_InitStruct.SPI_Mode = SPI_Mode_Master;	
   SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
   SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
   SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
   SPI_Init(SPI5, &SPI_InitStruct);
   SPI_Cmd(SPI5, ENABLE);

   SPI_Cmd(SPI5, ENABLE);
}
