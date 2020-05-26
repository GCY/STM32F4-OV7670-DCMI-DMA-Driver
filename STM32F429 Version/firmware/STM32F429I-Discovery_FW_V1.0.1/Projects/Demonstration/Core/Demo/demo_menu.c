/**
  ******************************************************************************
  * @file    demo_menu.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    11-November-2013
  * @brief   Demo menu and icons
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
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "WM.h"
#include "Res\menu_res.c"
#include "Res\clock.c"
#include "Res\cpu.c"
#include "Res\game.c"
#include "Res\image.c"
#include "Res\info.c"
#include "Res\multimedia.c"
#include "Res\background.c"
#include "time_utils.h"
#include "cpu_utils.h"

/* External variables --------------------------------------------------------*/
extern __IO uint8_t alarm_now;
extern __IO uint32_t alarm_set;
extern RTC_AlarmTypeDef  RTC_AlarmStructure;
extern WM_HWIN  VIDEO_hWin, hVideoScreen;
extern WM_HWIN  IMAGE_hWin, vFrame;
extern __IO uint32_t TS_Orientation;
extern __IO uint32_t IMAGE_Enlarge;
extern __IO uint32_t VIDEO_Enlarge;

extern void DEMO_SystemInfo ( WM_HWIN hWin);
extern void DEMO_Game(WM_HWIN hWin);
extern void DEMO_Video(WM_HWIN hWin);
extern void DEMO_Image(WM_HWIN hWin);
extern void DEMO_Clock(WM_HWIN hWin);
extern void DEMO_Cpu(WM_HWIN hWin);

/* Private typedef -----------------------------------------------------------*/
typedef struct {
  const GUI_BITMAP * pBitmap;  
  const char       * pText;
  const char       * pExplanation;
} BITMAP_ITEM;

/* Private defines -----------------------------------------------------------*/
#define WM_MSG_USB_STATUS_CHANGED      WM_USER + 0x01
#define ID_TIMER_TIME                  1

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t current_module = 0xFF;

static const BITMAP_ITEM _aBitmapItem[] = {
  {&bmmultimedia,   "Video Player"        , "Launch MPJPEG video"},
  {&bmimage,        "Image Browser"       , "Browse Images"},  
  {&bmclock,        "Clock/Calendar"      , "Clock settings"},
  {&bmgame,         "Game"                , "Launch Reversi game"},
  {&bmcpu,          "Perfomance"          , "Show CPU performance"},  
  {&bminfo,         "System Info"         , "Get System Information"},
};

static void (* _apModules[])( WM_HWIN hWin) = 
{
  DEMO_Video,
  DEMO_Image,
  DEMO_Clock,
  DEMO_Game,
  DEMO_Cpu,
  DEMO_SystemInfo,
};

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Callback routine of desktop window
  * @param  pMsg: pointer to a data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbBk(WM_MESSAGE * pMsg) {
  uint32_t NCode, Id, sel;
  static uint32_t module_mutex = 0;
 
  switch (pMsg->MsgId) 
  {
  case WM_PAINT:
    GUI_SetBkColor(GUI_TRANSPARENT);
    GUI_Clear();
    break;
    
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);     
    NCode = pMsg->Data.v;  
    
    switch (NCode) 
    {  
    case WM_NOTIFICATION_CLICKED: 
      if (Id == '0')
      {
        sel = ICONVIEW_GetSel(pMsg->hWinSrc);
        if(sel < GUI_COUNTOF(_aBitmapItem))
        {
          if(module_mutex == 0)
          {
            module_mutex = 1;
            _apModules [sel](pMsg->hWinSrc);
            current_module = sel;
          }
        }
      }
      break;
      
    case 0x500:
      module_mutex = 0;
      current_module = 0xFF;
      ICONVIEW_SetSel(pMsg->hWinSrc, -1);
      break;
      
    default:
      break;
    }
    break;
  default:
    WM_DefaultProc(pMsg);
  }
}

