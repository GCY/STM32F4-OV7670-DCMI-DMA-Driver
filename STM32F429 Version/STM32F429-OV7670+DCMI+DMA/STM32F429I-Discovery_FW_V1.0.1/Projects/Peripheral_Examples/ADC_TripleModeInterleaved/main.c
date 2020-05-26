/**
  ******************************************************************************
  * @file    ADC_TripleModeInterleaved/main.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    11-November-2013
  * @brief   This example provides a short description of how to use the ADC 
  *          peripheral to convert a regular channel in Triple interleaved mode 
  *          using DMA in mode 2 with 8.4Msps .
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
#include "main.h"

/** @addtogroup STM32F429I_DISCOVERY_Examples
  * @{
  */

/** @addtogroup ADC_TripleModeInterleaved
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/ 
  #define MESSAGE1   "  Triple ADC Interleaved      "
  #define MESSAGE1_1 "  DMA mode2 example           " 
  #define MESSAGE2   " ADC Ch13 Conv " 
  #define MESSAGE2_1 "    8.4Msps      "
  #define MESSAGE3   "Connect voltage "
  #define MESSAGE4   "to ADC Ch13   "
  #define MESSAGE5   " ADC1 = %d,%d V"
  #define MESSAGE6   " ADC2 = %d,%d V"
  #define MESSAGE7   " ADC3 = %d,%d V"
  #define LINENUM            0x15
  #define FONTSIZE         Font8x12


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint32_t uhADCTripleConvertedValue[3];
  
/* Private function prototypes -----------------------------------------------*/
static void ADC_Config(void);

#ifdef USE_LCD
static void Display_Init(void);
static void Display(void);
#endif /* USE_LCD */

/* Private functions ---------------------------------------------------------*/

/**
  * @brief   Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       files (startup_stm32f429_439xx.s) before to branch to application main. 
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f4xx.c file
     */
#ifdef USE_LCD 
  /* LCD Display init  */
  Display_Init();
#endif /* USE_LCD */
  
  /* ADC configuration */
  ADC_Config();

  /* Start ADC1 Software Conversion */
  ADC_SoftwareStartConv(ADC1);
  
  while (1)
  {  
  /* Display ADCs converted values on LCD */
    #ifdef USE_LCD 
    Display();
    #endif /* USE_LCD */
  }
}

/**
  * @brief  ADC configuration
  * @note   This function Configure the ADC peripheral  
            1) Enable peripheral clocks
            2) Configure ADC Channel 13 pin as analog input
            3) DMA2_Stream0 channel0 configuration
            4) Configure ADC1 Channel 13
            5) Configure ADC2 Channel 13
            6) Configure ADC3 Channel 13
  * @param  None
  * @retval None
  */
static void ADC_Config(void)
{
  GPIO_InitTypeDef       GPIO_InitStructure;
  DMA_InitTypeDef        DMA_InitStructure;
  ADC_InitTypeDef        ADC_InitStructure;
  ADC_CommonInitTypeDef  ADC_CommonInitStructure;  
  
  /* Enable peripheral clocks *************************************************/
  RCC_AHB1PeriphClockCmd( ADC123_CHANNEL_GPIO_CLK , ENABLE);
  RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_DMA2 , ENABLE);
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_ADC1 , ENABLE);
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_ADC2 , ENABLE);
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_ADC3 , ENABLE);  

  /* Configure ADC Channel 12 pin as analog input *****************************/ 
  GPIO_InitStructure.GPIO_Pin = GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIO_PORT, &GPIO_InitStructure);

  /* DMA2 Stream0 channel0 configuration **************************************/
  DMA_InitStructure.DMA_Channel = DMA_CHANNELx;  
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC_CDR_ADDRESS;
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&uhADCTripleConvertedValue;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_BufferSize = 3;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA_STREAMx, &DMA_InitStructure);

  /* DMA2_Stream0 enable */
  DMA_Cmd(DMA_STREAMx, ENABLE);

  /* ADC Common configuration *************************************************/
  ADC_CommonInitStructure.ADC_Mode = ADC_TripleMode_Interl;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_2;  
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2; 
  ADC_CommonInit(&ADC_CommonInitStructure);

  /* DMA mode 2 is used in interleaved mode in 12-bit resolutions *************/
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  
  /* ADC1 regular channel 13 configuration ************************************/
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 1;
  ADC_Init(ADC1, &ADC_InitStructure);
  
  /* ADC1 regular channel 13 configuration */
  ADC_RegularChannelConfig(ADC1, ADC_CHANNEL, 1, ADC_SampleTime_3Cycles);
  
  /* Enable ADC1 DMA */
  ADC_DMACmd(ADC1, ENABLE);

  /* ADC2 regular channel 13 configuration ************************************/
  ADC_Init(ADC2, &ADC_InitStructure);
  /* ADC2 regular channel13 configuration */ 
  ADC_RegularChannelConfig(ADC2, ADC_CHANNEL, 1, ADC_SampleTime_3Cycles);

  /* ADC3 regular channel 13 configuration ************************************/
  ADC_Init(ADC3, &ADC_InitStructure); 
  /* ADC3 regular channel13 configuration */
  ADC_RegularChannelConfig(ADC3, ADC_CHANNEL, 1, ADC_SampleTime_3Cycles);

  /* Enable DMA request after last transfer (multi-ADC mode) ******************/
  ADC_MultiModeDMARequestAfterLastTransferCmd(ENABLE);

  /* Enable ADC1 **************************************************************/
  ADC_Cmd(ADC1, ENABLE);

  /* Enable ADC2 **************************************************************/
  ADC_Cmd(ADC2, ENABLE);

  /* Enable ADC3 **************************************************************/
  ADC_Cmd(ADC3, ENABLE);
}

