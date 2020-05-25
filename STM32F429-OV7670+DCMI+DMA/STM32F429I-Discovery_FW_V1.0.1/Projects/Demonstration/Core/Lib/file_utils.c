/**
  ******************************************************************************
  * @file    file_utils.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    11-November-2013
  * @brief   Utilities for file handling
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
#include "file_utils.h"
#include <string.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
FILELIST_FileTypeDef    FILEMGR_FileList;
char FILEMGR_CurrentDirectory[FILEMGR_DIRECTORYSIZE];
/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Retieve the parent directory from a full file path
  * @param  path: pointer to file path
  * @retval None
*/
void GetParentDir (char *dir)
{
  uint8_t idx = FILEMGR_DIRECTORYSIZE;
  
  for (  ; idx > 0 ; idx --)
  {
    
    if (dir [idx] == '/')
    {
      dir [idx] = 0;
      break;
    }
    dir [idx] = 0;
  }
}


/**
  * @brief  Retieve the file name from a full file path
  * @param  file: pointer to base path
  * @param  path: pointer to full path
  * @retval None
*/
void GetFileOnly (char *file, char *path)
{
  char *baseName1, *baseName2;
  baseName1 = strrchr(path,'/');
  baseName2 = strrchr(path,':');
  
  if(baseName1++) 
  { 
    strcpy(file, baseName1);
  }
  else 
  {
    if (baseName2++) 
    {
      
      strcpy(file, baseName2);
    }
    else
    {
      strcpy(file, path);
    }
  }
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
