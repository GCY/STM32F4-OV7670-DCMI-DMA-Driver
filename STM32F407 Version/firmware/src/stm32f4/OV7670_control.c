/*
 *	==========================================================================
 *   OV7670_control.c	
 *   (c) 2014, Petr Machala
 *
 *   Description:
 *   OV7670 camera configuration and control file.
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
 *
 *   Camera wiring:
 *   3V3		-	3V		;		GND		-	GND
 *   SIOC	-	PB8		;		SIOD	-	PB9
 *   VSYNC -	PB7		;		HREF	-	PA4
 *   PCLK	-	PA6		;		XCLK	-	PA8
 *   D7		-	PE6		;		D6		-	PE5
 *   D5		-	PB6		;		D4		-	PE4
 *   D3		-	PC9		;		D2		-	PC8
 *   D1		-	PC7		;		D0		-	PC6
 *   RESET	-	/			;		PWDN	-	/
 *		
 *	==========================================================================
 */

#include "OV7670_reg.h"
#include "OV7670_control.h"

// Image buffer
volatile uint16_t frame_buffer[IMG_ROWS*IMG_COLUMNS];

const uint8_t OV7670_reg_org [OV7670_REG_NUM][2] = {				
   {0x12, 0x80},		//Reset registers

   // Image format
   {0x12, 0x14},		//QVGA size, RGB mode

   {0x40, 0xd0},		//RGB565
   {0xb0, 0x84},		//Color mode

   // Hardware window
   {0x11, 0x01},		//PCLK settings, 15fps
   {0x32, 0x80},		//HREF
   {0x17, 0x17},		//HSTART
   {0x18, 0x05},		//HSTOP
   {0x03, 0x0a},		//VREF
   {0x19, 0x02},		//VSTART
   {0x1a, 0x7a},		//VSTOP
   /*
   {0x17,0x16},
   {0x18,0x04},
   {0x32,0x24},
   {0x19,0x02},
   {0x1a,0x7a},
   {0x03,0x0a},
*/
   // Scalling numbers
   {0x70, 0x3a},		//X_SCALING
   {0x71, 0x35},		//Y_SCALING
   /*{0x72, 0x11},		//DCW_SCALING
   {0x73, 0xf0},		//PCLK_DIV_SCALING*/
   /*{0x72, 0x11},
   {0x73, 0xf1},*/
   {0x72, 0x22}, //20190712
   {0x73, 0xf2}, //20190712 
   {0xa2, 0x02},		//PCLK_DELAY_SCALING

   // Matrix coefficients
   {0x4f, 0x80},		{0x50, 0x80},
   {0x51, 0x00},		{0x52, 0x22},
   {0x53, 0x5e},		{0x54, 0x80},
   {0x58, 0x9e},{0x56,0x40},

   // Gamma curve values
   {0x7a, 0x20},		{0x7b, 0x10},
   {0x7c, 0x1e},		{0x7d, 0x35},
   {0x7e, 0x5a},		{0x7f, 0x69},
   {0x80, 0x76},		{0x81, 0x80},
   {0x82, 0x88},		{0x83, 0x8f},
   {0x84, 0x96},		{0x85, 0xa3},
   {0x86, 0xaf},		{0x87, 0xc4},
   {0x88, 0xd7},		{0x89, 0xe8},

   // AGC and AEC parameters
   {0xa5, 0x05},		{0xab, 0x07},
   {0x24, 0x95},		{0x25, 0x33},
   {0x26, 0xe3},		{0x9f, 0x78},
   {0xa0, 0x68},		{0xa1, 0x03},
   {0xa6, 0xd8},		{0xa7, 0xd8},
   {0xa8, 0xf0},		{0xa9, 0x90},
   {0xaa, 0x94},		{0x10, 0x00},

   // AWB parameters
   {0x43, 0x0a},		{0x44, 0xf0},
   {0x45, 0x34},		{0x46, 0x58},
   {0x47, 0x28},		{0x48, 0x3a},
   {0x59, 0x88},		{0x5a, 0x88},
   {0x5b, 0x44},		{0x5c, 0x67},
   {0x5d, 0x49},		{0x5e, 0x0e},
   {0x6c, 0x0a},		{0x6d, 0x55},
   {0x6e, 0x11},		{0x6f, 0x9f},

   {0x6a, 0x40},		{0x01, 0x40},
   {0x02, 0x60},		{0x13, 0xe7},

   // Additional parameters
   {0x34, 0x11},		{0x3f, 0x00},
   {0x75, 0x05},		{0x76, 0xe1},
   {0x4c, 0x00},		{0x77, 0x01},
   {0xb8, 0x0a},		{0x41, 0x18},
   {0x3b, 0x12},		{0xa4, 0x88},
   {0x96, 0x00},		{0x97, 0x30},
   {0x98, 0x20},		{0x99, 0x30},
   {0x9a, 0x84},		{0x9b, 0x29},
   {0x9c, 0x03},		{0x9d, 0x4c},
   {0x9e, 0x3f},		{0x78, 0x04},
   {0x0e, 0x61},		{0x0f, 0x4b},
   {0x16, 0x02},		{0x1e, 0x00},
   {0x21, 0x02},		{0x22, 0x91},
   {0x29, 0x07},		{0x33, 0x0b},
   {0x35, 0x0b},		{0x37, 0x1d},
   {0x38, 0x71},		{0x39, 0x2a},
   {0x3c, 0x78},		{0x4d, 0x40},
   {0x4e, 0x20},		{0x69, 0x00},
   {0x6b, 0x3a},		{0x74, 0x10},
   {0x8d, 0x4f},		{0x8e, 0x00},
   {0x8f, 0x00},		{0x90, 0x00},
   {0x91, 0x00},		{0x96, 0x00},
   {0x9a, 0x00},		{0xb1, 0x0c},
   {0xb2, 0x0e},		{0xb3, 0x82},
   {0x4b, 0x01},
/*
   {0x0c, 0x04},
   {0x3e, 0x19}*/
   {0x3e, 0x1a}, //20190712
   {0x0c, 0x04}, //20190712
};

