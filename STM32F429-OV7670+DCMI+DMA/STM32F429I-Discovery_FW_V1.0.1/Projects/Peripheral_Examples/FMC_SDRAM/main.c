/**
  ******************************************************************************
  * @file    FMC_SDRAM/main.c 
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    11-November-2013
  * @brief   This example shows how to write to the external SDRAM with 
  *          8bits AHB transaction or 16bits AHB transaction.
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
#include "stm32f4xx.h"
#include "main.h"

/** @addtogroup STM32F429I_DISCOVERY_Examples
  * @{
  */

/** @addtogroup FMC_SDRAM
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#define IS42S16400J_SIZE             0x400000

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{ 
  uint8_t ubWritedata_8b = 0x3C, ubReaddata_8b = 0;  
  uint16_t uhWritedata_16b = 0x1E5A, uhReaddata_16b = 0;  
  uint32_t uwReadwritestatus = 0;
  uint32_t counter = 0x0;
  
  /*!< At this stage the microcontroller clock setting is already configured, 
  this is done through SystemInit() function which is called from startup
  file (startup_stm32f429_439xx.s) before to branch to application main.
  To reconfigure the default setting of SystemInit() function, refer to
  system_stm32f4xx.c file
  */
  
  /* Initialize LEDs and user-push button mounted on STM32F429I-DISCOVERY */
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);
  STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_GPIO); 
  
  /* Initialize the LCD */
  LCD_Init();
  LCD_LayerInit();
  LTDC_Cmd(ENABLE);
  LCD_SetLayer(LCD_FOREGROUND_LAYER);
  LCD_Clear(LCD_COLOR_WHITE);
  LCD_SetTransparency(200);
  LTDC_ReloadConfig(LTDC_IMReload);
  
  /* SDRAM Initialization */  
  SDRAM_Init();
  
  /* FMC SDRAM GPIOs Configuration */
  SDRAM_GPIOConfig();
  
  /* Disable write protection */
  FMC_SDRAMWriteProtectionConfig(FMC_Bank2_SDRAM,DISABLE); 

  while (1)
  {
    
  /*********************** 8-bits AHB transaction test ************************/    
  /* Wait for User button to be pressed */
  while (STM_EVAL_PBGetState(BUTTON_USER) != Bit_SET)
  {}
  /* Wait for User button is released */
  while (STM_EVAL_PBGetState(BUTTON_USER) != Bit_RESET)
  {}
  
  /* Turn Off Leds */   
  STM_EVAL_LEDOff(LED3);
  STM_EVAL_LEDOff(LED4);
  
  /* Erase SDRAM memory */
  for (counter = 0x00; counter < IS42S16400J_SIZE; counter++)
  {
    *(__IO uint8_t*) (SDRAM_BANK_ADDR + counter) = (uint8_t)0x0;
  }
  
  /* Write data value to all SDRAM memory */
  for (counter = 0; counter < IS42S16400J_SIZE; counter++)
  {
    *(__IO uint8_t*) (SDRAM_BANK_ADDR + counter) = (uint8_t)(ubWritedata_8b + counter);
  }
  
  /* Read back SDRAM memory and check content correctness*/
  counter = 0;
  uwReadwritestatus = 0;
  while ((counter < IS42S16400J_SIZE) && (uwReadwritestatus == 0))
  {
    ubReaddata_8b = *(__IO uint8_t*)(SDRAM_BANK_ADDR + counter);
    if ( ubReaddata_8b != (uint8_t)(ubWritedata_8b + counter))
    {
      uwReadwritestatus = 1;
      STM_EVAL_LEDOn(LED4);
      STM_EVAL_LEDOff(LED3);              
    }
    else
    {
      STM_EVAL_LEDOn(LED3);
      STM_EVAL_LEDOff(LED4);
    }
    counter++;
  } 
  
  if(uwReadwritestatus == 0)
  {
    LCD_Clear(LCD_COLOR_GREEN);
    LCD_DisplayStringLine(LCD_LINE_4, (uint8_t *)"  8-bits AHB      ");
    LCD_DisplayStringLine(LCD_LINE_5, (uint8_t *)"  Transaction     "); 
    LCD_DisplayStringLine(LCD_LINE_6, (uint8_t *)"  Test-> OK       ");   
  }
  else
  {
    LCD_Clear(LCD_COLOR_RED);
    LCD_DisplayStringLine(LCD_LINE_4, (uint8_t *)"    8-bits AHB     ");
    LCD_DisplayStringLine(LCD_LINE_5, (uint8_t *)"   Transaction     "); 
    LCD_DisplayStringLine(LCD_LINE_6, (uint8_t *)"   Test-> NOT OK   ");     
  }
  
  /*********************** 16-bits AHB transaction test ***********************/    
  /* Wait for User button to be pressed */
  while (STM_EVAL_PBGetState(BUTTON_USER) != Bit_SET)
  {}
  /* Wait for User button is released */
  while (STM_EVAL_PBGetState(BUTTON_USER) != Bit_RESET)
  {}
  
  /* Turn Off Leds */   
  STM_EVAL_LEDOff(LED3);
  STM_EVAL_LEDOff(LED4);
  
  /* Erase SDRAM memory */
  for (counter = 0x00; counter < IS42S16400J_SIZE; counter++)
  {
    *(__IO uint16_t*) (SDRAM_BANK_ADDR + 2*counter) = (uint16_t)0x00;
  }
  
  /* Write data value to all SDRAM memory */
  for (counter = 0; counter < IS42S16400J_SIZE; counter++)
  {
    *(__IO uint16_t*) (SDRAM_BANK_ADDR + 2*counter) = (uint16_t)(uhWritedata_16b + counter);
  }
  
  /* Read back SDRAM memory and check content correctness*/
  counter = 0;
  uwReadwritestatus = 0;
  while ((counter < IS42S16400J_SIZE) && (uwReadwritestatus == 0))
  {
    uhReaddata_16b = *(__IO uint16_t*)(SDRAM_BANK_ADDR + 2*counter);
    if ( uhReaddata_16b != (uint16_t)(uhWritedata_16b + counter))
    {
      uwReadwritestatus = 1;
      STM_EVAL_LEDOn(LED4);
      STM_EVAL_LEDOff(LED3); 
    }
    else
    {
      STM_EVAL_LEDOn(LED3);
      STM_EVAL_LEDOff(LED4);
    }
    counter++;
  }
  if(uwReadwritestatus == 0)
  {
    LCD_Clear(LCD_COLOR_GREEN);
    LCD_DisplayStringLine(LCD_LINE_4, (uint8_t *)"  16-bits AHB     ");
    LCD_DisplayStringLine(LCD_LINE_5, (uint8_t *)"  Transaction     "); 
    LCD_DisplayStringLine(LCD_LINE_6, (uint8_t *)"  Test-> OK       ");   
  }
  else
  {
    LCD_Clear(LCD_COLOR_RED);
    LCD_DisplayStringLine(LCD_LINE_4, (uint8_t *)"    16-bits AHB    ");
    LCD_DisplayStringLine(LCD_LINE_5, (uint8_t *)"   Transaction     "); 
    LCD_DisplayStringLine(LCD_LINE_6, (uint8_t *)"   Test-> NOT OK   ");     
  }
  }
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

  while (1)
  {}
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
