/**
  ******************************************************************************
  * @file    demo_startup.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    11-November-2013
  * @brief   Demo startup
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
#include "global_includes.h"
#include "DIALOG.h"
#include "Res\startup_res.c"

/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint32_t idx = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Background callback
  * @param  pMsg: pointer to a data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbBk(WM_MESSAGE * pMsg) {
  uint16_t   xPos, Step = 20, i;
  const GUI_BITMAP * pBm;
  
  switch (pMsg->MsgId) 
  {
  case WM_PAINT:

    GUI_SetBkColor(GUI_WHITE);
    GUI_Clear();
    
    GUI_DrawBitmap(&bmSTLogo, (LCD_GetXSize() - bmSTLogo.XSize)/2 , (LCD_GetYSize() - bmSTLogo.YSize)/2); 
    
    for (i = 0, xPos = LCD_GetXSize() / 2 - 2 * Step; i < 5; i++, xPos += Step) 
    {
      pBm = (idx == i) ? &_bmWhiteCircle_10x10 : &_bmWhiteCircle_6x6;
      GUI_DrawBitmap(pBm, xPos - pBm->XSize / 2, 250 - pBm->YSize / 2);
    }
  }
}

/**
  * @brief  DEMO_Starup
  * @param  None
  * @retval None
  */
void DEMO_Starup(void)
{
  uint8_t loop = 16 ;
  GUI_RECT Rect = {50, 245, 380, 255};
  
  WM_SetCallback(WM_GetDesktopWindowEx(0), _cbBk);
  
  while (loop--)
  {
    idx = (16- loop) % 5;
    
    WM_InvalidateArea(&Rect);
    
    GUI_Delay(200);
  }
}
/*************************** End of file ****************************/
