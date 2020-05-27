#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <stm32f4xx.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_usart.h>
#include <stm32f4xx_exti.h>
#include <stm32f4xx_i2c.h>
#include <stm32f4xx_adc.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_dma.h>
#include <stm32f4xx_rtc.h>

#include "OV7670_control.h"

#include "./FATFS/ff.h"

#include "./usb_cdc_device/usbd_usr.h"
#include "./usb_cdc_device/usbd_cdc_core.h"
#include "./usb_cdc_device/usb_conf.h"
#include "./usb_cdc_device/usbd_desc.h"
#include "./usb_cdc_device/usbd_cdc_vcp.h"

#include "tiny_printf.h"

#include "tm_stm32f4_ili9341.h"
#include "tm_stm32f4_fonts.h"

#define __FPU_PRESENT
#define __FPU_USED

volatile uint32_t TimingDelay;

volatile uint32_t micros = 0;

volatile uint32_t line_count = 0;
volatile uint32_t last_capture_time = 0;

void Delay(__IO uint32_t nTime)
{
   TimingDelay = nTime;
   while(TimingDelay){
   }
}

void SysTick_Handler(void)
{
   if(TimingDelay){
      --TimingDelay;
   }
   ++micros;
}

void Init_LED()
{
   //Enable the GPIOD Clock
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);


   // GPIOD Configuration
   GPIO_InitTypeDef GPIO_InitStruct;
   GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
   GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
   GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
   GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;

   GPIO_SetBits(GPIOE, GPIO_Pin_2);
   GPIO_SetBits(GPIOE, GPIO_Pin_3);
   GPIO_Init(GPIOE, &GPIO_InitStruct);   
}

void Init_LCD()
{
   //Enable the GPIOD Clock
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);


   // GPIOD Configuration
   GPIO_InitTypeDef GPIO_InitStruct;
   GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14;
   GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
   GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
   GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;

   GPIO_SetBits(GPIOD, GPIO_Pin_14);
   GPIO_Init(GPIOD, &GPIO_InitStruct);   

}

void EXTILine2_Config(void)
{
   EXTI_InitTypeDef   EXTI_InitStructure;
   GPIO_InitTypeDef   GPIO_InitStructure;
   NVIC_InitTypeDef   NVIC_InitStructure;

   /* Enable GPIOB clock */
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
   /* Enable SYSCFG clock */
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

   /* Configure PB0 pin as input floating */
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
   GPIO_Init(GPIOA, &GPIO_InitStructure);

   /* Connect EXTI Line0 to PB0 pin */
   SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource1);

   /* Configure EXTI Line0 */
   EXTI_InitStructure.EXTI_Line = EXTI_Line2;
   EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
   EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;  
   EXTI_InitStructure.EXTI_LineCmd = ENABLE;
   EXTI_Init(&EXTI_InitStructure);

   /* Enable and set EXTI Line0 Interrupt to the lowest priority */
   NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);
}

/* Private */
#define USB_VCP_RECEIVE_BUFFER_LENGTH		128
uint8_t INT_USB_VCP_ReceiveBuffer[USB_VCP_RECEIVE_BUFFER_LENGTH];
uint32_t int_usb_vcp_buf_in, int_usb_vcp_buf_out, int_usb_vcp_buf_num;
extern USB_VCP_Result USB_VCP_INT_Status;
//extern LINE_CODING linecoding;
uint8_t USB_VCP_INT_Init = 0;
USB_OTG_CORE_HANDLE	USB_OTG_dev;

extern uint8_t INT_USB_VCP_ReceiveBuffer[USB_VCP_RECEIVE_BUFFER_LENGTH];

USB_VCP_Result USBVCPInit(void)
{
   USBD_Init(&USB_OTG_dev,
#ifdef USE_USB_OTG_FS
	 USB_OTG_FS_CORE_ID,
#else
	 USB_OTG_HS_CORE_ID,
#endif
	 &USR_desc, 
	 &USBD_CDC_cb, 
	 &USR_cb);   

   /* Reset buffer counters */
   int_usb_vcp_buf_in = 0;
   int_usb_vcp_buf_out = 0;
   int_usb_vcp_buf_num = 0;

   /* Initialized */
   USB_VCP_INT_Init = 1;

   return USB_VCP_OK;
}

USB_VCP_Result USB_VCP_GetStatus(void) {
   if (USB_VCP_INT_Init) {
      return USB_VCP_INT_Status;
   }
   return USB_VCP_ERROR;
}

USB_VCP_Result USB_VCP_Getc(uint8_t* c) {
   /* Any data in buffer */
   if (int_usb_vcp_buf_num > 0) {
      /* Check overflow */
      if (int_usb_vcp_buf_out >= USB_VCP_RECEIVE_BUFFER_LENGTH) {
	 int_usb_vcp_buf_out = 0;
      }
      *c = INT_USB_VCP_ReceiveBuffer[int_usb_vcp_buf_out];
      INT_USB_VCP_ReceiveBuffer[int_usb_vcp_buf_out] = 0;

      /* Set counters */
      int_usb_vcp_buf_out++;
      int_usb_vcp_buf_num--;

      /* Data OK */
      return USB_VCP_DATA_OK;
   }
   *c = 0;
   /* Data not ready */
   return USB_VCP_DATA_EMPTY;
}