const uint8_t OV7670_reg [OV7670_REG_NUM][2] = {				
   {0x12, 0x80},		//Reset registers

   // Image format
   {0x12, /*0x14*/0x04},		//QVGA size, RGB mode //20190725

   {0x40, 0xd0},		//RGB565
   {0xb0, 0x84},		//Color mode

   // Hardware window
   {0x11, 0x01},		//PCLK settings, 15fps
   /*{0x32, 0x80},		//HREF
   {0x17, 0x17},		//HSTART
   {0x18, 0x05},		//HSTOP
   {0x03, 0x0a},		//VREF
   {0x19, 0x02},		//VSTART
   {0x1a, 0x7a},		//VSTOP
*/
   /*
                         {0x17, 0x13},	{0x18, 0x01},
                         {0x32, 0xb6},	{0x19, 0x02},
                         {0x1a, 0x7a},	{0x03, 0x0a},
			 */
   
   {0x17,0x16},
   {0x18,0x04},
   {0x32,0x24},
   {0x19,0x02},
   {0x1a,0x7a},
   {0x03,0x0a},

   // Scalling numbers
   {0x70, 0x3a},		//X_SCALING
   {0x71, 0x35},		//Y_SCALING
      /*{0x70, 0x80},		//X_SCALING test
   {0x71, 0x80},		//Y_SCALING test*/

   /*{0x72, 0x11},		//DCW_SCALING
   {0x73, 0xf0},		//PCLK_DIV_SCALING*/
   /*{0x72, 0x11},
   {0x73, 0xf1},*/
   {0x72, 0x22}, //20190712
   {0x73, 0xf2}, //20190712 
   {0xa2, 0x01},		//PCLK_DELAY_SCALING

   // Matrix coefficients
   /*{0x4f, 0x80},		{0x50, 0x80},
   {0x51, 0x00},		{0x52, 0x22},
   {0x53, 0x5e},		{0x54, 0x80},*/	//20190725
    {0x4f, 0xb3},		 /* "matrix coefficient 1" */
    {0x50, 0xb3},		 /* "matrix coefficient 2" */
    {0x51, 0},		 /* vb */
    {0x52, 0x3d},		 /* "matrix coefficient 4" */
    {0x53, 0xa7},		 /* "matrix coefficient 5" */
    {0x54, 0xe4},		 /* "matrix coefficient 6" */
   {0x58, 0x9e},

   // Gamma curve values
   {0x7a, 0x20},		{0x7b, 0x10},
   {0x7c, 0x1e},		{0x7d, 0x35},
   {0x7e, 0x5a},		{0x7f, 0x69},
   {0x80, 0x76},		{0x81, 0x80},
   {0x82, 0x88},		{0x83, 0x8f},
   {0x84, 0x96},		{0x85, 0xa3},
   {0x86, 0xaf},		{0x87, 0xc4},
   {0x88, 0xd7},		{0x89, 0xe8},

   // AGC and AEC parameters
   {0xa5, 0x05},		{0xab, 0x07},
   {0x24, 0x95},		{0x25, 0x33},
   {0x26, 0xe3},		{0x9f, 0x78},
   {0xa0, 0x68},		{0xa1, 0x03},
   {0xa6, 0xd8},		{0xa7, 0xd8},
   {0xa8, 0xf0},		{0xa9, 0x90},
   {0xaa, 0x94},		{0x10, 0x00},

   // AWB parameters
   {0x43, 0x0a},		{0x44, 0xf0},
   {0x45, 0x34},		{0x46, 0x58},
   {0x47, 0x28},		{0x48, 0x3a},
   {0x59, 0x88},		{0x5a, 0x88},
   {0x5b, 0x44},		{0x5c, 0x67},
   {0x5d, 0x49},		{0x5e, 0x0e},
   {0x6c, 0x0a},		{0x6d, 0x55},
   {0x6e, 0x11},		{0x6f, /*0x9f*/0x9e}, //20190725

   {0x6a, 0x40},		{0x01, 0x40},
   {0x02, 0x60},		{0x13, 0xe7},

   // Additional parameters
   {0x34, 0x11},		{0x3f, 0x00},
   {0x75, 0x05},		{0x76, 0xe1},
   {0x4c, 0x00},		{0x77, 0x01},
   {0xb8, 0x0a},		{0x41, 0x08},
   {0x3b, 0x12},		{0xa4, 0x82},
   {0x96, 0x00},		{0x97, 0x30},
   {0x98, 0x20},		{0x99, 0x30},
   {0x9a, 0x84},		{0x9b, 0x29},
   {0x9c, 0x03},		{0x9d, 0x4c},
   {0x9e, 0x3f},		{0x78, 0x04},
   {0x0e, 0x61},		{0x0f, 0x4b},
   {0x16, 0x02},		{0x1e, 0x07},
   {0x21, 0x02},		{0x22, 0x91},
   {0x29, 0x07},		{0x33, 0x0b},
   {0x35, 0x0b},		{0x37, 0x1d},
   {0x38, 0x71},		{0x39, 0x2a},
   {0x3c, 0x78},		{0x4d, 0x40},
   {0x4e, 0x20},		{0x69, 0x00},
   {0x6b, 0x3a},		{0x74, 0x10},
   {0x8d, 0x4f},		{0x8e, 0x00},
   {0x8f, 0x00},		{0x90, 0x00},
   {0x91, 0x00},		{0x96, 0x00},
   {0x9a, 0x00},		{0xb1, 0x0c},
   {0xb2, 0x0e},		{0xb3, 0x82},
   {0x4b, 0x01},
/*
   {0x0c, 0x04},
   {0x3e, 0x19}*/
   {0x3e, 0x1a}, //20190712
   {0x0c, 0x04}, //20190712
   {0x3d,0x40}, // //20190725
   {0x14,0x6A}, // //20190725
   {0x30,0x0}, // //20190725
   {0x31,0x0}, // //20190725
   {0x15, 0x02}, // //20190726
};

