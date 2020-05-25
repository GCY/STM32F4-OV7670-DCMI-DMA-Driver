/**
  ******************************************************************************
  * @file    common_res.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    11-November-2013 
  * @brief   Common resources
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

#include "stdlib.h"
#include "common_res.h"
#include "GUI.h"

#ifndef GUI_CONST_STORAGE
  #define GUI_CONST_STORAGE const
#endif

/* Bitmap data for Button keys */
GUI_CONST_STORAGE GUI_COLOR aColorsButtonOn[] = {
  GUI_DARKBLUE ,GUI_LIGHTBLUE, 
};


GUI_CONST_STORAGE GUI_LOGPALETTE PalButtonOn = {
  2,	/* number of entries */
  1, 	/* No transparency */
  &aColorsButtonOn[0]
};

/* Bitmap data for Button keys */
GUI_CONST_STORAGE GUI_COLOR aColorsButtonOff[] = {
  GUI_DARKBLUE ,GUI_LIGHTBLUE, 
};


GUI_CONST_STORAGE GUI_LOGPALETTE PalButtonOff = {
  2,	/* number of entries */
  1, 	/* No transparency */
  &aColorsButtonOff[0]
};


/* Bitmap data for Button keys */
GUI_CONST_STORAGE GUI_COLOR aColorsButtonPause[] = {
  GUI_DARKBLUE ,GUI_DARKRED, 
};


GUI_CONST_STORAGE GUI_LOGPALETTE PalButtonPause = {
  2,	/* number of entries */
  1, 	/* No transparency */
  &aColorsButtonPause[0]
};

/* Bitmap data for Button keys */
GUI_CONST_STORAGE GUI_COLOR aColorsButtonPlay[] = {
  GUI_DARKBLUE ,GUI_DARKGREEN, 
};


GUI_CONST_STORAGE GUI_LOGPALETTE PalButtonPlay = {
  2,	/* number of entries */
  1, 	/* No transparency */
  &aColorsButtonPlay[0]
};

GUI_CONST_STORAGE unsigned char acButtonPlayPause[] = {
  __XX____,___X__X_,  
  __XXXX__,___X__X_,  
  __XXXXXX,___X__X_,
  __XXXXXX,XX_X__X_,
  __XXXXXX,XXXX__X_,
  __XXXXXX,XX_X__X_,
  __XXXXXX,___X__X_,
  __XXXX__,___X__X_,  
  __XX____,___X__X_,    
};

GUI_CONST_STORAGE GUI_BITMAP bmButtonPlayPauseOn = {
  16,           /* XSize */
  9,            /* YSize */
  2,            /* BytesPerLine */
  1,            /* BitsPerPixel */
  acButtonPlayPause, /* Pointer to picture data (indices) */
  &PalButtonOn     /* Pointer to palette */
};

GUI_CONST_STORAGE GUI_BITMAP bmButtonPlayPauseOff = {
  16,           /* XSize */
  9,            /* YSize */
  2,            /* BytesPerLine */
  1,            /* BitsPerPixel */
  acButtonPlayPause, /* Pointer to picture data (indices) */
  &PalButtonOff     /* Pointer to palette */
};

GUI_CONST_STORAGE GUI_BITMAP bmButtonPause = {
  16,           /* XSize */
  9,            /* YSize */
  2,            /* BytesPerLine */
  1,            /* BitsPerPixel */
  acButtonPlayPause, /* Pointer to picture data (indices) */
  &PalButtonPause     /* Pointer to palette */
};

GUI_CONST_STORAGE GUI_BITMAP bmButtonPlay = {
  16,           /* XSize */
  9,            /* YSize */
  2,            /* BytesPerLine */
  1,            /* BitsPerPixel */
  acButtonPlayPause, /* Pointer to picture data (indices) */
  &PalButtonPlay     /* Pointer to palette */
};


/******************************************************************************/

GUI_CONST_STORAGE unsigned char acButtonMute[] = {
  ________,______X_,  
  ________,____XXX_,  
  _____XXX,__XXXXX_,  
  _____XXX,XXXXXXX_,
  _____XXX,XXXXXXX_,
  _____XXX,XXXXXXX_,
  _____XXX,XXXXXXX_,
  _____XXX,XXXXXXX_,
  _____XXX,__XXXXX_,  
  ________,____XXX_,  
  ________,______X_,    
};

