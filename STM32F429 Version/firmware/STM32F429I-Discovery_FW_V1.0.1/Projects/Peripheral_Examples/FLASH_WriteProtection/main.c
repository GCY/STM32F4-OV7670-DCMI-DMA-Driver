/**
  ******************************************************************************
  * @file    FLASH_WriteProtection/main.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    11-November-2013
  * @brief   This example provides a description of how to enable and disable 
  *          the write protectionfor FLASH integrated within STM32F429I-DISCO 
  *          Devices.
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

/** @addtogroup FLASH_WriteProtection
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define FLASH_WRP_SECTORS   (OB_WRP_Sector_2 | OB_WRP_Sector_3) /* sectors 2 and 3  */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint32_t SectorsWRPStatus = 0xFFF;

/* Private function prototypes -----------------------------------------------*/
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
  file (startup_stm32f429_439xx.s) before to branch to application main.
  To reconfigure the default setting of SystemInit() function, refer to
  system_stm32f4xx.c file
  */     
  
  /* LCD initialization */
  LCD_Init();
  LCD_LayerInit();
  
  /* LTDC reload configuration */  
  LTDC_ReloadConfig(LTDC_IMReload);
  
  /* Enable the LTDC */
  LTDC_Cmd(ENABLE);
  
  /* Set LCD foreground layer */
  LCD_SetLayer(LCD_FOREGROUND_LAYER);
   
  /* Initialize User Button mounted on STM32F429I-DISCO */
  STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_GPIO);
  
  /* Display test name on LCD */
  LCD_Clear(LCD_COLOR_WHITE);
  LCD_SetBackColor(LCD_COLOR_BLUE);
  LCD_SetTextColor(LCD_COLOR_WHITE);
  LCD_DisplayStringLine(LCD_LINE_4,(uint8_t*)" Flash Write   ");
  LCD_DisplayStringLine(LCD_LINE_5,(uint8_t*)"protection test");
  LCD_DisplayStringLine(LCD_LINE_7,(uint8_t*)" Press User    ");
  LCD_DisplayStringLine(LCD_LINE_8,(uint8_t*)" push-button   ");
  
  while (1)
  {
    /* Wait for User push-button is pressed */
    while (STM_EVAL_PBGetState(BUTTON_USER) != Bit_RESET)
    {
    }
    
    /* Wait for User push-button is released */
    while (STM_EVAL_PBGetState(BUTTON_USER) != Bit_SET)
    {
    }
    
    /* Get FLASH_WRP_SECTORS write protection status */
    SectorsWRPStatus = FLASH_OB_GetWRP() & FLASH_WRP_SECTORS;
    
    if (SectorsWRPStatus == 0x00)
    {
      /* If FLASH_WRP_SECTORS are write protected, disable the write protection */
      
      /* Enable the Flash option control register access */
      FLASH_OB_Unlock();
      
      /* Disable FLASH_WRP_SECTORS write protection */
      FLASH_OB_WRPConfig(FLASH_WRP_SECTORS, DISABLE); 
      
      /* Start the Option Bytes programming process */  
      if (FLASH_OB_Launch() != FLASH_COMPLETE)
      {
        /* User can add here some code to deal with this error */
        while (1)
        {
        }
      }
      /* Disable the Flash option control register access (recommended to protect 
      the option Bytes against possible unwanted operations) */
      FLASH_OB_Lock();
      
      /* Get FLASH_WRP_SECTORS write protection status */
      SectorsWRPStatus = FLASH_OB_GetWRP() & FLASH_WRP_SECTORS;
      
      /* Check if FLASH_WRP_SECTORS write protection is disabled */
      if (SectorsWRPStatus == FLASH_WRP_SECTORS)
      {
        LCD_Clear(LCD_COLOR_GREEN);
        LCD_SetTextColor(LCD_COLOR_BLACK);
        LCD_DisplayStringLine(LCD_LINE_5,(uint8_t*)"     Write      ");
        LCD_DisplayStringLine(LCD_LINE_6,(uint8_t*)" protection is  ");
        LCD_DisplayStringLine(LCD_LINE_7,(uint8_t*)" disabled       ");
      }
      else
      {
        LCD_Clear(LCD_COLOR_RED);
        LCD_SetTextColor(LCD_COLOR_BLACK);
        LCD_DisplayStringLine(LCD_LINE_5,(uint8_t*)"     Write      ");
        LCD_DisplayStringLine(LCD_LINE_6,(uint8_t*)" protection is  ");
        LCD_DisplayStringLine(LCD_LINE_7,(uint8_t*)" not disabled   ");
      }
    }
    else
    { /* If FLASH_WRP_SECTORS are not write protected, enable the write protection */
      
      /* Enable the Flash option control register access */
      FLASH_OB_Unlock();
      
      /* Enable FLASH_WRP_SECTORS write protection */
      FLASH_OB_WRPConfig(FLASH_WRP_SECTORS, ENABLE); 
      
      /* Start the Option Bytes programming process */  
      if (FLASH_OB_Launch() != FLASH_COMPLETE)
      {
        /* User can add here some code to deal with this error */
        while (1)
        {
        }
      }
      
      /* Disable the Flash option control register access (recommended to protect 
      the option Bytes against possible unwanted operations) */
      FLASH_OB_Lock();
      
      /* Get FLASH_WRP_SECTORS write protection status */
      SectorsWRPStatus = FLASH_OB_GetWRP() & FLASH_WRP_SECTORS;
      
      /* Check if FLASH_WRP_SECTORS are write protected */
      if (SectorsWRPStatus == 0x00)
      {
        LCD_Clear(LCD_COLOR_GREEN);
        LCD_SetTextColor(LCD_COLOR_BLACK);
        LCD_DisplayStringLine(LCD_LINE_5,(uint8_t*)"     Write      ");
        LCD_DisplayStringLine(LCD_LINE_6,(uint8_t*)" protection is  ");
        LCD_DisplayStringLine(LCD_LINE_7,(uint8_t*)" enabled        ");
      }
      else
      {
        LCD_Clear(LCD_COLOR_RED);
        LCD_SetTextColor(LCD_COLOR_BLACK);
        LCD_DisplayStringLine(LCD_LINE_5,(uint8_t*)"     Write      ");
        LCD_DisplayStringLine(LCD_LINE_6,(uint8_t*)" protection is  ");
        LCD_DisplayStringLine(LCD_LINE_7,(uint8_t*)" not enabled    ");
      }
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
