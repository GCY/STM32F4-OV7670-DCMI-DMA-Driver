/**
  ******************************************************************************
  * @file    ADC_DMA/main.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    11-November-2013
  * @brief   This example describes how to use the ADC and DMA to transfer 
  *          continuously converted data from ADC to memory.
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

/** @addtogroup ADC_DMA
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/ 
  #define MESSAGE1   "ADC conversion w/DMA"
  #define MESSAGE1_1 "continuouslyTransfer" 
  #define MESSAGE2   " ADC Ch13 Conv   "
  #define MESSAGE2_1 "    2.4Msps      "
  #define MESSAGE5   " ADC3 = %d,%1d V "
  #define LINENUM            0x15
  #define FONTSIZE         Font12x12
  #define ADC3_DR_ADDRESS     ((uint32_t)0x4001224C)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* You can monitor the converted value by adding the variable "uhADC3ConvertedValue" 
   to the debugger watch window */
__IO uint16_t uhADC3ConvertedValue = 0;
__IO uint32_t uwADC3ConvertedVoltage = 0;

/* Private function prototypes -----------------------------------------------*/

#ifdef USE_LCD
static void Display_Init(void);
static void Display(void);
#endif /* USE_LCD */
/* Private functions ---------------------------------------------------------*/
static void ADC3_CH13_DMA_Config(void);

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
  this is done through SystemInit() function which is called from startup
  file (startup_stm32f429_439xx.s) before to branch to application main.
  To reconfigure the default setting of SystemInit() function, refer to
  system_stm32f4xx.c file
  */
#ifdef USE_LCD 
  /* LCD Display init  */
  Display_Init();
#endif /* USE_LCD */
  
  /* ADC3 configuration *******************************************************/
  /*  - Enable peripheral clocks                                              */
  /*  - DMA2_Stream0 channel2 configuration                                   */
  /*  - Configure ADC Channel13 pin as analog input                           */
  /*  - Configure ADC3 Channel13                                              */
  ADC3_CH13_DMA_Config();
  
  /* Start ADC3 Software Conversion */ 
  ADC_SoftwareStartConv(ADC3);
  
  while (1)
  {
    /* convert the ADC value (from 0 to 0xFFF) to a voltage value (from 0V to 3.0V)*/
    uwADC3ConvertedVoltage = uhADC3ConvertedValue *3000/0xFFF;
    /* Display ADCs converted values on LCD */
#ifdef USE_LCD 
    Display();
#endif /* USE_LCD */
  }
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
  
  uwVoltage = (uwADC3ConvertedVoltage)/1000;
  uwMVoltage = (uwADC3ConvertedVoltage%1000)/100;

  sprintf((char*)aTextBuffer, MESSAGE5, uwVoltage, uwMVoltage);
  LCD_DisplayStringLine(LCD_LINE_6, (uint8_t*)aTextBuffer);
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
  /* Enable the LTDC */
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
  
  /* Set the LCD Back Color and Text Color*/
  LCD_SetBackColor(LCD_COLOR_BLUE);
  LCD_SetTextColor(LCD_COLOR_WHITE);
  
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
}
#endif /* USE_LCD */

/**
  * @brief  ADC3 channel13 with DMA configuration
  * @param  None
  * @retval None
  */
static void ADC3_CH13_DMA_Config(void)
{
  ADC_InitTypeDef       ADC_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
  DMA_InitTypeDef       DMA_InitStructure;
  GPIO_InitTypeDef      GPIO_InitStructure;

  /* Enable ADC3, DMA2 and GPIO clocks ****************************************/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2 | RCC_AHB1Periph_GPIOC, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);

  /* DMA2 Stream0 channel2 configuration **************************************/
  DMA_InitStructure.DMA_Channel = DMA_Channel_2;  
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC3_DR_ADDRESS;
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&uhADC3ConvertedValue;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_BufferSize = 1;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA2_Stream0, &DMA_InitStructure);
  DMA_Cmd(DMA2_Stream0, ENABLE);

  /* Configure ADC3 Channel13 pin as analog input ******************************/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  /* ADC Common Init **********************************************************/
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
  ADC_CommonInit(&ADC_CommonInitStructure);

  /* ADC3 Init ****************************************************************/
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;	
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 1;
  ADC_Init(ADC3, &ADC_InitStructure);

  /* ADC3 regular channel13 configuration *************************************/
  ADC_RegularChannelConfig(ADC3, ADC_Channel_13, 1, ADC_SampleTime_3Cycles);

 /* Enable DMA request after last transfer (Single-ADC mode) */
  ADC_DMARequestAfterLastTransferCmd(ADC3, ENABLE);

  /* Enable ADC3 DMA */
  ADC_DMACmd(ADC3, ENABLE);

  /* Enable ADC3 */
  ADC_Cmd(ADC3, ENABLE);
}

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