USB_VCP_Result USB_VCP_Putc(volatile char c) {
   uint8_t ce = (uint8_t)c;

   /* Send data over USB */
   VCP_DataTx(&ce, 1);

   /* Return OK */
   return USB_VCP_OK;
}

USB_VCP_Result USB_VCP_Puts(char* str) {
   while (*str) {
      USB_VCP_Putc(*str++);
   }

   /* Return OK */
   return USB_VCP_OK;
}

USB_VCP_Result INT_USB_VCP_AddReceived(uint8_t c) {
   /* Still available data in buffer */
   if (int_usb_vcp_buf_num < USB_VCP_RECEIVE_BUFFER_LENGTH) {
      /* Check for overflow */
      if (int_usb_vcp_buf_in >= USB_VCP_RECEIVE_BUFFER_LENGTH) {
	 int_usb_vcp_buf_in = 0;
      }
      /* Add character to buffer */
      INT_USB_VCP_ReceiveBuffer[int_usb_vcp_buf_in] = c;
      /* Increase counters */
      int_usb_vcp_buf_in++;
      int_usb_vcp_buf_num++;

      /* Return OK */
      return USB_VCP_OK;
   }

   /* Return Buffer full */
   return USB_VCP_RECEIVE_BUFFER_FULL;
}

FATFS fs;
FIL fnew;	
FRESULT res_sd; 
UINT fnum;
//BYTE ReadBuffer[1024]={0};
//BYTE WriteBuffer[] = {"4095,4095\r\n"};

uint8_t init_count = 0;

uint32_t dcmi_count = 0;

int main(void)
{
   SystemInit();

   if(SysTick_Config(SystemCoreClock / 1000 / 1000)){
      while(1){}
   }

   Init_LED();
   EXTILine2_Config();

   USBVCPInit();
   Delay(13000);

   Init_LCD();

   //Initialize ILI9341
   TM_ILI9341_Init();
   //Rotate LCD for 90 degrees
   TM_ILI9341_Rotate(TM_ILI9341_Orientation_Landscape_2);
   //FIll lcd with color
   TM_ILI9341_Fill(ILI9341_COLOR_MAGENTA);
   //Draw white circle
   TM_ILI9341_DrawCircle(60, 60, 40, ILI9341_COLOR_GREEN);
   //Draw red filled circle
   TM_ILI9341_DrawFilledCircle(60, 60, 35, ILI9341_COLOR_RED);
   //Draw blue rectangle
   TM_ILI9341_DrawRectangle(120, 20, 220, 100, ILI9341_COLOR_BLUE);
   //Draw black filled rectangle
   TM_ILI9341_DrawFilledRectangle(130, 30, 210, 90, ILI9341_COLOR_BLACK);
   //Draw line with custom color 0x0005
   TM_ILI9341_DrawLine(10, 120, 310, 120, 0x0005);

   //Put string with black foreground color and blue background with 11x18px font
   TM_ILI9341_Puts(65, 130, "Humanoid Robot", &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_BLUE2);
   //Put string with black foreground color and blue background with 11x18px font
   TM_ILI9341_Puts(60, 150, "Vision Module", &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_BLUE2);
   //Put string with black foreground color and red background with 11x18px font
   TM_ILI9341_Puts(245, 225, "TonyGuo", &TM_Font_7x10, ILI9341_COLOR_BLACK, ILI9341_COLOR_ORANGE);

   Delay(13000);

   MCO1_init();
   SCCB_init();
   DCMI_DMA_init(); 

   DCMI_CaptureCmd(DISABLE);

   bool error = OV7670_init();  
   //bool error = OV7670_init_v2();
   //bool error = OV7670_init3();
   //bool error = OV7670_init_v3();

   char str_error[20];
   sprintf(str_error,"%d\n%d\n",error,init_count);
   USB_VCP_Puts(str_error);   
   if(init_count == OV7670_REG_NUM){
      GPIO_ResetBits(GPIOE, GPIO_Pin_2);
   }

   res_sd = f_mount(&fs,"0:",1);
   if(res_sd == FR_NO_FILESYSTEM){
      res_sd=f_mkfs("0:",0,0);							

      if(res_sd == FR_OK){
	 res_sd = f_mount(NULL,"0:",1);					
	 res_sd = f_mount(&fs,"0:",1);
      }
      else{
      }
   }

   uint32_t index = 0;;
   res_sd = f_open(&fnew, "0:FatFs data.csv",FA_CREATE_ALWAYS | FA_WRITE );
   char str_red[20] = {0};
   sprintf(str_red,"%d",res_sd);
   USB_VCP_Puts(str_red);	    

   BYTE WriteBuffer[30] = {0};
   sprintf(WriteBuffer,"%d,%d,%d\r\n",micros,12345,54321);
   res_sd=f_write(&fnew,WriteBuffer,strlen(WriteBuffer),&fnum);

   f_close(&fnew);

   GPIO_ResetBits(GPIOE, GPIO_Pin_3);

   int id_state = 0;
   int recheck_state = 0;
   int sn = 1;  

   while(1){

      if(USB_VCP_GetStatus() == USB_VCP_CONNECTED) {
	 if(sn == 1){
	    uint8_t c;
	    if (USB_VCP_Getc(&c) == USB_VCP_DATA_OK) {
	       /* Return data back */
	       //TM_USB_VCP_Putc(c);
	       if(c == 'A' && id_state == 0){
		  ++id_state;
	       }
	       if(c == 'S' && id_state == 1){
		  ++id_state;
	       }
	       if(c == 'K' && id_state == 2){
		  ++id_state;
		  DCMI_CaptureCmd(DISABLE);
		  USB_VCP_Puts("ASK1");
		  sn = 0;
	       }
	    }
	 }
	 else{
	    uint8_t c;
	    if (USB_VCP_Getc(&c) == USB_VCP_DATA_OK) {
	       /* Return data back */
	       //TM_USB_VCP_Putc(c);
	       if(c == 'C' && recheck_state == 0){
		  ++recheck_state;
	       }
	       if(c == 'H' && recheck_state == 1){
		  ++recheck_state;
	       }
	       if(c == 'K' && recheck_state == 2){
		  ++recheck_state;
		  //DCMI_CaptureCmd(ENABLE);
		  USB_VCP_Puts("CHK2");
	       }
	    }
	 }

	 if(recheck_state == 3){
	    //USB_VCP_Puts("");

	 }
      }

   }

   return(0); // System will implode
}   
/*
   uint32_t reverse_bits_32(uint32_t n) {
   n = ((n & 0xffff0000) >> 16) | ((n & 0x0000ffff) << 16);
   n = ((n & 0xff00ff00) >>  8) | ((n & 0x00ff00ff) <<  8);
   n = ((n & 0xf0f0f0f0) >>  4) | ((n & 0x0f0f0f0f) <<  4);
   n = ((n & 0xcccccccc) >>  2) | ((n & 0x33333333) <<  2);
   n = ((n & 0xaaaaaaaa) >>  1) | ((n & 0x55555555) <<  1);
   return n;
   }

   uint16_t reverse_bits_16(uint16_t x) {
   return (uint16_t) (reverse_bits_32(x) >> 16);
   }
   */
