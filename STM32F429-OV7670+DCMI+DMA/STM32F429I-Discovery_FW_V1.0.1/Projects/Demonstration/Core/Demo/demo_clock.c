/**
  ******************************************************************************
  * @file    demo_clock.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    11-November-2013  
  * @brief   Clock/Calendar/Alarm functions
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
#include "time_utils.h"
#include "calendar.h"
#include "Res\clock_res.c"
#include "Res\alarmclock.c"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define ID_FRAMEWIN_CLOCK           (GUI_ID_USER + 0x01)
#define ID_CALENDAR                 (GUI_ID_USER + 0x02)
#define ID_TEXT_DATE                (GUI_ID_USER + 0x03)
#define ID_BUTTON_CLOSE_CLOCK       (GUI_ID_USER + 0x04)
#define ID_BUTTON_SETTINGS_CLOCK    (GUI_ID_USER + 0x05)
#define ID_TEXT_CLOCK               (GUI_ID_USER + 0x07)

#define ID_FRAMEWIN_SETTINGS        (GUI_ID_USER + 0x08)
#define ID_CLOSE_SETTINGS           (GUI_ID_USER + 0x15)
#define ID_SET_SETTINGS             (GUI_ID_USER + 0x16)

#define ID_CLOCK_HOUR                (GUI_ID_USER + 0x20)
#define ID_CLOCK_MIN                 (GUI_ID_USER + 0x21)
#define ID_CLOCK_SEC                 (GUI_ID_USER + 0x22)

#define ID_CLOCK_DAY                 (GUI_ID_USER + 0x23)
#define ID_CLOCK_MONTH               (GUI_ID_USER + 0x24)
#define ID_CLOCK_YEAR                (GUI_ID_USER + 0x25)

#define ID_CLOCK_AHOUR               (GUI_ID_USER + 0x26)
#define ID_CLOCK_AMIN                (GUI_ID_USER + 0x27)
#define ID_CLOCK_ASEC                (GUI_ID_USER + 0x28)

#define ID_CLOCK_CHECK_DATE          (GUI_ID_USER + 0x29)
#define ID_CLOCK_CHECK_TIME          (GUI_ID_USER + 0x2A)
#define ID_CLOCK_CHECK_ALARM         (GUI_ID_USER + 0x2B)

#define ID_FRAMEWIN_INFO         (GUI_ID_USER + 0x02)
#define ID_ALARM                 (GUI_ID_USER + 0x03)
#define ID_IMAGE_INFO            (GUI_ID_USER + 0x04)
#define ID_TIME                  (GUI_ID_USER + 0x05)
#define ID_BUTTON_INFO_CLOSE     (GUI_ID_USER + 0x06)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static const GUI_WIDGET_CREATE_INFO _aDialog[] = {
  { FRAMEWIN_CreateIndirect, "Clock", ID_FRAMEWIN_CLOCK, 0, 0, 240, 320-20, WM_CF_STAYONTOP, 0 },
  { TEXT_CreateIndirect, "", ID_TEXT_DATE, 75, 30, 130, 23, 0, 0, TEXT_CF_HCENTER },
  { TEXT_CreateIndirect, "", ID_TEXT_CLOCK, 60, 0, 160, 60, 0, 0, 0 },  
  { BUTTON_CreateIndirect, "Close", ID_BUTTON_CLOSE_CLOCK, 30, 230, 80, 24, 0, 0, 0 },
  { BUTTON_CreateIndirect, "Settings", ID_BUTTON_SETTINGS_CLOCK, 130, 230, 80, 24, 0, 0, 0},
};

static const GUI_WIDGET_CREATE_INFO _aDialogSettingsCreate[] = {
  { FRAMEWIN_CreateIndirect, "Settings", ID_FRAMEWIN_SETTINGS, 0, 0, 240, 320-20, FRAMEWIN_CF_MOVEABLE|WM_CF_STAYONTOP, 0},
  { BUTTON_CreateIndirect, "Cancel", ID_CLOSE_SETTINGS, 30, 240, 80, 24, 0, 0, 0 },
  { BUTTON_CreateIndirect, "Apply", ID_SET_SETTINGS, 30, 210, 80, 24, 0, 0, 0},
};

/* Array of keys */
static int _aKey[] = {GUI_KEY_DELETE, GUI_KEY_TAB, GUI_KEY_LEFT, GUI_KEY_RIGHT};