const uint8_t OV7670_reg2 [OV7670_REG_NUM][2] = {				
   {0x12, 0x80},		//Reset registers

   // Image format
   {0x12, 0x14},		//QVGA size, RGB mode
   {0x40, 0xd0},		//RGB565
   {0xb0, 0x84},		//Color mode

   // Hardware window
   {0x11, 0x01},		//PCLK settings, 15fps
   {0x32, 0x80},		//HREF
   {0x17, 0x17},		//HSTART
   {0x18, 0x05},		//HSTOP
   {0x03, 0x0a},		//VREF
   {0x19, 0x02},		//VSTART
   {0x1a, 0x7a},		//VSTOP

   // Scalling numbers
   {0x70, 0x3a},		//X_SCALING
   {0x71, 0x35},		//Y_SCALING
   {0x72, 0x11},		//DCW_SCALING
   {0x73, 0xf0},		//PCLK_DIV_SCALING
   {0xa2, 0x02},		//PCLK_DELAY_SCALING

   // Matrix coefficients
   {0x4f, 0x80},		{0x50, 0x80},
   {0x51, 0x00},		{0x52, 0x22},
   {0x53, 0x5e},		{0x54, 0x80},
   {0x58, 0x9e},

   // Gamma curve values
   {0x7a, 0x20},		{0x7b, 0x10},
   {0x7c, 0x1e},		{0x7d, 0x35},
   {0x7e, 0x5a},		{0x7f, 0x69},
   {0x80, 0x76},		{0x81, 0x80},
   {0x82, 0x88},		{0x83, 0x8f},
   {0x84, 0x96},		{0x85, 0xa3},
   {0x86, 0xaf},		{0x87, 0xc4},
   {0x88, 0xd7},		{0x89, 0xe8},

   // AGC and AEC parameters
   {0xa5, 0x05},		{0xab, 0x07},
   {0x24, 0x95},		{0x25, 0x33},
   {0x26, 0xe3},		{0x9f, 0x78},
   {0xa0, 0x68},		{0xa1, 0x03},
   {0xa6, 0xd8},		{0xa7, 0xd8},
   {0xa8, 0xf0},		{0xa9, 0x90},
   {0xaa, 0x94},		{0x10, 0x00},

   // AWB parameters
   {0x43, 0x0a},		{0x44, 0xf0},
   {0x45, 0x34},		{0x46, 0x58},
   {0x47, 0x28},		{0x48, 0x3a},
   {0x59, 0x88},		{0x5a, 0x88},
   {0x5b, 0x44},		{0x5c, 0x67},
   {0x5d, 0x49},		{0x5e, 0x0e},
   {0x6c, 0x0a},		{0x6d, 0x55},
   {0x6e, 0x11},		{0x6f, 0x9f},
   {0x6a, 0x40},		{0x01, 0x40},
   {0x02, 0x60},		{0x13, 0xe7},

   // Additional parameters
   {0x34, 0x11},		{0x3f, 0x00},
   {0x75, 0x05},		{0x76, 0xe1},
   {0x4c, 0x00},		{0x77, 0x01},
   {0xb8, 0x0a},		{0x41, 0x18},
   {0x3b, 0x12},		{0xa4, 0x88},
   {0x96, 0x00},		{0x97, 0x30},
   {0x98, 0x20},		{0x99, 0x30},
   {0x9a, 0x84},		{0x9b, 0x29},
   {0x9c, 0x03},		{0x9d, 0x4c},
   {0x9e, 0x3f},		{0x78, 0x04},
   {0x0e, 0x61},		{0x0f, 0x4b},
   {0x16, 0x02},		{0x1e, 0x00},
   {0x21, 0x02},		{0x22, 0x91},
   {0x29, 0x07},		{0x33, 0x0b},
   {0x35, 0x0b},		{0x37, 0x1d},
   {0x38, 0x71},		{0x39, 0x2a},
   {0x3c, 0x78},		{0x4d, 0x40},
   {0x4e, 0x20},		{0x69, 0x00},
   {0x6b, 0x3a},		{0x74, 0x10},
   {0x8d, 0x4f},		{0x8e, 0x00},
   {0x8f, 0x00},		{0x90, 0x00},
   {0x91, 0x00},		{0x96, 0x00},
   {0x9a, 0x00},		{0xb1, 0x0c},
   {0xb2, 0x0e},		{0xb3, 0x82},
   {0x4b, 0x01},
};

