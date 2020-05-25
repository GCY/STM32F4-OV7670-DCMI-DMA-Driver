/**
  ******************************************************************************
  * @file    fatfs_drv.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    11-November-2013
  * @brief   FatFS functions
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "usbh_msc_core.h"
#include "usbh_usr.h"
#include "diskio.h"
#include "global_includes.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define USE_FS_RTC                 1
#define BLOCK_SIZE                 512 /* Block Size in Bytes */
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static volatile DSTATUS Stat = STA_NOINIT;	/* Disk status */
BYTE                                status = USBH_MSC_OK;
extern USB_OTG_CORE_HANDLE          USB_OTG_Core;
extern USBH_HOST                    USB_Host;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/**
  * @brief  Initialize Disk Drive  
  * @param  drv : driver index
  * @retval DSTATUS : operation status
  */
DSTATUS disk_initialize ( BYTE drv )
{

  Stat = STA_NOINIT;

  switch (drv)
  {
  case 0 :

    if(USB_Host_Application_Ready)
    {
      Stat &= ~STA_NOINIT;
    }
    break;
  }
  return Stat;
}

/**
   * @brief  Get Disk Status  
   * @param  drv : driver index
   * @retval DSTATUS : operation status
  */
DSTATUS disk_status ( BYTE drv)
{
  Stat = STA_NOINIT;
  
  switch (drv)
  {
  case 0 :

    if ((status == USBH_MSC_OK) && (HCD_IsDeviceConnected(&USB_OTG_Core)))
    {
      Stat &= ~STA_NOINIT;
    }
    break;

  }
  return Stat;
}

/**
  * @brief  Read Sector(s)
  * @param  drv : driver index
  * @param  buff : Pointer to the data buffer to store read data
  * @param  sector : Start sector number
  * param   count : Sector count (1..255)
  * @retval DSTATUS : operation status
  */
DRESULT disk_read (
                   BYTE drv,			/* Physical drive number (0) */
                   BYTE *buff,			/* Pointer to the data buffer to store read data */
                   DWORD sector,		/* Start sector number (LBA) */
                   BYTE count			/* Sector count (1..255) */
                     )
{  
  switch (drv)
  {
  case 0 :
    {
        if(USB_Host_Application_Ready)
        {
          do
          {
            status = USBH_MSC_Read10(&USB_OTG_Core, (uint8_t *)buff,sector,BLOCK_SIZE * count);
            USBH_MSC_HandleBOTXfer(&USB_OTG_Core ,&USB_Host);

            if(!HCD_IsDeviceConnected(&USB_OTG_Core))
            {
              return RES_ERROR;
            }
          }
          while(status == USBH_MSC_BUSY );

          if(status == USBH_MSC_OK)
          {
            return RES_OK;
          }
        }

        break;
      }
    }
    return RES_NOTRDY;
}
/**
  * @brief  write Sector(s) 
  * @param  drv : driver index
  * @param  buff : Pointer to the data to be written
  * @param  sector : Start sector number
  * @param  count :  Sector count (1..255)
  * @retval DSTATUS : operation status
  */

#if _READONLY == 0
DRESULT disk_write (
                    BYTE drv,			/* Physical drive number (0) */
                    const BYTE *buff,	/* Pointer to the data to be written */
                    DWORD sector,		/* Start sector number (LBA) */
                    BYTE count			/* Sector count (1..255) */
                      )
{

  switch (drv)
  {
  case 0 :
    {
      if(USB_Host_Application_Ready)
      {
        do
        {
          status = USBH_MSC_Write10(&USB_OTG_Core, (uint8_t *)buff, sector, BLOCK_SIZE * count);
          USBH_MSC_HandleBOTXfer(&USB_OTG_Core ,&USB_Host);

          if(!HCD_IsDeviceConnected(&USB_OTG_Core))
          {
            return RES_ERROR;
          }
        }
        while(status == USBH_MSC_BUSY );

      }

      if(status == USBH_MSC_OK)
      { 
        return RES_OK;
      }

      break;
    }
  }
  return RES_NOTRDY;
}
#endif /* _READONLY == 0 */


/**
  * @brief  I/O control operation
  * @param  drv : driver index
  * @param  ctrl : ontrol code
  * @param  buff : Buffer to send/receive control data
  * @retval DSTATUS : operation status
  */


#if _USE_IOCTL != 0
DRESULT disk_ioctl (
                    BYTE drv,/* Physical drive number (0) */
                    BYTE ctrl,/* Control code */
                    void *buff/* Buffer to send/receive control data */
                      )
{
  DRESULT res;

  if (drv >= _VOLUMES) return RES_PARERR;

  res = RES_ERROR;

  if (Stat & STA_NOINIT) return RES_NOTRDY;

  switch (ctrl) {
  case CTRL_SYNC :		/* Make sure that no pending write process */
    res = RES_OK;
    break;

  case GET_SECTOR_COUNT :	/* Get number of sectors on the disk (DWORD) */
    if(drv == 0)
    {
      *(DWORD*)buff = (DWORD) USBH_MSC_Param.MSCapacity;
    }
    res = RES_OK;
    break;

  case GET_SECTOR_SIZE :	/* Get R/W sector size (WORD) */
    *(WORD*)buff = BLOCK_SIZE;
    res = RES_OK;
    break;

  case GET_BLOCK_SIZE :	/* Get erase block size in unit of sector (DWORD) */
    if(drv == 0)
    {
      *(DWORD*)buff = BLOCK_SIZE;
    }
    else
    {
      *(DWORD*)buff = 32;
    }


    break;


  default:
    res = RES_PARERR;
  }



  return res;
}
#endif /* _USE_IOCTL != 0 */
/**
  * @brief  Get Time from RTC
  * @param  None
  * @retval Time in DWORD
  */

DWORD get_fattime (void)
{

  DWORD time = 0;
#if !defined (STM32F10X_CL)    
#ifdef USE_FS_RTC
  RTC_TimeTypeDef   RTC_TimeStructure;
  RTC_DateTypeDef   RTC_DateStructure;

  BYTE rtcYear , rtcMon , rtcMday, rtcHour, rtcMin, rtcSec;

  /* Get info from RTC here */
  RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);

  rtcSec    =  RTC_TimeStructure.RTC_Seconds;
  rtcMin    =  RTC_TimeStructure.RTC_Minutes;
  rtcHour   =  RTC_TimeStructure.RTC_Hours;

  RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);

  rtcYear =  RTC_DateStructure.RTC_Year;
  rtcMon =  RTC_DateStructure.RTC_Month;
  rtcMday =  RTC_DateStructure.RTC_Date;

  /* Pack date and time into a DWORD variable */
  time =	  (((DWORD)rtcYear) << 25)
                 | ((DWORD)rtcMon << 21)
                 | ((DWORD)rtcMday << 16)
                 | (WORD)(rtcHour << 11)
                 | (WORD)(rtcMin << 5)
                 | (WORD)(rtcSec >> 1);
#endif
#endif  
  return time;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