/* Dialog resource of numpad */
static const GUI_WIDGET_CREATE_INFO _aDialogNumPad[] = {
  /*  Function                 Text      Id                 Px   Py   Dx   Dy */
  { WINDOW_CreateIndirect,   0,        0, 240-95, 320-130,  95, 130},
  { BUTTON_CreateIndirect,   "7",      GUI_ID_USER +  7,   5,   5,  25,  20},
  { BUTTON_CreateIndirect,   "8",      GUI_ID_USER +  8,  35,   5,  25,  20},
  { BUTTON_CreateIndirect,   "9",      GUI_ID_USER +  9,  65,   5,  25,  20},
  { BUTTON_CreateIndirect,   "4",      GUI_ID_USER +  4,   5,  30,  25,  20},
  { BUTTON_CreateIndirect,   "5",      GUI_ID_USER +  5,  35,  30,  25,  20},
  { BUTTON_CreateIndirect,   "6",      GUI_ID_USER +  6,  65,  30,  25,  20},
  { BUTTON_CreateIndirect,   "1",      GUI_ID_USER +  1,   5,  55,  25,  20},
  { BUTTON_CreateIndirect,   "2",      GUI_ID_USER +  2,  35,  55,  25,  20},
  { BUTTON_CreateIndirect,   "3",      GUI_ID_USER +  3,  65,  55,  25,  20},
  { BUTTON_CreateIndirect,   "0",      GUI_ID_USER +  0,   5,  80,  25,  20},
  { BUTTON_CreateIndirect,   ".",      GUI_ID_USER + 10,  35,  80,  25,  20},
  { BUTTON_CreateIndirect,   "Del",    GUI_ID_USER + 11,  65,  80,  25,  20},
  { BUTTON_CreateIndirect,   "Tab",    GUI_ID_USER + 12,   5, 105,  25,  20},
  { BUTTON_CreateIndirect,   0,        GUI_ID_USER + 13,  35, 105,  25,  20},
  { BUTTON_CreateIndirect,   0,        GUI_ID_USER + 14,  65, 105,  25,  20},
};

static const GUI_WIDGET_CREATE_INFO _aDialogAlarm_V[] = {
  { FRAMEWIN_CreateIndirect, "Alarm", ID_FRAMEWIN_INFO, 0, 20, 240, 300, 0, 0 },
  { IMAGE_CreateIndirect, "Image", ID_IMAGE_INFO, 70, 35, 100, 100, 0, 0, 0 },
  { TEXT_CreateIndirect, "!!! ALARM !!!", ID_ALARM, 77, 160, 100, 30, 0, 0, 0 },
  { TEXT_CreateIndirect, "", ID_TIME, 74, 200, 93, 30, 0, 0, 0 },
  { BUTTON_CreateIndirect, "Close", ID_BUTTON_INFO_CLOSE, 90, 240, 60, 30, 0, 0, 0 },  
};

