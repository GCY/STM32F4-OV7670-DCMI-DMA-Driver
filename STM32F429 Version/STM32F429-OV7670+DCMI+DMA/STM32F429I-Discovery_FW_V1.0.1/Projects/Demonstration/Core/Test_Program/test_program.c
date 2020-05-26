/**
  ******************************************************************************
  * @file    test_program.c 
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    11-November-2013
  * @brief   Test program for STM32F429I-DISCO board
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "test_program.h"
#include "image.h"

/** @addtogroup STM32F429I-Discovery_Demo
* @{
*/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define ABS(x)         (x < 0) ? (-x) : x
#define L3G_Sensitivity_250dps     (float)   114.285f  /*!< gyroscope sensitivity with 250 dps full scale [LSB/dps]  */
#define L3G_Sensitivity_500dps     (float)   57.1429f  /*!< gyroscope sensitivity with 500 dps full scale [LSB/dps]  */
#define L3G_Sensitivity_2000dps    (float)   14.285f   /*!< gyroscope sensitivity with 2000 dps full scale [LSB/dps] */

/* Private variables ---------------------------------------------------------*/
#define IS42S16400J_SIZE_16             0x400000
static __IO uint32_t TimingDelay;
extern __IO uint32_t ButtonPressed;
__ALIGN_BEGIN USB_OTG_CORE_HANDLE  USB_OTG_dev __ALIGN_END;
float Buffer[6];
uint8_t Xval, Yval = 0x00;
L3GD20_InitTypeDef L3GD20_InitStructure;
L3GD20_FilterConfigTypeDef L3GD20_FilterStructure;

/* Private function prototypes -----------------------------------------------*/
static void SetSYSCLK_168(void);
static void SetSYSCLK_180(void);

/* Private functions ---------------------------------------------------------*/

/**
* @brief  Main program.
* @param  None
* @retval None
*/
void start_test(void)
{
  /* Configure system clock (SYSCLK) to 180 MHz, at this stage the USB is not used.
     Please note that the USB is not functional if the system clock is set to 180 MHz. */
  SetSYSCLK_180();

  /* Reconfigure the SysTick following SYSCLK frequency change */
  if (SysTick_Config(SystemCoreClock / 1000))
  { 
    /* Capture error */ 
    while (1);
  }
   
  /* Initialize LEDs, User Button and LCD on STM32F429I-Disco */
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);
  STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_EXTI); 
  
  /* Display message on LCD */ 
  LCD_Init();
  LCD_LayerInit();
  
  /* LTDC reload configuration */  
  LTDC_ReloadConfig(LTDC_IMReload);
  
  /* Enable the LTDC */
  LTDC_Cmd(ENABLE);
  
  /* Set LCD foreground layer */
  LCD_SetLayer(LCD_FOREGROUND_LAYER);
  LCD_SetTransparency(0);
  
  /* Set LCD background layer */
  LCD_SetLayer(LCD_BACKGROUND_LAYER);
  
  /* LCD display message */
  LCD_Clear(LCD_COLOR_BLUE);
  LCD_SetTextColor(LCD_COLOR_BLACK); 
  LCD_DisplayStringLine(LCD_LINE_1,(uint8_t*)"  TEST PROGRAM ");
  LCD_DisplayStringLine(LCD_LINE_3,(uint8_t*)"    V1.0.1     ");
  LCD_DisplayStringLine(LCD_LINE_5,(uint8_t*)"      PUSH     ");
  LCD_DisplayStringLine(LCD_LINE_7,(uint8_t*)"  USER BUTTON  ");
  LCD_DisplayStringLine(LCD_LINE_9,(uint8_t*)"    TO START   ");
  
  /* Wait for User button to be pressed */
  while (STM_EVAL_PBGetState(BUTTON_USER) != Bit_SET)
  {}
  /* Wait for User button is released */
  while (STM_EVAL_PBGetState(BUTTON_USER) != Bit_RESET)
  {}