void OVDelay(volatile uint16_t nCount){
   while(nCount--){	
   }
}

void MCO1_init(void){
   GPIO_InitTypeDef GPIO_InitStructure;

   RCC_ClockSecuritySystemCmd(ENABLE);

   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

   // GPIO config
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;		//PA8 - XCLK
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
   GPIO_Init(GPIOA, &GPIO_InitStructure);

   // GPIO AF config
   GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_MCO);

   // MCO clock source
   RCC_MCO1Config(RCC_MCO1Source_PLLCLK, RCC_MCO1Div_5);
}

void SCCB_init(void){
   GPIO_InitTypeDef GPIO_InitStructure;
   I2C_InitTypeDef I2C_InitStructure;

   RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE); 
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 

   // GPIO config
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;		//PB8 - SIOC
   //PB9 - SIOD
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
   GPIO_Init(GPIOB, &GPIO_InitStructure);

   // GPIO AF config
   GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_I2C1);
   GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_I2C1);

   // I2C config
   I2C_DeInit(I2C1);	
   I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
   I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
   I2C_InitStructure.I2C_OwnAddress1 = 0x00;
   I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
   I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
   I2C_InitStructure.I2C_ClockSpeed = 100000;
   I2C_ITConfig(I2C1, I2C_IT_ERR, ENABLE);
   I2C_Init(I2C1,&I2C_InitStructure);
   I2C_Cmd(I2C1, ENABLE);
}

