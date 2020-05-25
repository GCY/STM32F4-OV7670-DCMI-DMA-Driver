/**
  ******************************************************************************
  * @file    LTDC_AnimatedPictureFromUSB/main.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    11-November-2013
  * @brief   This example describes how to display on LCD pictures saved
  *          under USB mass storage.
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
#include "usbh_core.h"
#include "usbh_usr.h"
#include "usbh_msc_core.h"
#include  "lcd_log.h"
#include <stdio.h>
#include <string.h>

/** @addtogroup STM32F429I_DISCOVERY_Examples
* @{
*/

/** @defgroup LTDC_AnimatedPictureFromUSB
* @{
*/ 

/** @defgroup LTDC_AnimatedPictureFromUSB_Private_TypesDefinitions
* @{
*/ 
/**
* @}
*/ 

/** @defgroup LTDC_AnimatedPictureFromUSB_Private_Defines
* @{
*/ 
/**
* @}
*/ 


/** @defgroup LTDC_AnimatedPictureFromUSB_Private_Macros
* @{
*/ 
/**
* @}
*/ 


/** @defgroup LTDC_AnimatedPictureFromUSB_Private_Variables
* @{
*/
#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__ALIGN_BEGIN USB_OTG_CORE_HANDLE      USB_OTG_Core __ALIGN_END;

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__ALIGN_BEGIN USBH_HOST                USB_Host __ALIGN_END;
/**
* @}
*/ 
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup LTDC_AnimatedPictureFromUSB_Private_FunctionPrototypes
* @{
*/ 
/**
* @}
*/ 


/** @defgroup LTDC_AnimatedPictureFromUSB_Private_Functions
* @{
*/ 

/**
* @brief  Main routine for MSC class application
* @param  None
* @retval int
*/
int main(void)
{
  __IO uint32_t i = 0;
  
  /*!< At this stage the microcontroller clock setting is already configured, 
  this is done through SystemInit() function which is called from startup
  file (startup_stm32f429_439xx.s) before to branch to application main.
  To reconfigure the default setting of SystemInit() function, refer to
  system_stm32fxxx.c file
  */  
  
  /* Init Host Library */
  USBH_Init(&USB_OTG_Core, 
            USB_OTG_HS_CORE_ID,
            &USB_Host,
            &USBH_MSC_cb, 
            &USR_cb);

 
  while (1)
  {
    /* Host Task handler */
    USBH_Process(&USB_OTG_Core, &USB_Host);
    
    if (i++ == 0x10000)
    {
      STM_EVAL_LEDToggle(LED3);
      STM_EVAL_LEDToggle(LED4);
      i = 0;
    } 
    
  }
}

#ifdef USE_FULL_ASSERT
/**
* @brief  assert_failed
*         Reports the name of the source file and the source line number
*         where the assert_param error has occurred.
* @param  File: pointer to the source file name
* @param  Line: assert_param error line source number
* @retval None
*/
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
  ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  
  /* Infinite loop */
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

/**
* @}
*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