#ifdef USE_LCD
/**
  * @brief  Display ADCs converted values on LCD
  * @param  None
  * @retval None
  */
static void Display(void)
{
  uint32_t uwVoltage =0, uwMVoltage=0;
  uint8_t aTextBuffer[50];
  __IO uint32_t uwADC1ConvertedVoltage;
  __IO uint32_t uwADC2ConvertedVoltage;
  __IO uint32_t uwADC3ConvertedVoltage;
  __IO uint16_t uwADC1ConvertedValue;
  __IO uint16_t uwADC2ConvertedValue;
  __IO uint16_t uwADC3ConvertedValue;
  
  uwADC1ConvertedValue = (uhADCTripleConvertedValue[0] & 0xFFF);
  uwADC1ConvertedVoltage = uwADC1ConvertedValue *3000/0xFFF;

  uwVoltage = (uwADC1ConvertedVoltage)/1000;
  uwMVoltage = (uwADC1ConvertedVoltage%1000)/100;

  sprintf((char*)aTextBuffer, MESSAGE5, uwVoltage, uwMVoltage);
  LCD_DisplayStringLine(LCD_LINE_6, (uint8_t*)aTextBuffer);


  uwADC2ConvertedValue = (uhADCTripleConvertedValue[2]& 0xFFF);
  uwADC2ConvertedVoltage = uwADC2ConvertedValue *3000/0xFFF;

  uwVoltage=uwADC2ConvertedVoltage/1000;
  uwMVoltage = (uwADC2ConvertedVoltage%1000)/100;

  sprintf((char*)aTextBuffer, MESSAGE6, uwVoltage, uwMVoltage);
  LCD_DisplayStringLine(LCD_LINE_7, (uint8_t*)aTextBuffer);
  
  uwADC3ConvertedValue = (uhADCTripleConvertedValue[1]& 0xFFF);
  uwADC3ConvertedVoltage = uwADC3ConvertedValue *3000/0xFFF;

  uwVoltage=uwADC3ConvertedVoltage/1000;
  uwMVoltage = (uwADC3ConvertedVoltage%1000)/100;

  sprintf((char*)aTextBuffer, MESSAGE7, uwVoltage, uwMVoltage);
  LCD_DisplayStringLine(LCD_LINE_8, (uint8_t*)aTextBuffer);
}

/**
  * @brief  Display Init (LCD)
  * @param  None
  * @retval None
  */
static void Display_Init(void)
{
  /* Initialize the LCD */
  LCD_Init();
  LCD_LayerInit();
  
  /* Eable the LTDC */
  LTDC_Cmd(ENABLE);
  
  /* Set LCD Background Layer  */
  LCD_SetLayer(LCD_BACKGROUND_LAYER);
  
  /* Clear the Background Layer */ 
  LCD_Clear(LCD_COLOR_WHITE);
  
  /* Configure the transparency for background */
  LCD_SetTransparency(0);
  
  /* Set LCD Foreground Layer  */
  LCD_SetLayer(LCD_FOREGROUND_LAYER);

  /* Configure the transparency for foreground */
  LCD_SetTransparency(200);
  
  /* Clear the Foreground Layer */ 
  LCD_Clear(LCD_COLOR_WHITE);
  
  /* Set the LCD Back Color and Text Color*/
  LCD_SetBackColor(LCD_COLOR_BLUE);
  LCD_SetTextColor(LCD_COLOR_WHITE);
  
    /* Set the LCD Text size */
  LCD_SetFont(&FONTSIZE);
  
  LCD_DisplayStringLine(LINE(LINENUM), (uint8_t*)MESSAGE1);
  LCD_DisplayStringLine(LINE(LINENUM + 1), (uint8_t*)MESSAGE1_1);
  LCD_DisplayStringLine(LINE(0x17), (uint8_t*)"                               ");
  
  /* Set the LCD Text size */
  LCD_SetFont(&Font16x24);
  
  LCD_DisplayStringLine(LCD_LINE_0, (uint8_t*)MESSAGE2);
  LCD_DisplayStringLine(LCD_LINE_1, (uint8_t*)MESSAGE2_1);
  
  /* Set the LCD Back Color and Text Color*/
  LCD_SetBackColor(LCD_COLOR_WHITE);
  LCD_SetTextColor(LCD_COLOR_BLUE); 
  
  LCD_DisplayStringLine(LCD_LINE_2, (uint8_t*)MESSAGE3);
  LCD_DisplayStringLine(LCD_LINE_3, (uint8_t*)MESSAGE4);
}
#endif /* USE_LCD */


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