bool SCCB_write_reg(uint8_t reg_addr, uint8_t* data){
   uint32_t timeout = 0x7FFFFF;

   while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY)){
      if ((timeout--) == 0){
	 return true;
      }			
   }

   // Send start bit
   I2C_GenerateSTART(I2C1, ENABLE); 

   while( !I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT)){
      if ((timeout--) == 0){
	 return true;
      }
   }

   // Send slave address (camera write address)
   I2C_Send7bitAddress(I2C1, OV7670_WRITE_ADDR, I2C_Direction_Transmitter);

   while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){
      if ((timeout--) == 0){
	 return true;
      }
   }

   // Send register address
   I2C_SendData(I2C1, reg_addr);

   while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED)){
      if ((timeout--) == 0){
	 return true;
      }
   }

   // Send new register value
   I2C_SendData(I2C1, *data);  

   while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED)){
      if ((timeout--) == 0){
	 return true;
      }
   } 

   // Send stop bit
   I2C_GenerateSTOP(I2C1, ENABLE);
   return false;
}

bool OV7670_init(void){
   uint8_t data, i = 0;
   bool err = false;

   // Configure camera registers
   for(i=0; i<OV7670_REG_NUM ;i++){
      data = OV7670_reg[i][1]; 
      err = SCCB_write_reg(OV7670_reg[i][0], &data);

      if (err == true)
	 break;

      ++init_count;
      /*
      LCD_ILI9341_DrawLine(99+i, 110, 99+i, 130, ILI9341_COLOR_WHITE);*/
      OVDelay(0xFFFF);
   }

   return err;
}

bool OV7670_init3(void){
   uint8_t data, i = 0;
   bool err = false;

   // Configure camera registers
   for(i=0; i<OV7670_REG_NUM ;i++){
      data = OV7670_reg2[i][1]; 
      err = SCCB_write_reg(OV7670_reg2[i][0], &data);

      if (err == true)
	 break;
      /*
      LCD_ILI9341_DrawLine(99+i, 110, 99+i, 130, ILI9341_COLOR_WHITE);*/
      OVDelay(0xFFFF);
   }

   return err;
}