/* SDRAM TEST *****************************************************************/  
  /* LCD display message */
  LCD_DisplayStringLine(LCD_LINE_1,(uint8_t*)"   SDRAM TEST  ");
  LCD_DisplayStringLine(LCD_LINE_3,(uint8_t*)"               ");  
  
  /* Wait for User button to be pressed */
  while (STM_EVAL_PBGetState(BUTTON_USER) != Bit_SET)
  {}
  /* Wait for User button is released */
  while (STM_EVAL_PBGetState(BUTTON_USER) != Bit_RESET)
  {}
  
  /* SDRAM Test */
  SDRAM_Test();


/* LCD TEST *******************************************************************/    
  /* LCD display message */
  LCD_Clear(LCD_COLOR_BLUE);
  LCD_SetTextColor(LCD_COLOR_BLACK); 
  LCD_DisplayStringLine(LCD_LINE_1,(uint8_t*)"    LCD TEST   ");
  LCD_DisplayStringLine(LCD_LINE_3,(uint8_t*)"      PUSH     ");
  LCD_DisplayStringLine(LCD_LINE_5,(uint8_t*)"  USER BUTTON  ");
  LCD_DisplayStringLine(LCD_LINE_7,(uint8_t*)"    TO START   ");
  
  /* Wait for User button to be pressed */
  while (STM_EVAL_PBGetState(BUTTON_USER) != Bit_SET)
  {}
  /* Wait for User button is released */
  while (STM_EVAL_PBGetState(BUTTON_USER) != Bit_RESET)
  {}
  
  /* LCD Test */
  LCD_Test();

/* TP TEST ********************************************************************/  
  /* LCD display message */
  LCD_Clear(LCD_COLOR_BLUE);
  LCD_SetTextColor(LCD_COLOR_BLACK); 
  LCD_DisplayStringLine(LCD_LINE_1,(uint8_t*)"     TP TEST   ");
  LCD_DisplayStringLine(LCD_LINE_3,(uint8_t*)"      PUSH     ");
  LCD_DisplayStringLine(LCD_LINE_5,(uint8_t*)"  USER BUTTON  ");
  LCD_DisplayStringLine(LCD_LINE_7,(uint8_t*)"    TO START   ");
  
  /* Wait for User button to be pressed */
  while (STM_EVAL_PBGetState(BUTTON_USER) != Bit_SET)
  { }
  /* Wait for User button is released */
  while (STM_EVAL_PBGetState(BUTTON_USER) != Bit_RESET)
  { }
  
  /* TS Test */
  TP_Test();

/* MEMS TEST ******************************************************************/  
  /* LCD display message */
  LCD_Clear(LCD_COLOR_BLUE);
  LCD_SetTextColor(LCD_COLOR_BLACK); 
  LCD_DisplayStringLine(LCD_LINE_1,(uint8_t*)"   MEMS TEST   ");
  LCD_DisplayStringLine(LCD_LINE_3,(uint8_t*)"     PUSH      ");
  LCD_DisplayStringLine(LCD_LINE_5,(uint8_t*)"  USER BUTTON  ");
  LCD_DisplayStringLine(LCD_LINE_7,(uint8_t*)"    TO START   ");
  
  /* Wait for User button to be pressed */
  while (STM_EVAL_PBGetState(BUTTON_USER) != Bit_SET)
  { }
  /* Wait for User button is released */
  while (STM_EVAL_PBGetState(BUTTON_USER) != Bit_RESET)
  { }
  
  /* Configure Mems L3GD20*/
  Demo_GyroConfig();
  
  /* Mems Test */
  MEMS_Test();  