static const GUI_WIDGET_CREATE_INFO _aDialogAlarm_H[] = {
  { FRAMEWIN_CreateIndirect, "Alarm", ID_FRAMEWIN_INFO, 0, 0, 320, 240, 0, 0 },
  { IMAGE_CreateIndirect, "Image", ID_IMAGE_INFO, 23, 50, 100, 100, 0, 0, 0 },
  { TEXT_CreateIndirect, "!!! ALARM !!!", ID_ALARM, 174, 37, 89, 22, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "", ID_TIME, 174, 88, 93, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Close", ID_BUTTON_INFO_CLOSE, 186, 148, 60, 30, 0, 0x0, 0 },  
};

WM_HWIN   hNumPad;
WM_HWIN  ALARM_hWin;

__IO uint32_t alarm_set = 0;
extern __IO uint8_t alarm_now;
extern __IO uint32_t TS_Orientation;
/* Private function prototypes -----------------------------------------------*/
RTC_AlarmTypeDef  RTC_AlarmStructure;

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Callback function of the numpad
  * @param  pMsg: pointer to a data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbDialogNumPad(WM_MESSAGE * pMsg) {
  GUI_RECT r;
  int i, NCode, Id, Pressed = 0;
  WM_HWIN hDlg, hItem;
  hDlg = pMsg->hWin;
  switch (pMsg->MsgId) {
  case WM_PAINT:
    WM_GetClientRect(&r);
    GUI_SetColor(0x000000);
    GUI_DrawRect(r.x0, r.y0, r.x1, r.y1);          /* Draw rectangle around it */
    /* Draw the bright sides */
    GUI_SetColor(0xffffff);
    GUI_DrawHLine(r.y0 + 1, r.x0 + 1, r.x1 - 2);   /* Draw top line */
    GUI_DrawVLine(r.x0 + 1, r.y0 + 1, r.y1 - 2);
    /* Draw the dark sides */
    GUI_SetColor(0x555555);
    GUI_DrawHLine(r.y1-1, r.x0 + 1, r.x1 - 1);
    GUI_DrawVLine(r.x1-1, r.y0 + 1, r.y1 - 2);
    break;
  case WM_INIT_DIALOG:
    for (i = 0; i < GUI_COUNTOF(_aDialogNumPad) - 1; i++) {
      hItem = WM_GetDialogItem(hDlg, GUI_ID_USER + i);
      BUTTON_SetFocussable(hItem, 0);                       /* Set all buttons non focussable */
      switch (i) {
      case 13:
        BUTTON_SetBitmapEx(hItem, 0, &_bmArrowLeft, 7, 7);  /* Set bitmap for arrow left button (unpressed) */
        BUTTON_SetBitmapEx(hItem, 1, &_bmArrowLeft, 7, 7);  /* Set bitmap for arrow left button (pressed) */
        break;
      case 14:
        BUTTON_SetBitmapEx(hItem, 0, &_bmArrowRight, 7, 7); /* Set bitmap for arrow right button (unpressed) */
        BUTTON_SetBitmapEx(hItem, 1, &_bmArrowRight, 7, 7); /* Set bitmap for arrow right button (pressed) */
        break;
      }
    }
    hItem = WM_GetDialogItem(hDlg, GUI_ID_USER + 12);
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);      /* Id of widget */
    NCode = pMsg->Data.v;                 /* Notification code */
    switch (NCode) {
    case WM_NOTIFICATION_CLICKED:
      Pressed = 1;
    case WM_NOTIFICATION_RELEASED:
      if ((Id >= GUI_ID_USER) && (Id <= (GUI_ID_USER + GUI_COUNTOF(_aDialogNumPad) - 1))) {
        int Key;
        if (Id < GUI_ID_USER + 11) {
          char acBuffer[10];
          BUTTON_GetText(pMsg->hWinSrc, acBuffer, sizeof(acBuffer)); /* Get the text of the button */
          Key = acBuffer[0];
        } else {
          Key = _aKey[Id - GUI_ID_USER - 11];                        /* Get the text from the array */
        }
        GUI_SendKeyMsg(Key, Pressed);                                /* Send a key message to the focussed window */
      }
      break;
    }
  default:
    WM_DefaultProc(pMsg);
  }
}