bool OV7670_init_v2(void)
{
   OVDelay(0xFFF);SCCB_write_reg(REG_COM7, 0x80);OVDelay(0xFFF);

        // QQVGA RGB444
        SCCB_write_reg(REG_CLKRC,0x80);OVDelay(0xFFF);
        SCCB_write_reg(REG_COM11,0x0A) ;OVDelay(0xFFF);
        SCCB_write_reg(REG_TSLB,0x04);OVDelay(0xFFF);
        SCCB_write_reg(REG_COM7,0x04) ;OVDelay(0xFFF);
        
        //SCCB_write_reg(REG_RGB444, 0x02);OVDelay(0xFFF);
        //SCCB_write_reg(REG_COM15, 0xd0);OVDelay(0xFFF);
        SCCB_write_reg(REG_RGB444, 0x00);OVDelay(0xFFF);     // Disable RGB 444?
        SCCB_write_reg(REG_COM15, 0xD0);OVDelay(0xFFF);      // Set RGB 565?
        
        SCCB_write_reg(REG_HSTART,0x16) ;OVDelay(0xFFF);
        SCCB_write_reg(REG_HSTOP,0x04) ;OVDelay(0xFFF);
        SCCB_write_reg(REG_HREF,0x24) ;OVDelay(0xFFF);
        SCCB_write_reg(REG_VSTART,0x02) ;OVDelay(0xFFF);
        SCCB_write_reg(REG_VSTOP,0x7a) ;OVDelay(0xFFF);
        SCCB_write_reg(REG_VREF,0x0a) ;OVDelay(0xFFF);
        SCCB_write_reg(REG_COM10,0x02) ;OVDelay(0xFFF);
        SCCB_write_reg(REG_COM3, 0x04);OVDelay(0xFFF);
        SCCB_write_reg(REG_COM14, 0x1a);OVDelay(0xFFF);
        SCCB_write_reg(REG_MVFP,0x27) ;OVDelay(0xFFF);
        SCCB_write_reg(0x72, 0x22);OVDelay(0xFFF);
        SCCB_write_reg(0x73, 0xf2);OVDelay(0xFFF);
 
        // COLOR SETTING
        SCCB_write_reg(0x4f,0x80);OVDelay(0xFFF);
        SCCB_write_reg(0x50,0x80);OVDelay(0xFFF);
        SCCB_write_reg(0x51,0x00);OVDelay(0xFFF);
        SCCB_write_reg(0x52,0x22);OVDelay(0xFFF);
        SCCB_write_reg(0x53,0x5e);OVDelay(0xFFF);
        SCCB_write_reg(0x54,0x80);OVDelay(0xFFF);
        SCCB_write_reg(0x56,0x40);OVDelay(0xFFF);
        SCCB_write_reg(0x58,0x9e);OVDelay(0xFFF);
        SCCB_write_reg(0x59,0x88);OVDelay(0xFFF);
        SCCB_write_reg(0x5a,0x88);OVDelay(0xFFF);
        SCCB_write_reg(0x5b,0x44);OVDelay(0xFFF);
        SCCB_write_reg(0x5c,0x67);OVDelay(0xFFF);
        SCCB_write_reg(0x5d,0x49);OVDelay(0xFFF);
        SCCB_write_reg(0x5e,0x0e);OVDelay(0xFFF);
        SCCB_write_reg(0x69,0x00);OVDelay(0xFFF);
        SCCB_write_reg(0x6a,0x40);OVDelay(0xFFF);
        SCCB_write_reg(0x6b,0x0a);OVDelay(0xFFF);
        SCCB_write_reg(0x6c,0x0a);OVDelay(0xFFF);
        SCCB_write_reg(0x6d,0x55);OVDelay(0xFFF);
        SCCB_write_reg(0x6e,0x11);OVDelay(0xFFF);
        SCCB_write_reg(0x6f,0x9f);OVDelay(0xFFF);
 
        SCCB_write_reg(0xb0,0x84);OVDelay(0xFFF);
	return true;
}