GUI_CONST_STORAGE GUI_BITMAP bmButtonMuteOn = {
  16,           /* XSize */
  11,            /* YSize */
  2,            /* BytesPerLine */
  1,            /* BitsPerPixel */
  acButtonMute, /* Pointer to picture data (indices) */
  &PalButtonOn     /* Pointer to palette */
};

GUI_CONST_STORAGE GUI_BITMAP bmButtonMuteOff = {
  16,           /* XSize */
  11,            /* YSize */
  2,            /* BytesPerLine */
  1,            /* BitsPerPixel */
  acButtonMute, /* Pointer to picture data (indices) */
  &PalButtonOff     /* Pointer to palette */
};

GUI_CONST_STORAGE GUI_BITMAP bmButtonMuteAct = {
  16,           /* XSize */
  11,            /* YSize */
  2,            /* BytesPerLine */
  1,            /* BitsPerPixel */
  acButtonMute, /* Pointer to picture data (indices) */
  &PalButtonPause     /* Pointer to palette */
};
/******************************************************************************/

GUI_CONST_STORAGE unsigned char acButtonNext[] = 
{
  __XX____,___X____,  
  __XXXX__,___X____,  
  __XXXXXX,___X____,
  __XXXXXX,XX_X____,
  __XXXXXX,XXXX____,
  __XXXXXX,XX_X____,
  __XXXXXX,___X____,
  __XXXX__,___X____,  
  __XX____,___X____,    
};

GUI_CONST_STORAGE GUI_BITMAP bmButtonNextOn =
{
  16,           /* XSize */
  9,            /* YSize */
  2,            /* BytesPerLine */
  1,            /* BitsPerPixel */
  acButtonNext, /* Pointer to picture data (indices) */
  &PalButtonOn, /* Pointer to palette */
};

GUI_CONST_STORAGE GUI_BITMAP bmButtonNextOff = 
{
  16,           /* XSize */
  9,            /* YSize */
  2,            /* BytesPerLine */
  1,            /* BitsPerPixel */
  acButtonNext, /* Pointer to picture data (indices) */
  &PalButtonOff     /* Pointer to palette */
};
/******************************************************************************/

GUI_CONST_STORAGE unsigned char acButtonPrevious[] = 
{
  ____X___,____XX__,  
  ____X___,__XXXX__,  
  ____X___,XXXXXX__,
  ____X_XX,XXXXXX__,
  ____XXXX,XXXXXX__,
  ____X_XX,XXXXXX__,
  ____X___,XXXXXX__,
  ____X___,__XXXX__,  
  ____X___,____XX__,    
};

GUI_CONST_STORAGE GUI_BITMAP bmButtonPreviousOn =
{
  16,           /* XSize */
  9,            /* YSize */
  2,            /* BytesPerLine */
  1,            /* BitsPerPixel */
  acButtonPrevious, /* Pointer to picture data (indices) */
  &PalButtonOn, /* Pointer to palette */
};

GUI_CONST_STORAGE GUI_BITMAP bmButtonPreviousOff = 
{
  16,           /* XSize */
  9,            /* YSize */
  2,            /* BytesPerLine */
  1,            /* BitsPerPixel */
  acButtonPrevious, /* Pointer to picture data (indices) */
  &PalButtonOff     /* Pointer to palette */
};
/******************************************************************************/

GUI_CONST_STORAGE unsigned char acButtonStop[] = 
{
  ________,________,  
  ____XXXX,XXXX____,  
  ____XXXX,XXXX____,
  ____XXXX,XXXX____,
  ____XXXX,XXXX____,
  ____XXXX,XXXX____,
  ____XXXX,XXXX____,
  ____XXXX,XXXX____,  
  ________,________,    
};

GUI_CONST_STORAGE GUI_BITMAP bmButtonStopOn =
{
  16,           /* XSize */
  9,            /* YSize */
  2,            /* BytesPerLine */
  1,            /* BitsPerPixel */
  acButtonStop, /* Pointer to picture data (indices) */
  &PalButtonOn, /* Pointer to palette */
};

GUI_CONST_STORAGE GUI_BITMAP bmButtonStopOff = 
{
  16,           /* XSize */
  9,            /* YSize */
  2,            /* BytesPerLine */
  1,            /* BitsPerPixel */
  acButtonStop, /* Pointer to picture data (indices) */
  &PalButtonOff     /* Pointer to palette */
};

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