/* USB TEST *******************************************************************/  
  /* LCD display message */
  LCD_Clear(LCD_COLOR_BLUE);
  LCD_SetTextColor(LCD_COLOR_BLACK); 
  LCD_DisplayStringLine(LCD_LINE_1,(uint8_t*)"    USB TEST   ");
  LCD_DisplayStringLine(LCD_LINE_3,(uint8_t*)"      PUSH     ");
  LCD_DisplayStringLine(LCD_LINE_5,(uint8_t*)"  USER BUTTON  ");
  LCD_DisplayStringLine(LCD_LINE_7,(uint8_t*)"    TO START   ");
  
  /* Wait for User button to be pressed */
  while (STM_EVAL_PBGetState(BUTTON_USER) != Bit_SET)
  {}
  /* Wait for User button is released */
  while (STM_EVAL_PBGetState(BUTTON_USER) != Bit_RESET)
  {}

  /* Configure system clock (SYSCLK) to 168 MHz to provide 48 MHz clock needed for
     the USB operation.
     Please note that the USB is not functional if the system clock is set to 180 MHz. */
  SetSYSCLK_168();

  /* Reconfigure the SysTick following SYSCLK frequency change */
  if (SysTick_Config(SystemCoreClock / 1000))
  { 
    /* Capture error */ 
    while (1);
  }  
  /* USB Test */
  USB_Test();  


/* End of test program: go to infinite loop ***********************************/
  LCD_Clear(LCD_COLOR_BLUE);
  LCD_SetTextColor(LCD_COLOR_BLACK); 
  LCD_DisplayStringLine(LCD_LINE_5,(uint8_t*)"  End Of Test  ");
  LCD_DisplayStringLine(LCD_LINE_7,(uint8_t*)"   Program     ");
 
  while (1)
  {   
  }
}

/**
* @brief   Detect motion direction to test MEMS.
* @param  None
* @retval None
*/
void MEMS_Test(void)
{  
  
  /* LCD display message */
  LCD_DisplayStringLine(LCD_LINE_3,(uint8_t*)"MOVE DISCOVERY ");
  LCD_DisplayStringLine(LCD_LINE_5,(uint8_t*)"If screen color");
  LCD_DisplayStringLine(LCD_LINE_6,(uint8_t*)"changes:Test OK");
  LCD_DisplayStringLine(LCD_LINE_7,(uint8_t*)"Else Test KO   ");

 
  /* Wait for User button to be pressed */
  while (STM_EVAL_PBGetState(BUTTON_USER) != Bit_SET)
  {
    /* Read Gyro Angular data */
    Demo_GyroReadAngRate(Buffer);
    
    /* Update autoreload and capture compare registers value*/
    Xval = ABS((int8_t)(Buffer[0]));
    Yval = ABS((int8_t)(Buffer[1]));   
    if ( Xval>Yval)
    {
      if ((int8_t)Buffer[0] > 10.0f)
      { 
        /* LED3 and LED4 On */
        STM_EVAL_LEDOn(LED3);
        STM_EVAL_LEDOn(LED4);
        
        /* Clear the LCD */ 
        LCD_Clear(LCD_COLOR_BLUE);
        LCD_DisplayStringLine(LCD_LINE_10,(uint8_t*)"   PUSH USER    ");
        LCD_DisplayStringLine(LCD_LINE_11,(uint8_t*)"  BUTTON TO GO  ");
        LCD_DisplayStringLine(LCD_LINE_12,(uint8_t*)"  TO NEXT TEST  ");
        Delay(200);
      }
      if ((int8_t)Buffer[0] < -10.0f)
      { 
        /* LED3 and LED4 Off */
        STM_EVAL_LEDOff(LED3);
        STM_EVAL_LEDOff(LED4);
        /* Clear the LCD */ 
        LCD_Clear(LCD_COLOR_WHITE); 
        LCD_DisplayStringLine(LCD_LINE_10,(uint8_t*)"   PUSH USER    ");
        LCD_DisplayStringLine(LCD_LINE_11,(uint8_t*)"  BUTTON TO GO  ");
        LCD_DisplayStringLine(LCD_LINE_12,(uint8_t*)"  TO NEXT TEST  ");
        Delay(200);
      }
    }
    else
    {
      if ((int8_t)Buffer[1] < -10.0f)
      {
        /* LED4 On */
        STM_EVAL_LEDOff(LED3);
        STM_EVAL_LEDOn(LED4);
        
        /* Clear the LCD */ 
        LCD_Clear(LCD_COLOR_RED);
        LCD_DisplayStringLine(LCD_LINE_10,(uint8_t*)"   PUSH USER    ");
        LCD_DisplayStringLine(LCD_LINE_11,(uint8_t*)"  BUTTON TO GO  ");
        LCD_DisplayStringLine(LCD_LINE_12,(uint8_t*)"  TO NEXT TEST  ");          
        Delay(200);
      }
      if ((int8_t)Buffer[1] > 10.0f)
      {
        /* LED3 On */
        STM_EVAL_LEDOff(LED4);
        STM_EVAL_LEDOn(LED3);
        
        /* Clear the LCD */ 
        LCD_Clear(LCD_COLOR_GREEN);
        LCD_DisplayStringLine(LCD_LINE_10,(uint8_t*)"   PUSH USER    ");
        LCD_DisplayStringLine(LCD_LINE_11,(uint8_t*)"  BUTTON TO GO  ");
        LCD_DisplayStringLine(LCD_LINE_12,(uint8_t*)"  TO NEXT TEST  ");        
        Delay(200);
      } 
    }
  }
  /* Wait for User button is released */
  while (STM_EVAL_PBGetState(BUTTON_USER) != Bit_RESET)
  {}
  /* LED3 and LED4 Off */
  STM_EVAL_LEDOff(LED3);
  STM_EVAL_LEDOff(LED4);
}

