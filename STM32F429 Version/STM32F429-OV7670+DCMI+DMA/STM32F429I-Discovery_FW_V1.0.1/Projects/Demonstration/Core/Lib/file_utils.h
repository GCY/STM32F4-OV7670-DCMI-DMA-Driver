/**
  ******************************************************************************
  * @file    file_utils.h
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    11-November-2013
  * @brief   Header for file_utils module
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _FILE_UTILS_H__
#define _FILE_UTILS_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "global_includes.h"

   /* Exported constants --------------------------------------------------------*/
#define FILELIST_DEPDTH                        100
#define FILELIST_LINESIZE                      40
#define FILEMGR_DIRECTORYSIZE                  128
#define FILEMGR_FILESIZE                        64   
/* Exported types ------------------------------------------------------------*/
   typedef struct _FILELIST_LineTypeDef
{
  uint8_t          type;
  uint8_t           line[FILEMGR_FILESIZE];
  
}
FILELIST_LineTypeDef;

typedef struct _FILELIST_FileTypeDef
{
  FILELIST_LineTypeDef  file[FILELIST_DEPDTH] ;
  uint16_t             ptr;
  uint8_t              full;
  int16_t              idx;
  
}
FILELIST_FileTypeDef;
/* Exported variables --------------------------------------------------------*/
extern FILELIST_FileTypeDef   FILEMGR_FileList;
extern char FILEMGR_CurrentDirectory[FILEMGR_DIRECTORYSIZE];
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void GetParentDir (char *dir);
void GetFileOnly (char *file, char *path);
#ifdef __cplusplus
}
#endif

#endif /* _FILE_UTILS_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