/**
  * @brief  Callback function of the Settings dialog
  * @param  pMsg: pointer to a data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbDialogSettings(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int Id, NCode, idx;
  static int8_t sec, min, hour;
  static int8_t asec, amin, ahour;
  static int8_t day, month, max_days;
  static int16_t  year;
  SPINBOX_Handle   hSpin;
  DROPDOWN_Handle  hDropMonth; 
  TEXT_Handle     hText  ;
  static  CALENDAR_DATE   current_date;
  RTC_TimeTypeDef   RTC_TimeStructure;
  RTC_DateTypeDef   RTC_DateStructure;
  static uint8_t TempStr[50];  
  
  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    
    /* Get Clock setting from RTC */
    RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
    
    sec    =  RTC_TimeStructure.RTC_Seconds;
    min    =  RTC_TimeStructure.RTC_Minutes;
    hour   =  RTC_TimeStructure.RTC_Hours;
    
    RTC_GetAlarm(RTC_Format_BIN, RTC_Alarm_A, &RTC_AlarmStructure);
    
    asec  = RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds;
    amin  = RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes;
    ahour = RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours;
    
    RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
    
    year =  RTC_DateStructure.RTC_Year + 2000;
    month =  RTC_DateStructure.RTC_Month;
    day =  RTC_DateStructure.RTC_Date;
    
    max_days = GetMaxDays(month, year);
    
    /* Update the dialog items */
    hItem = pMsg->hWin;
    FRAMEWIN_SetFont(hItem, GUI_FONT_13B_ASCII);
    
    /* Date */
    
    hText = TEXT_CreateEx(20, 20, 100, 25, pMsg->hWin, WM_CF_SHOW,0, 0x11F," Date : ");
    TEXT_SetFont(hText, GUI_FONT_13B_ASCII);
    TEXT_SetTextColor(hText, 0x00804000);
    
    hSpin = SPINBOX_CreateEx(20, 35, 40, 18, pMsg->hWin, WM_CF_SHOW, ID_CLOCK_DAY, 1, max_days);
    SPINBOX_SetFont(hSpin, GUI_FONT_13B_ASCII);
    SPINBOX_SetTextColor(hSpin, SPINBOX_CI_ENABLED, 0x00804000);
    SPINBOX_EnableBlink(hSpin, 250, 1);
      
      
    SPINBOX_SetValue(hSpin, day);
    
    hDropMonth = DROPDOWN_CreateEx(65, 35, 80, 160, pMsg->hWin, WM_CF_SHOW, 0,  ID_CLOCK_MONTH);
    DROPDOWN_SetFont(hDropMonth, GUI_FONT_13B_ASCII);
    DROPDOWN_SetTextColor(hDropMonth, DROPDOWN_CI_UNSEL, 0x00804000);
    DROPDOWN_SetTextColor(hDropMonth, DROPDOWN_CI_SEL, 0x00804000);    
    
    for (idx = 0; idx < 12; idx++ )
    {
      DROPDOWN_AddString (hDropMonth, (char *)strMonth[idx]);
    }
    
    DROPDOWN_SetSel(hDropMonth, month - 1);
    
    hSpin = SPINBOX_CreateEx(150, 35, 50, 18, pMsg->hWin, WM_CF_SHOW, ID_CLOCK_YEAR, 2000, 2099);
    SPINBOX_SetFont(hSpin, GUI_FONT_13B_ASCII);
    SPINBOX_SetTextColor(hSpin, SPINBOX_CI_ENABLED, 0x00804000);  
    SPINBOX_EnableBlink(hSpin, 250, 1);
    SPINBOX_SetValue(hSpin, year);
    
    hItem = CHECKBOX_Create(205, 37, 20, 26, pMsg->hWin, ID_CLOCK_CHECK_DATE ,WM_CF_SHOW);
    CHECKBOX_SetState(hItem, 1);
    
    /* Time */
    
    hText = TEXT_CreateEx(20, 50 + 20, 100, 25, pMsg->hWin, WM_CF_SHOW,0, 0x123," Time : ");
    TEXT_SetFont(hText, GUI_FONT_13B_ASCII);
    TEXT_SetTextColor(hText, 0x00804000);    
    
    hSpin = SPINBOX_CreateEx(20, 65 + 20, 40, 18, pMsg->hWin, WM_CF_SHOW, ID_CLOCK_HOUR, 0,23);
    SPINBOX_SetFont(hSpin, GUI_FONT_13B_ASCII);
    SPINBOX_SetTextColor(hSpin, SPINBOX_CI_ENABLED, 0x00804000);      
    SPINBOX_SetValue(hSpin, hour);
    SPINBOX_EnableBlink(hSpin, 250, 1);
    
    hSpin = SPINBOX_CreateEx(75, 65 + 20, 40, 18, pMsg->hWin, WM_CF_SHOW, ID_CLOCK_MIN, 0, 59);
    SPINBOX_SetFont(hSpin, GUI_FONT_13B_ASCII);  
    SPINBOX_SetTextColor(hSpin, SPINBOX_CI_ENABLED, 0x00804000);  
    SPINBOX_SetValue(hSpin, min);
    SPINBOX_EnableBlink(hSpin, 250, 1);
    
    hSpin = SPINBOX_CreateEx(130, 65 + 20, 40, 18, pMsg->hWin, WM_CF_SHOW, ID_CLOCK_SEC, 0, 59);
    SPINBOX_SetFont(hSpin, GUI_FONT_13B_ASCII);  
    SPINBOX_SetTextColor(hSpin, SPINBOX_CI_ENABLED, 0x00804000);  
    SPINBOX_SetValue(hSpin, sec);
    SPINBOX_EnableBlink(hSpin, 250, 1);
    
    
    hItem = CHECKBOX_Create(205, 65 + 20, 20, 26, pMsg->hWin, ID_CLOCK_CHECK_TIME ,WM_CF_SHOW);
    CHECKBOX_SetState(hItem, 1);
    
    /* Alarm */
    
    hText = TEXT_CreateEx(20, 78 + 40, 100, 25, pMsg->hWin, WM_CF_SHOW,0, 0x126," Alarm : ");
    TEXT_SetFont(hText, GUI_FONT_13B_ASCII);
    TEXT_SetTextColor(hText, 0x00804000);    
    
    hSpin = SPINBOX_CreateEx(20, 93 + 40, 40, 18, pMsg->hWin, WM_CF_SHOW, ID_CLOCK_AHOUR, 0,23);
    SPINBOX_SetFont(hSpin, GUI_FONT_13B_ASCII);
    SPINBOX_SetTextColor(hSpin, SPINBOX_CI_ENABLED, 0x00804000);      
    SPINBOX_SetValue(hSpin, ahour);
    SPINBOX_EnableBlink(hSpin, 250, 1);
    
    
    hSpin = SPINBOX_CreateEx(75, 93 + 40, 40, 18, pMsg->hWin, WM_CF_SHOW, ID_CLOCK_AMIN, 0, 59);
    SPINBOX_SetFont(hSpin, GUI_FONT_13B_ASCII); 
    SPINBOX_SetTextColor(hSpin, SPINBOX_CI_ENABLED, 0x00804000);  
    SPINBOX_SetValue(hSpin, amin);
    SPINBOX_EnableBlink(hSpin, 250, 1);
        
    hSpin = SPINBOX_CreateEx(130, 93 + 40, 40, 18, pMsg->hWin, WM_CF_SHOW, ID_CLOCK_ASEC, 0, 59);
    SPINBOX_SetFont(hSpin, GUI_FONT_13B_ASCII);  
    SPINBOX_SetTextColor(hSpin, SPINBOX_CI_ENABLED, 0x00804000);  
    SPINBOX_SetValue(hSpin, asec);
    hItem = CHECKBOX_Create(205, 93 + 40, 20, 26, pMsg->hWin, ID_CLOCK_CHECK_ALARM ,WM_CF_SHOW);
    CHECKBOX_SetState(hItem, 1);
    SPINBOX_EnableBlink(hSpin, 250, 1);
    
    break;
    
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
      
    case ID_CLOSE_SETTINGS: /* Notifications sent by 'Close' */
      switch(NCode) {
      case WM_NOTIFICATION_RELEASED:
        /* Exit */
        GUI_EndDialog(pMsg->hWin, 0);
        GUI_EndDialog (hNumPad, 0);
        break;
      }
      break;
    case ID_SET_SETTINGS: /* Notifications sent by 'Apply' */
      switch(NCode) {
      case WM_NOTIFICATION_RELEASED:
        
        if(CHECKBOX_GetState( WM_GetDialogItem(WM_GetParent(pMsg->hWin), ID_CLOCK_CHECK_DATE)))
        { 
          current_date.Year = year =  SPINBOX_GetValue (WM_GetDialogItem(pMsg->hWin, ID_CLOCK_YEAR));
          current_date.Month = month = DROPDOWN_GetSel (WM_GetDialogItem(pMsg->hWin, ID_CLOCK_MONTH)) + 1;
          current_date.Day = day =  SPINBOX_GetValue (WM_GetDialogItem(pMsg->hWin, ID_CLOCK_DAY)); 
          
          
          RTC_DateStructure.RTC_Year  = year - 2000;
          RTC_DateStructure.RTC_Month = month;
          RTC_DateStructure.RTC_Date  = day;
          RTC_DateStructure.RTC_WeekDay = 0;
          RTC_SetDate(RTC_Format_BIN, &RTC_DateStructure);
          
          hItem = WM_GetDialogItem(WM_GetParent(pMsg->hWin), ID_CALENDAR);
          CALENDAR_SetDate(hItem, &current_date);
          CALENDAR_SetSel(hItem, &current_date);
          
          /* Date */
          hItem = WM_GetDialogItem(WM_GetParent(pMsg->hWin), ID_TEXT_DATE);    
          sprintf((char *)TempStr, "%02d, %s, %04d",day , strMonth[month-1], year);
          TEXT_SetText(hItem, (char *)TempStr);
        }
        
        if(CHECKBOX_GetState( WM_GetDialogItem(WM_GetParent(pMsg->hWin), ID_CLOCK_CHECK_TIME)))
        {
          /* Save new param in RTC */
          sec    =  SPINBOX_GetValue (WM_GetDialogItem(pMsg->hWin, ID_CLOCK_SEC));
          min    =  SPINBOX_GetValue (WM_GetDialogItem(pMsg->hWin, ID_CLOCK_MIN));
          hour   =  SPINBOX_GetValue (WM_GetDialogItem(pMsg->hWin, ID_CLOCK_HOUR));
          
          RTC_TimeStructure.RTC_Seconds = sec;
          RTC_TimeStructure.RTC_Minutes = min;
          RTC_TimeStructure.RTC_Hours   = hour;
          RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure);
        }
        
        if(CHECKBOX_GetState( WM_GetDialogItem(WM_GetParent(pMsg->hWin), ID_CLOCK_CHECK_ALARM)))
        {
          asec  = SPINBOX_GetValue (WM_GetDialogItem(pMsg->hWin, ID_CLOCK_ASEC));
          amin  = SPINBOX_GetValue (WM_GetDialogItem(pMsg->hWin, ID_CLOCK_AMIN));
          ahour = SPINBOX_GetValue (WM_GetDialogItem(pMsg->hWin, ID_CLOCK_AHOUR));
          
          /* Disable the Alarm A */
          RTC_AlarmCmd(RTC_Alarm_A, DISABLE);
          
          /* Disable the RTC Alarm A Interrupt */
          RTC_ITConfig(RTC_IT_ALRA, DISABLE);
          
          STM_EVAL_LEDOff(LED4);
          
          RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds = asec;
          RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes = amin;
          RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours = ahour;
          RTC_AlarmStructure.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_Date;
          RTC_AlarmStructure.RTC_AlarmDateWeekDay = day;
          RTC_SetAlarm(RTC_Format_BIN, RTC_Alarm_A, &RTC_AlarmStructure);
          
          /* Enable the RTC Alarm A Interrupt */
          RTC_ITConfig(RTC_IT_ALRA, ENABLE);
          
          /* Enable the alarm  A */
          RTC_AlarmCmd(RTC_Alarm_A, ENABLE);
          
          alarm_set = 1;
        }
        
        /* Exit */
        WM_InvalidateWindow(WM_GetParent(pMsg->hWin));
        GUI_EndDialog(pMsg->hWin, 0);
        GUI_EndDialog (hNumPad, 0);
        break;
      }
      break;
    }
    break;
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/**
  * @brief  Callback function of the Clock window
  * @param  pMsg: pointer to a data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbClockWindow(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  static WM_HTIMER hTimerTime;  
  RTC_TimeTypeDef   RTC_TimeStructure;
  RTC_DateTypeDef   RTC_DateStructure;
  uint8_t sec, min, hour, day, month;
  uint16_t year;
  static uint8_t TempStr[50];
  static  CALENDAR_DATE   current_date;
  
  switch (pMsg->MsgId) 
  {
    
  case WM_CREATE:
    RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
    current_date.Year = RTC_DateStructure.RTC_Year + 2000;
    current_date.Month = RTC_DateStructure.RTC_Month;
    current_date.Day = RTC_DateStructure.RTC_Date;
    /* Create timer */
    hTimerTime = WM_CreateTimer(pMsg->hWin, 0, 1000, 0);        
    break;
    
  case WM_TIMER:
    /* Write date and clock */
    RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
    sec    =  RTC_TimeStructure.RTC_Seconds;
    min    =  RTC_TimeStructure.RTC_Minutes;
    hour   =  RTC_TimeStructure.RTC_Hours;
    
    RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
    year =  RTC_DateStructure.RTC_Year + 2000;
    month =  RTC_DateStructure.RTC_Month;
    day =  RTC_DateStructure.RTC_Date;  
    
    if((current_date.Day != day) ||(current_date.Month != month)
       ||(current_date.Year != year))
    {
      current_date.Year = year =  RTC_DateStructure.RTC_Year + 2000;
      current_date.Month = month =  RTC_DateStructure.RTC_Month;
      current_date.Day = day =  RTC_DateStructure.RTC_Date;
      
      hItem = WM_GetDialogItem(WM_GetParent(pMsg->hWin), ID_TEXT_DATE);    
      sprintf((char *)TempStr, "%02d, %s, %04d",day , strMonth[month-1], year);
      TEXT_SetText(hItem, (char *)TempStr);
      
      hItem = WM_GetDialogItem(WM_GetParent(pMsg->hWin), ID_CALENDAR);
      CALENDAR_SetDate(hItem, &current_date);
      CALENDAR_SetSel(hItem, &current_date);
      WM_InvalidateWindow(hItem);
    }
    
    hItem = WM_GetDialogItem(WM_GetParent(pMsg->hWin), ID_TEXT_CLOCK);
    sprintf((char *)TempStr, "%02d:%02d:%02d",hour , min, sec);
    TEXT_SetText(hItem, (char *)TempStr);
    
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
  WM_HWIN  hItem;
  int Id, NCode;
  RTC_TimeTypeDef   RTC_TimeStructure;
  RTC_DateTypeDef   RTC_DateStructure;
  uint8_t sec, min, hour, day, month;
  uint16_t year;
  uint8_t offset, max;
  static uint8_t TempStr[50];
  
  switch (pMsg->MsgId) {
    
  case WM_PAINT:
    break;
    
  case WM_INIT_DIALOG:
    
    RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
    sec    =  RTC_TimeStructure.RTC_Seconds;
    min    =  RTC_TimeStructure.RTC_Minutes;
    hour   =  RTC_TimeStructure.RTC_Hours;
    
    RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
    year =  RTC_DateStructure.RTC_Year + 2000;
    month =  RTC_DateStructure.RTC_Month;
    day =  RTC_DateStructure.RTC_Date;
    
    /* Initialization of 'System Information' */
    hItem = pMsg->hWin;
    FRAMEWIN_SetFont(hItem, GUI_FONT_13B_ASCII);
    
    /* Initialization of 'Close' */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CLOSE_CLOCK);
    BUTTON_SetFont(hItem, GUI_FONT_13B_ASCII);
    
    /* Set date in text mode */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_DATE);
    TEXT_SetFont(hItem, GUI_FONT_13B_1);
    TEXT_SetTextColor(hItem, 0x00804000);
    
    WM_CreateWindowAsChild(80, 45, 354, 23, pMsg->hWin, WM_CF_SHOW | WM_CF_HASTRANS, _cbClockWindow , 0);
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_CLOCK);
    TEXT_SetFont(hItem, &GUI_FontBauhaus9332);
    TEXT_SetTextColor(hItem, 0x00804000);   
    
    /* Set Init values */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_DATE);
    
    /* Write date and clock */
    sprintf((char *)TempStr, "%02d, %s, %04d",day , strMonth[month-1], year);
    TEXT_SetText(hItem, (char *)TempStr);
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_CLOCK);
    sprintf((char *)TempStr, "%02d:%02d:%02d",hour , min, sec);
    TEXT_SetText(hItem, (char *)TempStr);
    
    GetDateOffset (year, month, &offset , &max);
    
    CALENDAR_SetDefaultSize(CALENDAR_SI_HEADER, 25 );
    CALENDAR_SetDefaultSize(CALENDAR_SI_CELL_X, 30 );
    CALENDAR_SetDefaultSize(CALENDAR_SI_CELL_Y, 20 );
    
    CALENDAR_SetDefaultFont(CALENDAR_FI_CONTENT,GUI_FONT_16B_1 );
    CALENDAR_SetDefaultFont(CALENDAR_FI_HEADER, GUI_FONT_16B_1) ;    
    
    CALENDAR_Create(pMsg->hWin, 15, 70, year, month, day, 2, ID_CALENDAR, WM_CF_SHOW);
    
    WM_InvalidateWindow(pMsg->hWin);    
    break;
    
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);    /* Id of widget */
    NCode = pMsg->Data.v;               /* Notification code */
    switch (NCode) {
    case WM_NOTIFICATION_RELEASED:      /* React only if released */
      switch (Id) {
      case ID_BUTTON_CLOSE_CLOCK:
        GUI_EndDialog(pMsg->hWin, 0);
        break;
        
      case ID_BUTTON_SETTINGS_CLOCK:
        GUI_CreateDialogBox(_aDialogSettingsCreate, GUI_COUNTOF(_aDialogSettingsCreate), &_cbDialogSettings, pMsg->hWin, 0, 0);
        hNumPad = GUI_CreateDialogBox(_aDialogNumPad, 
                                      GUI_COUNTOF(_aDialogNumPad), 
                                      _cbDialogNumPad, WM_GetDesktopWindowEx(1), 0, 0); /* Create the numpad dialog */
        WM_SetStayOnTop(hNumPad, 1);        
        break;
      }
      
      break;
      
    case WM_NOTIFICATION_CHILD_DELETED:
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
  * @brief  Callback routine of the alarm dialog
  * @param  pMsg: pointer to a data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbAlarmDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int Id, NCode;
  char temp[50];
  uint8_t asec, amin, ahour;
  
  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    
    /* Initialization of 'Alarm' */
    hItem = pMsg->hWin;
    FRAMEWIN_SetFont(hItem, GUI_FONT_13HB_ASCII);
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_ALARM);
    TEXT_SetFont(hItem, GUI_FONT_13HB_ASCII);
    
    /* Initialization of 'Image' */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_INFO);
    IMAGE_SetBitmap(hItem, &bmalarmclock);
    
    /* Initialization of 'Time' */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TIME);
    ahour = RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours;
    amin = RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes;
    asec = RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds;
    sprintf (temp, "%02d:%02d:%02d", ahour, amin, asec);
    TEXT_SetText(hItem, temp);
    TEXT_SetFont(hItem, GUI_FONT_24B_ASCII);
    TEXT_SetTextColor(hItem, GUI_BROWN);
    
    /* Initialization of 'Close' */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_INFO_CLOSE);
    BUTTON_SetFont(hItem, GUI_FONT_13HB_ASCII);
    break;
    
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);    /* Id of widget */
    NCode = pMsg->Data.v;               /* Notification code */
    switch (NCode) {
    case WM_NOTIFICATION_RELEASED:      /* React only if released */
      switch (Id) {
      case ID_BUTTON_INFO_CLOSE:
        GUI_EndDialog(pMsg->hWin, 0);
        STM_EVAL_LEDOff(LED4);
        alarm_set=0;
        break;
      }
      break;
    }
    break;
    
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/**
  * @brief  ALARM_BackgroundProcess
  * @param  None
  * @retval None
  */
void ALARM_BackgroundProcess (void)
{
   
    if (alarm_now == 0)
    {
     
      if (TS_Orientation == 1)
      {
        ALARM_hWin = GUI_CreateDialogBox(_aDialogAlarm_H, GUI_COUNTOF(_aDialogAlarm_H), _cbAlarmDialog, WM_GetDesktopWindowEx(1), 0, 0);
      }
      else
      {
        ALARM_hWin = GUI_CreateDialogBox(_aDialogAlarm_V, GUI_COUNTOF(_aDialogAlarm_V), _cbAlarmDialog, WM_GetDesktopWindowEx(1), 0, 0);
      }
      WM_SetStayOnTop(ALARM_hWin, 1);
      alarm_now = 1;
    }
}
/**
  * @brief  DEMO_Clock
  * @param  hWin: Window handle
  * @retval None
  */
void DEMO_Clock(WM_HWIN hWin)
{
  GUI_CreateDialogBox(_aDialog, GUI_COUNTOF(_aDialog), _cbDialog, hWin, 0, 0);
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