/**
* @brief  Detect screen touch in specific square.
* @param  None
* @retval None
*/
void TP_Test(void)
{
  static TP_STATE* TP_State; 
  
  /* Clear the LCD */ 
  LCD_Clear(LCD_COLOR_BLUE2);
  
  /* Configure the IO Expander */
  if (IOE_Config() == IOE_OK)
  {
    /* Update the structure with the current position */
    TP_State = IOE_TP_GetState();  
    
    while (!(TP_State->TouchDetected) )
    {
      LCD_SetTextColor(LCD_COLOR_RED); 
      LCD_DisplayStringLine(LCD_LINE_1,(uint8_t*)"  PLEASE TOUCH ");
      LCD_DisplayStringLine(LCD_LINE_5,(uint8_t*)"  THE SCREEN   ");
      LCD_DrawFullRect(80, 200, 80, 80);       
      TP_State = IOE_TP_GetState(); 
      TP_State->X = TP_State->X;
      TP_State->Y = TP_State->Y;
      if ((TP_State->TouchDetected) && (TP_State->Y < 280) && (TP_State->Y > 200) && (TP_State->X > 80) && (TP_State->X < 160))
      {
        /* LCD display message */ 
        LCD_Clear(LCD_COLOR_GREEN);
        LCD_SetTextColor(LCD_COLOR_BLACK); 
        LCD_DisplayStringLine(LCD_LINE_1,(uint8_t*)"   TP TEST OK  ");
        LCD_DisplayStringLine(LCD_LINE_3,(uint8_t*)"      PUSH     ");
        LCD_DisplayStringLine(LCD_LINE_5,(uint8_t*)"  USER BUTTON  ");
        /* Waiting User Button is pressed */
        while ((STM_EVAL_PBGetState(BUTTON_USER) != Bit_SET))
        {}      
        /* Wait for User button is released */
        while (STM_EVAL_PBGetState(BUTTON_USER) != Bit_RESET)
        { }
      }
      else
      {
        TP_State->TouchDetected = 0;
      }
    }
  }  
  else
  {
    /* LCD display message */
    LCD_Clear(LCD_COLOR_RED);
    LCD_SetTextColor(LCD_COLOR_BLACK); 
    LCD_DisplayStringLine(LCD_LINE_1,(uint8_t*)"   TS TEST     ");
    LCD_DisplayStringLine(LCD_LINE_3,(uint8_t*)"    FAILED     ");
    
    /* Waiting User Button is pressed */
    while ((STM_EVAL_PBGetState(BUTTON_USER) != Bit_SET))
    {}      
    /* Wait for User Button is released */
    while (STM_EVAL_PBGetState(BUTTON_USER) != Bit_RESET)
    { } 
  }
}

