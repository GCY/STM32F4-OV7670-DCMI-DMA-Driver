/**
  ******************************************************************************
  * @file    demo_cpu.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    11-November-2013 
  * @brief   CPU performance functions
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
#include <stdlib.h>  
#include "global_includes.h"
#include "DIALOG.h"
#include "cpu_utils.h"

/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
#define ID_FRAMEWIN_CPU    (GUI_ID_USER + 0x01)
#define ID_CPU_GRAPH       (GUI_ID_USER + 0x03)
#define ID_TEXT_CPU        (GUI_ID_USER + 0x04)
#define ID_BENCH_CPU       (GUI_ID_USER + 0x05)

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
GRAPH_DATA_Handle hData;
GRAPH_SCALE_Handle hScale;

static const GUI_WIDGET_CREATE_INFO _aDialog[] = {
  { FRAMEWIN_CreateIndirect, "Performance", ID_FRAMEWIN_CPU, 0, 0, 240, 320-20, WM_CF_STAYONTOP, 0 },
  { TEXT_CreateIndirect, "00000000 Pixels/s ", ID_TEXT_CPU, 28, 175, 300, 25, 0, 0, 0 }, 
  { BUTTON_CreateIndirect, "Start Speed Benchmark", ID_BENCH_CPU, 20 , 220 , 200, 40, 0, 0, 0 },  
};

static const GUI_COLOR _aColor[8] = {
  0x000000, 
  0x0000FF, 
  0x00FF00, 
  0x00FFFF, 
  0xFF0000, 
  0xFF00FF, 
  0xFFFF00, 
  0xFFFFFF
};

uint32_t Stop_Test = 0;

/* Private function prototypes -----------------------------------------------*/
int Run_SpeedTest(void) ;

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Return pixels per second rate
  * @param  None
  * @retval U32
  */
static U32 _GetPixelsPerSecond(void) {
  GUI_COLOR Color, BkColor;
  U32 x0, y0, x1, y1, xSize, ySize;
  I32 t, t0;
  U32 Cnt, PixelsPerSecond, PixelCnt;
  
  /* Find an area which is not obstructed by any windows */
  xSize   = LCD_GetXSize();
  ySize   = LCD_GetYSize();
  Cnt     = 0;
  x0      = 0;
  x1      = xSize - 1;
  y0      = 65;
  y1      = ySize - 60 - 1;
  Color   = GUI_GetColor();
  BkColor = GUI_GetBkColor();
  GUI_SetColor(BkColor);
  
  /* Repeat fill as often as possible in 100 ms */
  t0 = GUI_GetTime();
  do {
    GUI_FillRect(x0, y0, x1, y1);
    Cnt++;
    t = GUI_GetTime();    
  } while ((t - (t0 + 100)) <= 0);
  
  /* Compute result */
  t -= t0;
  PixelCnt = (x1 - x0 + 1) * (y1 - y0 + 1) * Cnt;
  PixelsPerSecond = PixelCnt / t * 1000;   
  GUI_SetColor(Color);
  return PixelsPerSecond;
}

/**
  * @brief  Run the speed test
  * @param  None
  * @retval int
  */
int Run_SpeedTest(void) {
  int      TimeStart, i;
  U32      PixelsPerSecond;
  unsigned aColorIndex[8];
  int      xSize, ySize, vySize;
  GUI_RECT Rect, ClipRect;
  xSize  = LCD_GetXSize();
  ySize  = LCD_GetYSize();
  vySize = LCD_GetVYSize();
#if GUI_SUPPORT_CURSOR
  GUI_CURSOR_Hide();
#endif
  if (vySize > ySize)
  {
    ClipRect.x0 = 0;
    ClipRect.y0 = 0;
    ClipRect.x1 = xSize;
    ClipRect.y1 = ySize;
    GUI_SetClipRect(&ClipRect);
  }
  
  Stop_Test = 0;
  
  for (i = 0; i< 8; i++)
  {
    aColorIndex[i] = GUI_Color2Index(_aColor[i]);
  }  
  TimeStart = GUI_GetTime();
  for (i = 0; ((GUI_GetTime() - TimeStart) < 5000) &&( Stop_Test == 0); i++)
  {
    GUI_SetColorIndex(aColorIndex[i&7]);
    
    /* Calculate random positions */
    Rect.x0 = (GUI_GetTime()/(i%25)) % xSize - xSize / 2;
    Rect.y0 = (GUI_GetTime()/(i%25)) % ySize - ySize / 2;
    Rect.x1 = Rect.x0 + 20 + (GUI_GetTime()/(i%25)) % xSize;
    Rect.y1 = Rect.y0 + 20 + (GUI_GetTime()/(i%25)) % ySize;

    GUI_FillRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1);
    
    /* Clip rectangle to visible area and add the number of pixels (for speed computation) */
    if (Rect.x1 >= xSize)
    {
      Rect.x1 = xSize - 1;
    }
    
    if (Rect.y1 >= ySize)
    {
      Rect.y1 = ySize - 1;
    }
    
    if (Rect.x0 < 0 )
    {
      Rect.x0 = 0;
    }
    
    if (Rect.y1 < 0)
    {
      Rect.y1 = 0;
    }
    
    GUI_Exec();
    
    /* Allow short breaks so we do not use all available CPU time ... */
  }
  PixelsPerSecond = _GetPixelsPerSecond();
  GUI_SetClipRect(NULL);
  return PixelsPerSecond;
}