/**
  * @brief  Callback routine of the status bar
  * @param  pMsg: pointer to a data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbStatus(WM_MESSAGE * pMsg) {
  int xSize, ySize;
  static uint8_t TempStr[50];
  
  RTC_TimeTypeDef   RTC_TimeStructure;
  RTC_DateTypeDef   RTC_DateStructure;
  uint8_t sec, min, hour;
  
  static WM_HTIMER hTimerTime;
  WM_HWIN hWin;
  
  hWin = pMsg->hWin;
  switch (pMsg->MsgId) {

  case WM_CREATE:
    hTimerTime = WM_CreateTimer(hWin, ID_TIMER_TIME, 1000, 0);
    break;
  case WM_DELETE:
    WM_DeleteTimer(hTimerTime);
    break;
  case WM_TIMER:
    WM_InvalidateWindow(hWin);
    WM_RestartTimer(pMsg->Data.v, 0);
    break;
    
  case WM_MSG_USB_STATUS_CHANGED:    
    WM_InvalidateWindow(hWin);
    break;
    
  case WM_PAINT:
    xSize = WM_GetWindowSizeX(hWin);
    ySize = WM_GetWindowSizeY(hWin);
    
    /* Draw background */
    GUI_SetColor(0x303030);
    GUI_FillRect(0, 0, xSize , ySize - 3);
    GUI_SetColor(0x808080);
    GUI_DrawHLine(ySize - 2, 0, xSize );
    GUI_SetColor(0x404040);
    GUI_DrawHLine(ySize - 1, 0, xSize );
    
    /* Draw time & Date */
    GUI_SetTextMode(GUI_TM_TRANS);
    GUI_SetColor(GUI_WHITE);
    GUI_SetFont(GUI_FONT_13B_ASCII);
    
    RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
    sec    =  RTC_TimeStructure.RTC_Seconds;
    min    =  RTC_TimeStructure.RTC_Minutes;
    hour   =  RTC_TimeStructure.RTC_Hours;
    
    RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
    
    sprintf((char *)TempStr, "%02d:%02d:%02d", hour , min, sec);
    GUI_DispStringAt((char *)TempStr, xSize - 50, 4);
    
    /* Draw alarm icon */
    if (alarm_set == 1)
    {
      GUI_DrawBitmap(&_bmAlarm_16x16, xSize - 73, 3);
    }
    
    /* Logo */
    GUI_DrawBitmap(&bmSTLogo40x20, 5, 1);
    
    /* USB */
    if(USB_Host_Application_Ready == 1)
    {
      GUI_DrawBitmap(&bmusbdisk, xSize - 115, 0);
    }
    
    sprintf((char *)TempStr, "CPU : %d %%", FreeRTOS_GetCPUUsage());
    
    if(FreeRTOS_GetCPUUsage() < 75 )
    {
      GUI_SetColor(GUI_WHITE);
    }
    else
    {
      GUI_SetColor(GUI_RED);
    }
    GUI_DispStringAt( (char *)TempStr, 50, 4);
    GUI_SetColor(GUI_WHITE);
    break;
    
  default:
    WM_DefaultProc(pMsg);
  }
}

/**
  * @brief  Demo Main menu
  * @param  None
  * @retval None
  */
void DEMO_MainMenu(void) 
{
  ICONVIEW_Handle hIcon;
  WM_HWIN hStatusWin;
  static uint32_t prev_usb_state = 0;
  int i = 0;
  

  GUI_DrawBitmap(&bmbackground, 0,0);     
  GUI_SetAlpha(0xA0);
  GUI_DrawBitmap(&bmSTM32_F4, LCD_GetXSize() - bmSTM32_F4.XSize - 10 , LCD_GetYSize() - bmSTM32_F4.YSize - 10); 
  GUI_SetAlpha(0x00);

  WM_SetCallback(WM_GetDesktopWindowEx(1), _cbBk);
  
  hStatusWin = WM_CreateWindowAsChild(
                                      0,
                                      0,
                                      LCD_GetXSize(),
                                      25,
                                      WM_GetDesktopWindowEx(1), WM_CF_SHOW | WM_CF_HASTRANS , _cbStatus, 0);
  
  hIcon = ICONVIEW_CreateEx(0, 25, LCD_GetXSize(), LCD_GetYSize()- 26, WM_GetDesktopWindowEx(1), WM_CF_SHOW | WM_CF_HASTRANS ,0 ,'0', 115, 95);
  
  ICONVIEW_SetFont(hIcon, &GUI_Font13B_ASCII);
  
  ICONVIEW_SetBkColor(hIcon, ICONVIEW_CI_SEL, 0x941000 | 0x80404040);
  
  ICONVIEW_SetSpace(hIcon, GUI_COORD_Y, 3);
  
  ICONVIEW_SetFrame(hIcon, GUI_COORD_Y, 1);
  
  for (i = 0; i < GUI_COUNTOF(_aBitmapItem); i++)
  {
    ICONVIEW_AddBitmapItem(hIcon,_aBitmapItem[i].pBitmap, _aBitmapItem[i].pText);
  }
  
  GUI_SelectLayer(1);  

  while (1) 
  {
    if(USB_Host_Application_Ready != prev_usb_state)
    {
      prev_usb_state = USB_Host_Application_Ready;
      WM_SendMessageNoPara(hStatusWin, WM_MSG_USB_STATUS_CHANGED);
      
      if(USB_Host_Application_Ready == 0)
      {
        if(current_module == 0)
        {
          GUI_EndDialog(VIDEO_hWin, 0);
          if (VIDEO_Enlarge == 1)
          {
            WM_DeleteWindow(hVideoScreen);
            GUI_SetOrientation(0);
            TS_Orientation = 0;
          }
        }
        else if (current_module == 1)
        {
          GUI_EndDialog(IMAGE_hWin, 0);
          if (IMAGE_Enlarge == 1)
          {
            WM_DeleteWindow(vFrame);
            GUI_SetOrientation(0);
            TS_Orientation = 0;
          }
        }
      }
    }
    GUI_Delay(100);
  }
}

/*************************** End of file ****************************/
