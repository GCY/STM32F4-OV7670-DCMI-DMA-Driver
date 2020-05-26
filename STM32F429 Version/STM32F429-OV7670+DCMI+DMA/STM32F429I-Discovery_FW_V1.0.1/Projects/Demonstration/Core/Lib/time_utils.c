/**
  ******************************************************************************
  * @file    time_utils.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    11-November-2013
  * @brief   Utilities for time calculation 
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
#include "time_utils.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t  MonLen[12]= {32, 29, 32, 31, 32, 31, 32, 32, 31, 32, 31, 32};
uint8_t strMonth[][12] = {"January",  "February",  "March",  "April",  "May",  
"June",  "July",  "August",  "September",  "October", "November",  "December"};
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Check whether the passed year is Leap or not.
  * @param  nYear : year to be checked
* @retval   Bool: check result (1: True, 0 False) 
  */
uint8_t IsLeapYear(uint16_t nYear)
{
  if(nYear % 4 != 0) return 0;
  if(nYear % 100 != 0) return 1;
  return (u8)(nYear % 400 == 0);
}

/**
  * @brief  Get days number in a month
  * @param  nMonth : current month
  * @param  nYear : current year
  * @retval days in a month
  */
uint8_t GetMaxDays(uint8_t nMonth, uint16_t nYear)
{
  if(nMonth == 2)
  {
    if(IsLeapYear(nYear))
    {
      return  29;
    }
    else
    {
      return  (MonLen[nMonth - 1] - 1);
    }
  }
  else
  {
    return (MonLen[nMonth - 1] - 1);
  }
}


/**
  * @brief  Determines the days of the month and the day offset in the the week
  * @param  nday : current day
  * @param  nmonth : current month
  * @param  nmonth : current year
  * @param  offset : day offset in the week
  * @param  max : number of days in the month 
  * @retval None
  */
void GetDateOffset(uint32_t nyear, uint8_t nmonth,uint8_t *offset , uint8_t *max)
{
  uint32_t a = 0, b = 0, c = 0, s = 0, e = 0, f = 0, g = 0, d = 0;
 
  /* Calculate max */
  
  if(nmonth == 2)
  {
    if(IsLeapYear(nyear))
    {
      *max = 29;
    }
    else
    {
      *max = (MonLen[nmonth - 1] - 1);
    }
  }
  else
  {
    *max = (MonLen[nmonth - 1] - 1);
  } 
  
  if(nmonth < 3)
  {
    a = nyear - 1;
  }
  else
  {
    a = nyear;
  }
  
  b = (a/4) - (a/100) + (a/400);
  c = ((a - 1)/4) - ((a - 1)/100) + ((a - 1)/400);
  s = b - c;

  if(nmonth < 3)
  {
    e = 0;
    f =  31 * (nmonth - 1);
  }
  else
  {
    e = s + 1;
    f = 1 + (153*(nmonth - 3) + 2)/5 + 58 + s; 
  }

  g = (a + b) % 7;
  d = (f + g - e) % 7;

  *offset = d;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
