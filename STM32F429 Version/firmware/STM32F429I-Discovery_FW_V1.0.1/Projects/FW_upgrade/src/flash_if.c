/**
  ******************************************************************************
  * @file    FW_upgrade/src/flash_if.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    11-November-2013
  * @brief   This file provides all the flash_layer functions.
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
#include "flash_if.h"
#include "usbh_usr.h"

/** @addtogroup STM32F429I-Discovery_FW_Upgrade
  * @{
  */

/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static uint32_t FLASH_If_GetSectorNumber(uint32_t Address);

extern USB_OTG_CORE_HANDLE          USB_OTG_Core;

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Unlocks the FLASH Program Erase Controller.
  * @note   This function can be used for all STM32F10x devices.
  *         - For STM32F10X_XL devices this function unlocks Bank1 and Bank2.
  *         - For all other devices it unlocks Bank1 and it is equivalent
  *           to FLASH_UnlockBank1 function..
  * @param  None
  * @retval None
  */
void FLASH_If_FlashUnlock(void)
{
  FLASH_Unlock();
}

/**
  * @brief Get readout protection status
  * @param  None
  * @retval FLASH ReadOut Protection Status(SET or RESET)
  */
FlagStatus FLASH_If_ReadOutProtectionStatus(void)
{
  FlagStatus readoutstatus = RESET;
  if (FLASH_OB_GetRDP() == SET)
  {
    readoutstatus = SET;
  }
  else
  {
    readoutstatus = RESET;
  }
  return readoutstatus;
}

/**
  * @brief  Erases the required FLASH Sectors.
  * @retval Sectors erase status: 
  *  0: Erase sectors done with success
  *  1: Erase error
  */
uint32_t FLASH_If_EraseSectors(uint32_t Address)
{
  uint32_t sectorindex, startsector = 0x00;
  FLASH_Status erasestatus = FLASH_COMPLETE;
  
  /* Erase Flash sectors */
  startsector = FLASH_If_GetSectorNumber(Address);
  
  sectorindex = startsector;
  
  /* Erase FLASH sectors to download image */
  while ((erasestatus == FLASH_COMPLETE) && (sectorindex <= FLASH_Sector_11) && (HCD_IsDeviceConnected(&USB_OTG_Core) == 1))
  {
    erasestatus = FLASH_EraseSector(sectorindex, VoltageRange_3);
    sectorindex += 8;
  }
  
  if ((sectorindex != (FLASH_Sector_11 + 8)) || (erasestatus != FLASH_COMPLETE))
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

/**
  * @brief  Programs a word at a specified address.
  * @param  Address: specifies the address to be programmed.
  * @param  Data: specifies the data to be programmed.
  * @retval FLASH Status: The returned value can be: FLASH_ERROR_PG,
  *   FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_If_ProgramWord(uint32_t Address, uint32_t Data)
{
  FLASH_Status status = FLASH_COMPLETE;

  status = FLASH_ProgramWord(Address, Data);
  return status;
}

/**
  * @brief  Return the Flash sector Number of the address
  * @param  None.
  * @retval The Flash sector Number of the address
  */
static uint32_t FLASH_If_GetSectorNumber(uint32_t Address)
{
  uint32_t sector = 0;
  
  if(Address < ADDR_FLASH_SECTOR_1 && Address >= ADDR_FLASH_SECTOR_0)
  {
    sector = FLASH_Sector_0;  
  }
  else if(Address < ADDR_FLASH_SECTOR_2 && Address >= ADDR_FLASH_SECTOR_1)
  {
    sector = FLASH_Sector_1;  
  }
  else if(Address < ADDR_FLASH_SECTOR_3 && Address >= ADDR_FLASH_SECTOR_2)
  {
    sector = FLASH_Sector_2;  
  }
  else if(Address < ADDR_FLASH_SECTOR_4 && Address >= ADDR_FLASH_SECTOR_3)
  {
    sector = FLASH_Sector_3;  
  }
  else if(Address < ADDR_FLASH_SECTOR_5 && Address >= ADDR_FLASH_SECTOR_4)
  {
    sector = FLASH_Sector_4;  
  }
  else if(Address < ADDR_FLASH_SECTOR_6 && Address >= ADDR_FLASH_SECTOR_5)
  {
    sector = FLASH_Sector_5;  
  }
  else if(Address < ADDR_FLASH_SECTOR_7 && Address >= ADDR_FLASH_SECTOR_6)
  {
    sector = FLASH_Sector_6;  
  }
  else if(Address < ADDR_FLASH_SECTOR_8 && Address >= ADDR_FLASH_SECTOR_7)
  {
    sector = FLASH_Sector_7;  
  }
  else if(Address < ADDR_FLASH_SECTOR_9 && Address >= ADDR_FLASH_SECTOR_8)
  {
    sector = FLASH_Sector_8;  
  }
  else if(Address < ADDR_FLASH_SECTOR_10 && Address >= ADDR_FLASH_SECTOR_9)
  {
    sector = FLASH_Sector_9;  
  }
  else if(Address < ADDR_FLASH_SECTOR_11 && Address >= ADDR_FLASH_SECTOR_10)
  {
    sector = FLASH_Sector_10;  
  }
  else/*(Address < FLASH_END_ADDR && Address >= ADDR_FLASH_SECTOR_11)*/
  {
    sector = FLASH_Sector_11;  
  }
    return sector;
}

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
