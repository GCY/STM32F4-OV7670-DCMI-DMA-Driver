#include "stm32f4xx.h"
#include "system_control.h"
#include "OV7670_control.h"
#include "lcd_ili9341.h"
#include "lcd_fonts.h"
#include "lcd_spi.h"
#include <stdbool.h>

#include "./usb_cdc_device/usbd_usr.h"
#include "./usb_cdc_device/usbd_cdc_core.h"
#include "./usb_cdc_device/usb_conf.h"
#include "./usb_cdc_device/usbd_desc.h"
#include "./usb_cdc_device/usbd_cdc_vcp.h"

const uint32_t SECOND = 1000000;

static volatile uint8_t STM_mode = 0;
static volatile bool btn_pressed = false;
static volatile bool sett_mode = true;
static volatile bool frame_flag = false;

volatile uint32_t TimingDelay;

volatile uint32_t FPS_last_time = 0;

volatile uint32_t micros = 0;

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

uint32_t DCMI_count = 0;
uint32_t last_DCMI_count = 0;

int main(void){
   // System init
   SystemInit();

   if(SysTick_Config(SystemCoreClock / 1000 / 1000)){
      while(1){}
   }

   USBVCPInit();
   //USB_VCP_Puts("TonyGUO");
   STM_LedInit();
   STM_ButtonInit();
   STM_TimerInit();
   MCO1_init();
   SCCB_init();
   DCMI_DMA_init();
   LCD_ILI9341_Init();

   // RED LED = MODE 2
   STM_LedOn(LED_RED);

   // LCD init page
   LCD_ILI9341_Rotate(LCD_ILI9341_Orientation_Landscape_2);
   LCD_ILI9341_Fill(ILI9341_COLOR_BLACK);

   LCD_ILI9341_Puts(20, 55, "Configuring camera", &LCD_Font_16x26, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);
   LCD_ILI9341_DrawRectangle(99, 110, 221, 130, ILI9341_COLOR_WHITE);

   // OV7670 configuration
   bool error = OV7670_init();
   //bool error = OV7670_init2();

   if (error == true){
      LCD_ILI9341_Puts(100, 165, "Failed", &LCD_Font_16x26, ILI9341_COLOR_RED, ILI9341_COLOR_BLACK);
      LCD_ILI9341_Puts(20, 200, "Push reset button", &LCD_Font_16x26, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);
      while(1){
      }
   }		
   else{
      LCD_ILI9341_Puts(100, 165, "Success", &LCD_Font_16x26, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);
   }

   /*
   // LCD welcome page
   LCD_ILI9341_Fill(ILI9341_COLOR_BLACK);
   LCD_ILI9341_Puts(60, 110,  "TonyGUO", &LCD_Font_16x26, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLUE);
   */

   // Increse SPI baudrate
   LCD_SPI_BaudRateUp();



   // Infinite program loop
   while(1){
      if(USB_VCP_GetStatus() == USB_VCP_CONNECTED) {
	 uint8_t c;
	 if (USB_VCP_Getc(&c) == USB_VCP_DATA_OK) {
	    STM_LedOff(LED_RED);
	    /* Return data back */
	    USB_VCP_Putc(c);
	 }
      }

      if (btn_pressed == true){
	 if (STM_mode == 0){
	    // MODE 1 - RUN
	    STM_mode = 1;
	    STM_LedOff(LED_RED);
	    STM_LedOn(LED_GREEN);

	    DCMI_CaptureCmd(ENABLE);
	 }
	 else{
	    if(frame_flag == false){
	       // MODE 2 - STOP
	       STM_mode = 0;
	       STM_LedOff(LED_GREEN);
	       STM_LedOn(LED_RED);

	       DCMI_CaptureCmd(DISABLE);
	    }
	 }
	 btn_pressed = false;
      }

      if( frame_flag == true){			
	 // Show camera image
	 //LCD_ILI9341_Rotate(LCD_ILI9341_Orientation_Landscape_1);
	 LCD_ILI9341_DisplayImage((uint16_t*) frame_buffer);
	 char fps_str[20]  = "00 FPS\n";
	 fps_str[0] = (last_DCMI_count/10)+'0';
	 fps_str[1] = (last_DCMI_count%10)+'0';
	 LCD_ILI9341_Puts(10, 10, fps_str, &LCD_Font_7x10, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);	 

	 /*
	    for(int i = 0;i < IMG_ROWS*IMG_COLUMNS;++i){
	    USB_VCP_Putc((uint8_t)(frame_buffer[i] & 0x00FF));
	    USB_VCP_Putc((uint8_t)((frame_buffer[i] >> 8) & 0x00FF));
	    }*/

	 frame_flag = false;
      }
   }
}

void TIM3_IRQHandler(void){
   if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET){		
      static uint8_t old_state = 0xFF;
      uint8_t new_state = STM_ButtonPressed();

      TIM_ClearITPendingBit(TIM3, TIM_IT_Update);

      // Button state
      if (new_state > old_state){
	 if (STM_mode == 0){
	    TIM_Cmd(TIM4, ENABLE);
	 }
	 sett_mode = false;
      }
      if (sett_mode == false){
	 if (new_state < old_state){
	    btn_pressed = true;
	    if (STM_mode == 0)
	       TIM_Cmd(TIM4, DISABLE);
	 }
      }
      old_state = new_state;
   }
}

void TIM4_IRQHandler(void){	
   if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET){				
      static bool init = false; 

      TIM_ClearITPendingBit(TIM4, TIM_IT_Update);

      if (init == true){
	 // MODE 3 - SETTINGS
	 TIM_Cmd(TIM4, DISABLE);

	 sett_mode = true;
	 STM_mode = 3;
	 STM_LedOn(LED_GREEN);
	 STM_LedOn(LED_RED);
      }
      else{
	 init = true;
      }
   }
}

void DMA2_Stream1_IRQHandler(void){
   // DMA complete
   if(DMA_GetITStatus(DMA2_Stream1,DMA_IT_TCIF1) != RESET){
      DMA_ClearITPendingBit(DMA2_Stream1,DMA_IT_TCIF1);

      DMA_Cmd(DMA2_Stream1, ENABLE);
      frame_flag = true;
      if(micros - FPS_last_time > SECOND){
	 FPS_last_time = micros;
	 char fps_str[20]  = "00 FPS\n";
	 fps_str[0] = (DCMI_count/10)+'0';
	 fps_str[1] = (DCMI_count%10)+'0';
	 USB_VCP_Puts(fps_str);
	 //LCD_ILI9341_Puts(10, 10, fps_str, &LCD_Font_7x10, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);
	 last_DCMI_count = DCMI_count;
	 DCMI_count = 0;
      }
      else{
	 DCMI_count++;
      }
   }
}