/**
* @brief  Display image on LCD.
* @param  None
* @retval None
*/
void LCD_Test(void)
{ 
  DMA2D_InitTypeDef              DMA2D_InitStruct;
  DMA2D_FG_InitTypeDef           DMA2D_FG_InitStruct;
  
  /* Transfer data from SDRAM to Graphics RAM using DMA2D */
  /* Memory to Memory Transfer */
  DMA2D_DeInit();
  DMA2D_InitStruct.DMA2D_Mode = DMA2D_M2M;       
  DMA2D_InitStruct.DMA2D_CMode = DMA2D_RGB565;      
  DMA2D_InitStruct.DMA2D_OutputGreen = 0x0;      
  DMA2D_InitStruct.DMA2D_OutputBlue = 0x00;     
  DMA2D_InitStruct.DMA2D_OutputRed = 0x00;                
  DMA2D_InitStruct.DMA2D_OutputAlpha = 0x0;                  
  DMA2D_InitStruct.DMA2D_OutputMemoryAdd = 0xD0000000;                
  DMA2D_InitStruct.DMA2D_OutputOffset = 0x0;                
  DMA2D_InitStruct.DMA2D_NumberOfLine = 320;            
  DMA2D_InitStruct.DMA2D_PixelPerLine = 240;
  DMA2D_Init(&DMA2D_InitStruct);   
  DMA2D_FG_InitStruct.DMA2D_FGMA = (uint32_t)&Image;
  DMA2D_FG_InitStruct.DMA2D_FGO = 0x00;
  DMA2D_FG_InitStruct.DMA2D_FGCM = CM_RGB565;
  DMA2D_FG_InitStruct.DMA2D_FGPFC_ALPHA_VALUE = 0x00;
  DMA2D_FG_InitStruct.DMA2D_FGC_BLUE = 0x00;
  DMA2D_FG_InitStruct.DMA2D_FGC_GREEN = 0x00;
  DMA2D_FG_InitStruct.DMA2D_FGC_RED = 0x00;
  DMA2D_FGConfig(&DMA2D_FG_InitStruct);
  
  /* Start Transfer */ 
  DMA2D_StartTransfer();
  
  /* LTDC configuration reload */  
  LTDC_ReloadConfig(LTDC_IMReload);
  
  /* Wait for TC Flag activation */
  while(DMA2D_GetFlagStatus(DMA2D_FLAG_TC) == RESET)
  {}

  /* Add some delay to have the image visible for 1.5s */
  Delay (1500);
  
  /* Clear the LCD */ 
  LCD_Clear(LCD_COLOR_GREEN);
  LCD_SetTextColor(LCD_COLOR_BLACK); 
  LCD_DisplayStringLine(LCD_LINE_1,(uint8_t*)"  LCD TEST OK  ");
  LCD_DisplayStringLine(LCD_LINE_3,(uint8_t*)"      PUSH     ");
  LCD_DisplayStringLine(LCD_LINE_5,(uint8_t*)"  USER BUTTON  ");
  /* Waiting User Button is pressed */
  while ((STM_EVAL_PBGetState(BUTTON_USER) != Bit_SET))
  {}
  /* Wait for User Button is released */
  while (STM_EVAL_PBGetState(BUTTON_USER) != Bit_RESET)
  {}
}

