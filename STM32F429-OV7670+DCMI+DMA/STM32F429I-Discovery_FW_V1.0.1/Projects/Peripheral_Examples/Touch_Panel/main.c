/**
  ******************************************************************************
  * @file    Touch_Panel/main.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    11-November-2013
  * @brief   This example describes how to configure and use the touch panel 
  *          mounted on STM32F429I-DISCO boards.
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

/** @addtogroup Touch_Panel
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void TP_Config(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief   Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  uint16_t linenum = 0;
  static TP_STATE* TP_State; 
    
  /*!< At this stage the microcontroller clock setting is already configured, 
  this is done through SystemInit() function which is called from startup
  file (startup_stm32f429_439xx.s) before to branch to application main.
  To reconfigure the default setting of SystemInit() function, refer to
  system_stm32f4xx.c file
  */      

  /* LCD initialization */
  LCD_Init();
  
  /* LCD Layer initialization */
  LCD_LayerInit();
    
  /* Enable the LTDC */
  LTDC_Cmd(ENABLE);
  
  /* Set LCD foreground layer */
  LCD_SetLayer(LCD_FOREGROUND_LAYER);
  
  /* Touch Panel configuration */
  TP_Config();
    
  while (1)
  {
 
    TP_State = IOE_TP_GetState();
    
    if((TP_State->TouchDetected) && ((TP_State->Y < 245) && (TP_State->Y >= 3)))
    {
      if((TP_State->X >= 237) || (TP_State->X < 3))
      {}     
      else
      {
        LCD_DrawFullCircle(TP_State->X, TP_State->Y, 3);
      }
    }
    else if ((TP_State->TouchDetected) && (TP_State->Y <= 280) && (TP_State->Y >= 250) && (TP_State->X >= 5) && (TP_State->X <= 35))
    {
      LCD_SetTextColor(LCD_COLOR_BLUE2);
    }
    else if ((TP_State->TouchDetected) && (TP_State->Y <= 280) && (TP_State->Y >= 250) && (TP_State->X >= 40) && (TP_State->X <= 70))
    {
      LCD_SetTextColor(LCD_COLOR_CYAN); 
    }
    else if ((TP_State->TouchDetected) && (TP_State->Y <= 280) && (TP_State->Y >= 250) && (TP_State->X >= 75) && (TP_State->X <= 105))
    {
      LCD_SetTextColor(LCD_COLOR_YELLOW); 
    }      
    else if ((TP_State->TouchDetected) && (TP_State->Y <= 318) && (TP_State->Y >= 288) && (TP_State->X >= 5) && (TP_State->X <= 35))
    {
      LCD_SetTextColor(LCD_COLOR_RED);
    }
    else if ((TP_State->TouchDetected) && (TP_State->Y <= 318) && (TP_State->Y >= 288) && (TP_State->X >= 40) && (TP_State->X <= 70))
    {
      LCD_SetTextColor(LCD_COLOR_BLUE); 
    }
    else if ((TP_State->TouchDetected) && (TP_State->Y <= 318) && (TP_State->Y >= 288) && (TP_State->X >= 75) && (TP_State->X <= 105))
    {
      LCD_SetTextColor(LCD_COLOR_GREEN); 
    }
    else if ((TP_State->TouchDetected) && (TP_State->Y <= 318) && (TP_State->Y >= 288) && (TP_State->X >= 110) && (TP_State->X <= 140))
    {
      LCD_SetTextColor(LCD_COLOR_BLACK); 
    }
    else if ((TP_State->TouchDetected) && (TP_State->Y <= 318) && (TP_State->Y >= 288) && (TP_State->X >= 145) && (TP_State->X <= 175))
    {
      LCD_SetTextColor(LCD_COLOR_MAGENTA); 
    }
    else if ((TP_State->TouchDetected) && (TP_State->Y <= 318) && (TP_State->Y >= 270) && (TP_State->X >= 180) && (TP_State->X <= 230))
    {
      LCD_SetFont(&Font8x8);
      for(linenum = 0; linenum < 31; linenum++)
      {
        LCD_ClearLine(LINE(linenum));
      }
    }
    else
    {
    }
  }
}

/**
* @brief  Configure the IO Expander and the Touch Panel.
* @param  None
* @retval None
*/
static void TP_Config(void)
{
  /* Clear the LCD */ 
  LCD_Clear(LCD_COLOR_WHITE);
  
  /* Configure the IO Expander */
  if (IOE_Config() == IOE_OK)
  {   
    LCD_SetFont(&Font8x8);
    LCD_DisplayStringLine(LINE(32), (uint8_t*)"              Touch Panel Paint     ");
    LCD_DisplayStringLine(LINE(34), (uint8_t*)"              Example               ");
    LCD_SetTextColor(LCD_COLOR_BLUE2); 
    LCD_DrawFullRect(5, 250, 30, 30);
    LCD_SetTextColor(LCD_COLOR_CYAN); 
    LCD_DrawFullRect(40, 250, 30, 30);
    LCD_SetTextColor(LCD_COLOR_YELLOW); 
    LCD_DrawFullRect(75, 250, 30, 30);
    LCD_SetTextColor(LCD_COLOR_RED); 
    LCD_DrawFullRect(5, 288, 30, 30);
    LCD_SetTextColor(LCD_COLOR_BLUE); 
    LCD_DrawFullRect(40, 288, 30, 30);
    LCD_SetTextColor(LCD_COLOR_GREEN); 
    LCD_DrawFullRect(75, 288, 30, 30);
    LCD_SetTextColor(LCD_COLOR_MAGENTA); 
    LCD_DrawFullRect(145, 288, 30, 30);
    LCD_SetTextColor(LCD_COLOR_BLACK); 
    LCD_DrawFullRect(110, 288, 30, 30);
    LCD_DrawRect(180, 270, 48, 50);
    LCD_SetFont(&Font16x24);
    LCD_DisplayChar(LCD_LINE_12, 195, 0x43);
    LCD_DrawLine(0, 248, 240, LCD_DIR_HORIZONTAL);
    LCD_DrawLine(0, 284, 180, LCD_DIR_HORIZONTAL);
    LCD_DrawLine(1, 248, 71, LCD_DIR_VERTICAL);
    LCD_DrawLine(37, 248, 71, LCD_DIR_VERTICAL);
    LCD_DrawLine(72, 248, 71, LCD_DIR_VERTICAL);
    LCD_DrawLine(107, 248, 71, LCD_DIR_VERTICAL);
    LCD_DrawLine(142, 284, 36, LCD_DIR_VERTICAL);
    LCD_DrawLine(0, 319, 240, LCD_DIR_HORIZONTAL);
  }  
  else
  {
    LCD_Clear(LCD_COLOR_RED);
    LCD_SetTextColor(LCD_COLOR_BLACK); 
    LCD_DisplayStringLine(LCD_LINE_6,(uint8_t*)"   IOE NOT OK      ");
    LCD_DisplayStringLine(LCD_LINE_7,(uint8_t*)"Reset the board   ");
    LCD_DisplayStringLine(LCD_LINE_8,(uint8_t*)"and try again     ");
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