void DMA2_Stream1_IRQHandler(void){
   // DMA complete
   if(DMA_GetITStatus(DMA2_Stream1,DMA_IT_TCIF1) != RESET){
      DMA_ClearITPendingBit(DMA2_Stream1,DMA_IT_TCIF1);

      DMA_Cmd(DMA2_Stream1, ENABLE);

      /*
	 char hex_str[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
	 for(int i = 0;i < IMG_ROWS;++i){
	 USB_VCP_Putc(hex_str[(frame_buffer[i] >> 12) & 0x000F]);
	 USB_VCP_Putc(hex_str[(frame_buffer[i] >> 8) & 0x000F]);
	 USB_VCP_Putc(hex_str[(frame_buffer[i] >> 4) & 0x000F]);	 
	 USB_VCP_Putc(hex_str[frame_buffer[i] & 0x000F]);
	 USB_VCP_Puts(" ");
	 }
	 USB_VCP_Puts("\n");
	 */

      /*
	 char str[20] = {0};
	 sprintf(str,"%d\n",line_count);
	 USB_VCP_Puts(str);
	 */

      //if(line_count == 0){
      TM_ILI9341_SetCursorPosition(0, 0, IMG_ROWS - 1,  IMG_COLUMNS - 1);
      //TM_ILI9341_SetCursorPosition(0, line_count*IMG_COLUMNS, IMG_ROWS-1, ((line_count+1) * IMG_COLUMNS) - 1);
      TM_ILI9341_SendCommand(ILI9341_GRAM);      
      ILI9341_WRX_SET;
      ILI9341_CS_RESET;      
      uint32_t j,k;
      for(int i = 0;i < (IMG_ROWS*IMG_COLUMNS);++i){
	 //uint16_t rb = reverse_bits_16(frame_buffer[i]);
	 k = frame_buffer[i] >> 8;
	 j = frame_buffer[i] & 0xFF;

	 TM_SPI_Send16(ILI9341_SPI, k);
	 TM_SPI_Send16(ILI9341_SPI, j);
      }
      ILI9341_CS_SET;   
      /*++line_count;
	}
	else{
	line_count = 0;
	}*/

      //DCMI_CaptureCmd(DISABLE);
      /* 
	 ++line_count;
	 if(line_count > 1){
	 line_count = 0;
	 DCMI_CaptureCmd(DISABLE);
	 } */ 
   }
}

void EXTI2_IRQHandler(void)
{
   if(micros - last_capture_time > 300000){
      last_capture_time = micros;
      DCMI_CaptureCmd(ENABLE);
   }
   EXTI_ClearFlag(EXTI_Line2);
}