/**
  * @brief  Callback function of the CPU window
  * @param  pMsg: pointer to a data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbCpuWindow(WM_MESSAGE * pMsg) {
  
  static WM_HTIMER hTimerTime; 
  switch (pMsg->MsgId) 
  {
  case WM_CREATE:
    /* Create timer */
    hTimerTime = WM_CreateTimer(pMsg->hWin, 0, 400, 0);        
    break;
    
  case WM_TIMER:
    GRAPH_DATA_YT_AddValue(hData , FreeRTOS_GetCPUUsage());
    WM_InvalidateWindow(pMsg->hWin);
    WM_RestartTimer(pMsg->Data.v, 0);
    break; 
    
  case WM_DELETE:
    WM_DeleteTimer(hTimerTime);
    break;
    
  default:
    WM_DefaultProc(pMsg);
  }
}

/**
  * @brief  Callback routine of the dialog
  * @param  pMsg: pointer to a data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int Id, NCode;
  WM_HWIN hGraph;
  int cpu_speed = 0;
  char temp[50];
  
  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    
    /* Initialization of 'CPU' */
    hItem = pMsg->hWin;
    FRAMEWIN_SetFont(hItem, GUI_FONT_13HB_ASCII);
    FRAMEWIN_AddCloseButton(hItem, FRAMEWIN_BUTTON_RIGHT,  0);
    
    hItem = TEXT_CreateEx(5, 30, 100, 25, pMsg->hWin, WM_CF_SHOW,0, 0x123,"");
    TEXT_SetFont(hItem, GUI_FONT_13B_1);
    TEXT_SetTextColor(hItem, 0x00804000);
    TEXT_SetText(hItem, "CPU Usage %:");
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_CPU);
    TEXT_SetFont(hItem, GUI_FONT_24B_ASCII);
    TEXT_SetTextColor(hItem, GUI_DARKRED);
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_CPU_GRAPH);  
    FRAMEWIN_SetBarColor(hItem, 0, GUI_DARKGRAY);  
    FRAMEWIN_SetBarColor(hItem, 1, GUI_DARKGRAY);   
    
    hGraph = GRAPH_CreateEx(5, 45, 230, 115, pMsg->hWin, WM_CF_SHOW, 0, GUI_ID_GRAPH0);
    hData = GRAPH_DATA_YT_Create(GUI_LIGHTGREEN, 500, 0, 20);
    GRAPH_SetGridVis(hGraph, 1);
    GRAPH_SetBorder(hGraph, 10, 4, 5, 4); 
    GRAPH_AttachData(hGraph, hData);
    hScale = GRAPH_SCALE_Create(20, GUI_TA_RIGHT, GRAPH_SCALE_CF_VERTICAL, 25);
    GRAPH_AttachScale(hGraph, hScale);  
    GRAPH_SCALE_SetTextColor(hScale, GUI_YELLOW);
    GRAPH_SetGridDistX(hGraph, 25);
    GRAPH_SetGridDistY(hGraph, 25);   
    
    WM_CreateWindowAsChild(0, 20, 240, 23, pMsg->hWin, WM_CF_SHOW | WM_CF_HASTRANS, _cbCpuWindow , 0); 
    
    break;
    
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);    /* Id of widget */
    NCode = pMsg->Data.v;               /* Notification code */
    switch (NCode) {
    case WM_NOTIFICATION_RELEASED:      /* React only if released */
      switch (Id) {
        
      case ID_BENCH_CPU:
        Stop_Test = 0;
        
        WM_HideWindow(pMsg->hWin);
        GUI_Exec();
        cpu_speed = Run_SpeedTest();
        WM_ShowWindow(pMsg->hWin);
        
        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_CPU);
        sprintf (temp, "%d  Pixels/s ", cpu_speed); 
        TEXT_SetText(hItem, temp);
        WM_InvalidateWindow(WM_GetDesktopWindowEx(1));
        break;
      }
      break;
      
    case WM_NOTIFICATION_CHILD_DELETED:
      Stop_Test = 1;
      WM_NotifyParent(WM_GetParent(pMsg->hWin), 0x500);
      break; 
    }
    break;
    
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/**
  * @brief  DEMO_Cpu
  * @param  hWin: Window handle
  * @retval None
  */
void DEMO_Cpu(WM_HWIN hWin)
{
  GUI_CreateDialogBox(_aDialog, GUI_COUNTOF(_aDialog), _cbDialog, hWin, 0, 0);
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