/**
* @brief  USB Test : LED5 ON  when USB OK and connected to PC.
* @param  None
* @retval None
*/
void USB_Test(void)
{     

  /* Init Device Library ********************************************************/
  USBD_Init(&USB_OTG_dev,
            USB_OTG_HS_CORE_ID,
            &USR_desc, 
            &USBD_HID_cb, 
            &USR_cb);   
  
  /* Connect USB to PC/Laptop, if LED5 is ON test OK, otherwise NotOK */
  /* LCD display message */
  LCD_Clear(LCD_COLOR_GREEN);
  LCD_SetTextColor(LCD_COLOR_BLACK); 
  LCD_DisplayStringLine(LCD_LINE_1,(uint8_t*)" CONNECT USB   ");
  LCD_DisplayStringLine(LCD_LINE_2,(uint8_t*)"USER CN6 TO PC ");
  LCD_DisplayStringLine(LCD_LINE_4,(uint8_t*)"If LED5 ON:    ");
  LCD_DisplayStringLine(LCD_LINE_5,(uint8_t*)"    TEST OK    ");
  LCD_DisplayStringLine(LCD_LINE_7,(uint8_t*)"If LED5 OFF:   ");
  LCD_DisplayStringLine(LCD_LINE_8,(uint8_t*)"    TEST KO    ");
  LCD_DisplayStringLine(LCD_LINE_10,(uint8_t*)"      PUSH     ");
  LCD_DisplayStringLine(LCD_LINE_11,(uint8_t*)"  USER BUTTON  ");
  
  /* Waiting User Button is pressed */
  while ((STM_EVAL_PBGetState(BUTTON_USER) != Bit_SET))
  {}      
  /* Wait for User Button is released */
  while (STM_EVAL_PBGetState(BUTTON_USER) != Bit_RESET)
  { }   
}

/**
* @brief  Erase, then write/read operation are performed to test SDRAM.
* @param  None
* @retval None
*/
void SDRAM_Test(void)
{
  uint16_t writedata_16b = 0x1E5A, readdata_16b = 0;  
  uint32_t readwritestatus = 0;
  uint32_t index = 0;
  
  /* SDRAM Initialization */  
  SDRAM_Init();
  
  /* FMC SDRAM GPIOs Configuration*/
  SDRAM_GPIOConfig(); 
  
  /* Disable write protection */
  FMC_SDRAMWriteProtectionConfig(FMC_Bank2_SDRAM,DISABLE); 
  
  /* Erase SDRAM memory */
  for (index = 0x00; index < IS42S16400J_SIZE_16; index++)
  {
    *(__IO uint16_t*) (SDRAM_BANK_ADDR + 2*index) = (uint16_t)0x00;
  }
  
  /* Write data value to all SDRAM memory */
  for (index = 0; index < IS42S16400J_SIZE_16; index++)
  {
    *(__IO uint16_t*) (SDRAM_BANK_ADDR + 2*index) = (uint16_t)(writedata_16b + index);
  }
  
  /* Read back SDRAM memory and check content correctness*/
  index = 0;
  readwritestatus = 0;
  while ((index < IS42S16400J_SIZE_16) && (readwritestatus == 0))
  {
    readdata_16b = *(__IO uint16_t*)(SDRAM_BANK_ADDR + 2*index);
    if ( readdata_16b != (uint16_t)(writedata_16b + index))
    {
      readwritestatus = 1;
      STM_EVAL_LEDOn(LED4);
      STM_EVAL_LEDOff(LED3); 
      /* LCD display ERROR message */
      LCD_Clear(LCD_COLOR_RED);
      LCD_SetTextColor(LCD_COLOR_BLACK); 
      LCD_DisplayStringLine(LCD_LINE_1,(uint8_t*)"   SDRAM TEST  ");
      LCD_DisplayStringLine(LCD_LINE_3,(uint8_t*)"    FAILED     ");
      /*infinite loop on memory error.*/
      while(1)
      {
        Delay(100);
        STM_EVAL_LEDToggle(LED4);
      }      
    }
    else
    {
      STM_EVAL_LEDOn(LED3);
    }
    index++;
  }
     /* LCD display OK message */ 
  LCD_Clear(LCD_COLOR_GREEN);
  LCD_SetTextColor(LCD_COLOR_BLACK); 
  LCD_DisplayStringLine(LCD_LINE_1,(uint8_t*)" SDRAM TEST OK ");
  LCD_DisplayStringLine(LCD_LINE_3,(uint8_t*)"      PUSH     ");
  LCD_DisplayStringLine(LCD_LINE_5,(uint8_t*)"  USER BUTTON  ");
   /* Waiting User Button is pressed */
  while ((STM_EVAL_PBGetState(BUTTON_USER) != Bit_SET))
  {}
  /* Wait for User button is released */
  while (STM_EVAL_PBGetState(BUTTON_USER) != Bit_RESET)
  { }
   
  /* Turn Off Leds */ 
  STM_EVAL_LEDOff(LED3);
  STM_EVAL_LEDOff(LED4);  
}