void OV7670_init_v3(void)
{
    OVDelay(0xFFFF);SCCB_write_reg(REG_COM7, 0x80); /* reset to default values */
    OVDelay(0xFFFF);SCCB_write_reg(REG_CLKRC, 0x80);
    OVDelay(0xFFFF);SCCB_write_reg(REG_COM11, 0x0A);
    OVDelay(0xFFFF);SCCB_write_reg(REG_TSLB, 0x04);
    OVDelay(0xFFFF);SCCB_write_reg(REG_TSLB, 0x04);
    OVDelay(0xFFFF);SCCB_write_reg(REG_COM7, 0x04); /* output format: rgb */

    OVDelay(0xFFFF);SCCB_write_reg(REG_RGB444, 0x00); /* disable RGB444 */
    OVDelay(0xFFFF);SCCB_write_reg(REG_COM15, 0xD0); /* set RGB565 */

    /* not even sure what all these do, gonna check the oscilloscope and go
     * from there... */
    OVDelay(0xFFFF);SCCB_write_reg(REG_HSTART, 0x16);
    OVDelay(0xFFFF);SCCB_write_reg(REG_HSTOP, 0x04);
    OVDelay(0xFFFF);SCCB_write_reg(REG_HREF, 0x24);
    OVDelay(0xFFFF);SCCB_write_reg(REG_VSTART, 0x02);
    OVDelay(0xFFFF);SCCB_write_reg(REG_VSTOP, 0x7a);
    OVDelay(0xFFFF);SCCB_write_reg(REG_VREF, 0x0a);
    OVDelay(0xFFFF);SCCB_write_reg(REG_COM10, 0x02);
    OVDelay(0xFFFF);SCCB_write_reg(REG_COM3, 0x04);
    OVDelay(0xFFFF);SCCB_write_reg(REG_MVFP, 0x3f);

    /* 160x120, i think */
    OVDelay(0xFFFF);SCCB_write_reg(REG_COM14, 0x1a); // divide by 4
    OVDelay(0xFFFF);SCCB_write_reg(0x72, 0x22); // downsample by 4
    OVDelay(0xFFFF);SCCB_write_reg(0x73, 0xf2); // divide by 4

    /* 320x240: */
   // OVDelay(0xFFFF);SCCB_write_reg(REG_COM14, 0x19);
   // OVDelay(0xFFFF);SCCB_write_reg(0x72, 0x11);
  //  OVDelay(0xFFFF);SCCB_write_reg(0x73, 0xf1);

    // test pattern
    //OVDelay(0xFFFF);SCCB_write_reg(0x70, 0xf0);
    //OVDelay(0xFFFF);SCCB_write_reg(0x71, 0xf0);

    // COLOR SETTING
    OVDelay(0xFFFF);SCCB_write_reg(0x4f, 0x80);
    OVDelay(0xFFFF);SCCB_write_reg(0x50, 0x80);
    OVDelay(0xFFFF);SCCB_write_reg(0x51, 0x00);
    OVDelay(0xFFFF);SCCB_write_reg(0x52, 0x22);
    OVDelay(0xFFFF);SCCB_write_reg(0x53, 0x5e);
    OVDelay(0xFFFF);SCCB_write_reg(0x54, 0x80);
    OVDelay(0xFFFF);SCCB_write_reg(0x56, 0x40);
    OVDelay(0xFFFF);SCCB_write_reg(0x58, 0x9e);
    OVDelay(0xFFFF);SCCB_write_reg(0x59, 0x88);
    OVDelay(0xFFFF);SCCB_write_reg(0x5a, 0x88);
    OVDelay(0xFFFF);SCCB_write_reg(0x5b, 0x44);
    OVDelay(0xFFFF);SCCB_write_reg(0x5c, 0x67);
    OVDelay(0xFFFF);SCCB_write_reg(0x5d, 0x49);
    OVDelay(0xFFFF);SCCB_write_reg(0x5e, 0x0e);
    OVDelay(0xFFFF);SCCB_write_reg(0x69, 0x00);
    OVDelay(0xFFFF);SCCB_write_reg(0x6a, 0x40);
    OVDelay(0xFFFF);SCCB_write_reg(0x6b, 0x0a);
    OVDelay(0xFFFF);SCCB_write_reg(0x6c, 0x0a);
    OVDelay(0xFFFF);SCCB_write_reg(0x6d, 0x55);
    OVDelay(0xFFFF);SCCB_write_reg(0x6e, 0x11);
    OVDelay(0xFFFF);SCCB_write_reg(0x6f, 0x9f);

    OVDelay(0xFFFF);SCCB_write_reg(0xb0, 0x84); 
}

void DCMI_DMA_init(){	
   GPIO_InitTypeDef GPIO_InitStructure;
   DCMI_InitTypeDef DCMI_InitStructure;
   DMA_InitTypeDef  DMA_InitStructure;
   NVIC_InitTypeDef NVIC_InitStructure;

   RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_DCMI, ENABLE);
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); 
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

   // GPIO config
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_6;		//PA4 - HREF
   //PA6 - PCLK
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
   GPIO_Init(GPIOA, &GPIO_InitStructure);

   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;		//PB6 - D5
   //PB7 - VSYNC
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
   GPIO_Init(GPIOB, &GPIO_InitStructure);

   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;/* | GPIO_Pin_8 | GPIO_Pin_9;*/		//PC6 - D0
   //PC7 - D1
   ////PC8 - D2
   ////PC9 - D3
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
   GPIO_Init(GPIOC, &GPIO_InitStructure);

   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
   //PE0 - D2
   //PE1 - D3
   //PE4 - D4
   //PE5 - D6
   //PE6 - D7
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
   GPIO_Init(GPIOE, &GPIO_InitStructure);

   // GPIO AF config
   GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_DCMI);
   GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_DCMI);
   GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_DCMI);
   GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_DCMI);
   GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_DCMI);
   GPIO_PinAFConfig(GPIOE, GPIO_PinSource0, GPIO_AF_DCMI);
   GPIO_PinAFConfig(GPIOE, GPIO_PinSource1, GPIO_AF_DCMI);
   GPIO_PinAFConfig(GPIOE, GPIO_PinSource4, GPIO_AF_DCMI);
   GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_DCMI);
   GPIO_PinAFConfig(GPIOE, GPIO_PinSource5, GPIO_AF_DCMI);
   GPIO_PinAFConfig(GPIOE, GPIO_PinSource6, GPIO_AF_DCMI);
