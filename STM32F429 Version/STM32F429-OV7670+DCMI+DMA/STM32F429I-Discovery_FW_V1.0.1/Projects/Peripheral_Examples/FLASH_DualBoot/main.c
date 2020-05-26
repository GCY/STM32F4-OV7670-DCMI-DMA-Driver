/**
  ******************************************************************************
  * @file    FLASH_DualBoot/main.c 
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    11-November-2013
  * @brief   This example provides a description of how to program bank1 and 
  *          bank2 of the FLASH memory integrated within STM32F429I-DISCO Devices
  *          and swap between both of them.
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
#include <stdio.h>

/** @addtogroup STM32F429I_DISCOVERY_Examples
  * @{
  */

/** @addtogroup FLASH_DualBoot
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

  #define MESSAGE1   "STM32F429X          " 
  #define MESSAGE2   "Device running on   " 

  #ifdef FLASH_BANK1
    #define MESSAGE3   "FLASH BANK1        "
  #else
    #define MESSAGE3   "FLASH BANK2        "
  #endif

#define MESSAGE5   "PUSH Key button"

#define MESSAGE7   "   Swap BFB2   "


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static __IO uint32_t uwTimingDelay;
RCC_ClocksTypeDef RCC_Clocks;
uint8_t Message[24];
uint32_t VAR = 0;
/* Private function prototypes -----------------------------------------------*/
static void Delay(__IO uint32_t nTime);

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

  /* SysTick end of count event each 10ms */
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 100);
  
  /* Enable the SYSCFG APB Clock */
  RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
  
  /* Initialize LEDs and LCD available on EVAL board **************************/
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);  
  
  /* Configure the Joystick buttons */
  STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_GPIO);
  /* Display message on LCD ***************************************************/ 
  /* Initialize the LCD */
  LCD_Init();
  LCD_LayerInit();
  
  /* LTDC reload configuration */  
  LTDC_ReloadConfig(LTDC_IMReload);
  
  /* Enable the LTDC */
  LTDC_Cmd(ENABLE);
  
  /* Set LCD foreground layer */
  LCD_SetLayer(LCD_FOREGROUND_LAYER);
  
  /* Clear the Foreground Layer */ 
  LCD_Clear(LCD_COLOR_WHITE);
  
    /* Set the LCD Back Color */
#ifdef FLASH_BANK1
  LCD_SetBackColor(LCD_COLOR_BLUE);
#else
  LCD_SetBackColor(LCD_COLOR_RED);
#endif /* BOOT_FROM_BANK1 */
  
  /* Set the LCD Text Color */
  LCD_SetTextColor(LCD_COLOR_WHITE);
  
  /* Display LCD messages */
  LCD_DisplayStringLine(LCD_LINE_1, (uint8_t *)MESSAGE1);
  LCD_DisplayStringLine(LCD_LINE_2, (uint8_t *)MESSAGE3);
  LCD_DisplayStringLine(LCD_LINE_3, (uint8_t *)MESSAGE5);
  
  LCD_DisplayStringLine(LCD_LINE_4, (uint8_t *)MESSAGE7);

  VAR  =  *(__IO uint32_t *)0x40013800;
  sprintf((char*)Message, "SYSCFG:%#x       ", VAR );
  LCD_DisplayStringLine(LCD_LINE_6, (uint8_t *)Message);
  
  VAR  =  *(__IO uint32_t *)0x1FFFC000;
  sprintf((char*)Message, "OB:%#x       ", VAR );
  LCD_DisplayStringLine(LCD_LINE_7, (uint8_t *)Message);
  
  VAR  =  *(__IO uint32_t *)0x00000000;
  sprintf((char*)Message, "ADD0:%#x       ", VAR );
  LCD_DisplayStringLine(LCD_LINE_8, (uint8_t *)Message);
  
   VAR  =  *(__IO uint32_t *)0x00000004;
  sprintf((char*)Message, "ADD4:%#x     ", VAR );
  LCD_DisplayStringLine(LCD_LINE_9, (uint8_t *)Message);
  
  /* Turn on LEDs *************************************************************/
  STM_EVAL_LEDOn(LED3);
  STM_EVAL_LEDOn(LED4);

  /* Add your application code here */
    
  /* Infinite loop */
  while (1)
  {
    /*--- If Wake-up button is pushed, Set BFB2 bit to enable boot from Bank2
          (active after next reset, w/ Boot pins set in Boot from Flash memory position ---*/
    
        /* Wait for User push-button is pressed */
    if (STM_EVAL_PBGetState(BUTTON_USER) != Bit_RESET)
    {     
      /* Wait for User push-button is released */
      while (STM_EVAL_PBGetState(BUTTON_USER) != Bit_SET)
      {
      }
      
      /* Set BFB2 bit to enable boot from Flash Bank2 */
      FLASH_OB_Unlock();
      FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | 
                      FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR | FLASH_FLAG_RDERR);
      FLASH_OB_BORConfig(OB_BOR_OFF);
      
      if (((*(__IO uint8_t *)OPTCR_BYTE0_ADDRESS) & (FLASH_OPTCR_BFB2)) != 0)
      {
        FLASH_OB_BootConfig(OB_Dual_BootDisabled);
      }
      else
      {
        FLASH_OB_BootConfig(OB_Dual_BootEnabled);
      }
      FLASH_OB_Launch();
      
      FLASH_OB_Lock();
      
      NVIC_SystemReset();     
      
    }
#ifdef FLASH_BANK1        
    /* Toggle LD3 */
    STM_EVAL_LEDToggle(LED3);
    /* Insert 50 ms delay */
    Delay(15);
#else
    /* Toggle LD4 */    
    STM_EVAL_LEDToggle(LED4);
    /* Insert 50 ms delay */
    Delay(15);
#endif    
  }
}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void Delay(__IO uint32_t nTime)
{ 
  uwTimingDelay = nTime;

  while(uwTimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (uwTimingDelay != 0x00)
  { 
    uwTimingDelay--;
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

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


