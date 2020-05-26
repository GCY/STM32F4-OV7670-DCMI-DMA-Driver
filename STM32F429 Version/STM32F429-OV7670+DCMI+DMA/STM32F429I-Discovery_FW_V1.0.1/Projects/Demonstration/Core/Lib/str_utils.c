/**
  ******************************************************************************
  * @file    str_utils.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    11-November-2013
  * @brief   Utilities for string handling
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
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
#include <stdio.h>
#include <string.h>
#include "str_utils.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  This function return a file extension
  * @param  filename : full file name string
  * @retval File extension string
  */
char *get_filename_ext(const char *filename) 
{ 
  char *dot = strrchr(filename, '.'); 
  if(!dot || dot == filename) return ""; 
  return dot + 1; 
} 
/**
  * @brief  This function check file extension
  * @param  filename : full file name string
  * @param  ext : file extension string
  * @retval result : true or false
  */
uint8_t check_filename_ext (char *filename, char *ext)
{
  char *temp;
  temp = get_filename_ext (filename);
  
  return !strcmp (temp, ext);
  
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