/*
   // DCMI config
   DCMI_DeInit();
   DCMI_InitStructure.DCMI_CaptureMode = DCMI_CaptureMode_Continuous; 
   DCMI_InitStructure.DCMI_ExtendedDataMode = DCMI_ExtendedDataMode_8b; 
   DCMI_InitStructure.DCMI_CaptureRate = DCMI_CaptureRate_All_Frame;
   DCMI_InitStructure.DCMI_PCKPolarity = DCMI_PCKPolarity_Rising;
   DCMI_InitStructure.DCMI_HSPolarity = DCMI_HSPolarity_Low; 
   DCMI_InitStructure.DCMI_VSPolarity = DCMI_VSPolarity_High;	        
   DCMI_InitStructure.DCMI_SynchroMode = DCMI_SynchroMode_Hardware;                                                              
   DCMI_Init(&DCMI_InitStructure);
   DCMI_Cmd(ENABLE);

   // DMA config

   DMA_DeInit(DMA2_Stream1);  
   DMA_InitStructure.DMA_Channel = DMA_Channel_1;
   DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&DCMI->DR);
   DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)frame_buffer;
   DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
   DMA_InitStructure.DMA_BufferSize = IMG_ROWS*IMG_COLUMNS*2/4; 
   DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
   DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
   DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
   DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
   DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
   DMA_InitStructure.DMA_Priority = DMA_Priority_High;
   DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;
   DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
   DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
   DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
   DMA_Init(DMA2_Stream1, &DMA_InitStructure);
   DMA_Cmd(DMA2_Stream1, ENABLE);

   // DMA interrupt
   DMA_ITConfig(DMA2_Stream1, DMA_IT_TC, ENABLE);

   NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream1_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);	*/

   DCMI_DeInit();
   DCMI_InitStructure.DCMI_CaptureMode = DCMI_CaptureMode_Continuous; 
   DCMI_InitStructure.DCMI_ExtendedDataMode = DCMI_ExtendedDataMode_8b; 
   DCMI_InitStructure.DCMI_CaptureRate = DCMI_CaptureRate_All_Frame;
   DCMI_InitStructure.DCMI_PCKPolarity = DCMI_PCKPolarity_Rising;
   DCMI_InitStructure.DCMI_HSPolarity = DCMI_HSPolarity_Low; 
   DCMI_InitStructure.DCMI_VSPolarity = DCMI_VSPolarity_High;	        
   DCMI_InitStructure.DCMI_SynchroMode = DCMI_SynchroMode_Hardware;                                                              
   DCMI_Init(&DCMI_InitStructure);
   DCMI_Cmd(ENABLE);

   DMA_DeInit(DMA2_Stream1);  
   DMA_InitStructure.DMA_Channel = DMA_Channel_1;
   DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&DCMI->DR);
   DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)frame_buffer;
   DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
   DMA_InitStructure.DMA_BufferSize = (IMG_ROWS*IMG_COLUMNS)*2/4; 
   DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
   DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
   DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
   DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
   DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
   DMA_InitStructure.DMA_Priority = DMA_Priority_High;
   DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;
   DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
   DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
   DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
   DMA_Init(DMA2_Stream1, &DMA_InitStructure);
   DMA_Cmd(DMA2_Stream1, ENABLE);

   // DMA interrupt
   DMA_ITConfig(DMA2_Stream1, DMA_IT_TC, ENABLE);

   NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream1_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);	   
}