/**
* @brief  Configure the Mems to gyroscope application.
* @param  None
* @retval None
*/
void Demo_GyroConfig(void)
{
  /* Configure Mems L3GD20 */
  L3GD20_InitStructure.Power_Mode = L3GD20_MODE_ACTIVE;
  L3GD20_InitStructure.Output_DataRate = L3GD20_OUTPUT_DATARATE_1;
  L3GD20_InitStructure.Axes_Enable = L3GD20_AXES_ENABLE;
  L3GD20_InitStructure.Band_Width = L3GD20_BANDWIDTH_4;
  L3GD20_InitStructure.BlockData_Update = L3GD20_BlockDataUpdate_Continous;
  L3GD20_InitStructure.Endianness = L3GD20_BLE_LSB;
  L3GD20_InitStructure.Full_Scale = L3GD20_FULLSCALE_500; 
  L3GD20_Init(&L3GD20_InitStructure);
  L3GD20_FilterStructure.HighPassFilter_Mode_Selection =L3GD20_HPM_NORMAL_MODE_RES;
  L3GD20_FilterStructure.HighPassFilter_CutOff_Frequency = L3GD20_HPFCF_0;
  L3GD20_FilterConfig(&L3GD20_FilterStructure) ;
  L3GD20_FilterCmd(L3GD20_HIGHPASSFILTER_ENABLE);
}

/**
* @brief  Calculate the angular Data rate Gyroscope.
* @param  pfData : Data out pointer
* @retval None
*/
void Demo_GyroReadAngRate (float* pfData)
{
  uint8_t tmpbuffer[6] ={0};
  int16_t RawData[3] = {0};
  uint8_t tmpreg = 0;
  float sensitivity = 0;
  int i =0;
  
  L3GD20_Read(&tmpreg,L3GD20_CTRL_REG4_ADDR,1);
  L3GD20_Read(tmpbuffer,L3GD20_OUT_X_L_ADDR,6);
  
  /* check in the control register 4 the data alignment (Big Endian or Little Endian)*/
  if(!(tmpreg & 0x40))
  {
    for(i=0; i<3; i++)
    {
      RawData[i]=(int16_t)(((uint16_t)tmpbuffer[2*i+1] << 8) + tmpbuffer[2*i]);
    }
  }
  else
  {
    for(i=0; i<3; i++)
    {
      RawData[i]=(int16_t)(((uint16_t)tmpbuffer[2*i] << 8) + tmpbuffer[2*i+1]);
    }
  }
  
  /* Switch the sensitivity value set in the CRTL4 */
  switch(tmpreg & 0x30)
  {
  case 0x00:
    sensitivity=L3G_Sensitivity_250dps;
    break;
    
  case 0x10:
    sensitivity=L3G_Sensitivity_500dps;
    break;
    
  case 0x20:
    sensitivity=L3G_Sensitivity_2000dps;
    break;
  }
  /* divide by sensitivity */
  for(i=0; i<3; i++)
  {
    pfData[i]=(float)RawData[i]/sensitivity;
  }
}

/**
* @brief  Basic management of the timeout situation.
* @param  None.
* @retval None.
*/
uint32_t L3GD20_TIMEOUT_UserCallback(void)
{
  return 0;
}

/**
* @brief  Inserts a delay time.
* @param  nTime: specifies the delay time length, in 10 ms.
* @retval None
*/
void Delay(__IO uint32_t nTime)
{
  TimingDelay = nTime; 
  while(TimingDelay != 0);
}

/**
* @brief  Decrements the TimingDelay variable.
* @param  None
* @retval None
*/
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}


/**
* @brief  Configures System clock to 180 MHz.
* @param  None
* @retval None
*/
static void SetSYSCLK_180(void)
{
  /* Select HSE as system clock source */
  RCC_SYSCLKConfig(RCC_SYSCLKSource_HSE);
  
  /* Wait till HSE is used as system clock source */
  while (RCC_GetSYSCLKSource() != 0x04)
  {}
  
  /* Disable PLL */
  RCC_PLLCmd(DISABLE);  
  
  /* Configure PLL clock to have:
  PLL_VCO = (HSE_VALUE / PLL_M) * PLL_N = 360 MHz
  SYSCLK = PLL_VCO / PLL_P = 180 MHz
  USB OTG FS, SDIO and RNG Clock =  PLL_VCO / PLLQ = ~51 MHz (USB is not used)
  */
  RCC_PLLConfig(RCC_PLLSource_HSE, 
                8, //PLLM
                360, //PLLN
                2, //PLLP
                7 //PLLQ
                  );
  
  /* Enable PLL */
  RCC_PLLCmd(ENABLE);
  
  /* Wait till PLL is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
  {}
  
  /* Enable the Over-drive to extend the clock frequency to 180 Mhz */
  PWR_OverDriveCmd(ENABLE);
    
  /* Wait till Over-drive is ready */
  while (PWR_GetFlagStatus(PWR_FLAG_ODRDY) == RESET)
  {}
  /* Enable the Over-drive mode switching to extend the clock frequency to 180 Mhz */
  PWR_OverDriveSWCmd(ENABLE);
    
  /* Wait till Over-drive mode switching is ready */
  while (PWR_GetFlagStatus(PWR_FLAG_ODSWRDY) == RESET)
  {}
  
  /* Select PLL as system clock source */
  RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
  
  /* Wait till PLL is used as system clock source */
  while (RCC_GetSYSCLKSource() != 0x08)
  {}
  
  /* Update SystemCoreClock global variable value */
  SystemCoreClock = 180000000;
}


/**
* @brief  Configures System clock to 168 MHz, to have USB clocked at 48 MHz.
* @param  None
* @retval None
*/
static void SetSYSCLK_168(void)
{  
  /* Select HSE as system clock source */
  RCC_SYSCLKConfig(RCC_SYSCLKSource_HSE);
  
  /* Wait till HSE is used as system clock source */
  while (RCC_GetSYSCLKSource() != 0x04)
  {}
  
  /* Disable PLL */
  RCC_PLLCmd(DISABLE);  

  /* Disable Over-drive mode */
  PWR_OverDriveCmd(DISABLE);    
  PWR_OverDriveSWCmd(DISABLE);
    
  /* Wait till ODSWRDY is reset */
  while (PWR_GetFlagStatus(PWR_FLAG_ODSWRDY) != RESET)
  {}
    
  /* Configure PLL clock to have:
  PLL_VCO = (HSE_VALUE / PLL_M) * PLL_N = 336 MHz
  SYSCLK = PLL_VCO / PLL_P = 168 MHz
  USB OTG FS, SDIO and RNG Clock =  PLL_VCO / PLLQ = 48 MHz
  */
  RCC_PLLConfig(RCC_PLLSource_HSE, 
                8, //PLLM
                336, //PLLN
                2, //PLLP
                7 //PLLQ
                  );
  
  /* Enable PLL */
  RCC_PLLCmd(ENABLE);  
  
  /* Wait till PLL is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
  {}
  
  /* Select PLL as system clock source */
  RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
  
  /* Wait till PLL is used as system clock source */
  while (RCC_GetSYSCLKSource() != 0x08)
  {} 
  
  /* Update SystemCoreClock global variable value */
  SystemCoreClock = 168000000;
}

/**
* @}
*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
