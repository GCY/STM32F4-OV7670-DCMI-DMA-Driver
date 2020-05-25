/**
  ******************************************************************************
  * @file    FW_upgrade/src/command.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    11-November-2013
  * @brief   This file provides all the IAP command functions.
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
#include "command.h"

/** @addtogroup STM32F429I-Discovery_FW_Upgrade
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
#define UPLOAD_FILENAME            "0:UPLOAD.BIN"
#define DOWNLOAD_FILENAME          "0:image.BIN"

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint8_t RAM_Buf[BUFFER_SIZE] =
  {
    0x00
  };
static uint32_t TmpProgramCounter = 0x00, TmpReadSize = 0x00 , RamAddress = 0x00;
static uint32_t LastPGAddress = APPLICATION_ADDRESS;

extern FATFS fatfs;
extern FIL file;
extern FIL fileR;
extern DIR dir;
extern FILINFO fno;

extern USB_OTG_CORE_HANDLE          USB_OTG_Core;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  IAP Read all flash memory.
  * @param  None
  * @retval None
  */
void COMMAND_UPLOAD(void)
{
  __IO uint32_t address = APPLICATION_ADDRESS;
  __IO uint32_t counterread = 0x00;
  
  uint32_t tmpcounter = 0x00, indexoffset = 0x00;
  FlagStatus readoutstatus = SET;
  uint16_t bytesWritten;
  
  /* Get the read out protection status */
  readoutstatus = FLASH_If_ReadOutProtectionStatus();
  if (readoutstatus == RESET)
  {
    /* Remove UPLOAD file if exist on flash disk */
    f_unlink (UPLOAD_FILENAME);
    
    /* Init written byte counter */
    indexoffset = (APPLICATION_ADDRESS - USER_FLASH_STARTADDRESS);
    
    /* Open binary file to write on it */
    if ((HCD_IsDeviceConnected(&USB_OTG_Core) == 1) && (f_open(&file, UPLOAD_FILENAME, FA_CREATE_ALWAYS | FA_WRITE) == FR_OK))
    {  
      /* Read flash memory */
      while ((indexoffset != USER_FLASH_SIZE) && (HCD_IsDeviceConnected(&USB_OTG_Core) == 1))
      {
        for (counterread = 0; counterread < BUFFER_SIZE; counterread++)
        {
          /* Check the read bytes versus the end of flash */
          if (indexoffset + counterread != USER_FLASH_SIZE)
          {
            tmpcounter = counterread;
            RAM_Buf[tmpcounter] = (*(uint8_t*)(address++));
          }
          /* In this case all flash was read */
          else
          {
            break;
          }
        }

        /* Write buffer to file */
        f_write (&file, RAM_Buf, BUFFER_SIZE, (void *)&bytesWritten);
        
        /* Number of byte written  */
        indexoffset = indexoffset + counterread;
      }
      
      /* Set Green LED ON: Upload Done */ 
      STM_EVAL_LEDOn(LED3); 
      
      /* Close file and filesystem */
      f_close (&file);
      f_mount(0, NULL);
    }
  }
  else
  {
    /* Message ROP active: Toggle Red LED in infinite loop */
    Fail_Handler();
  }
}

/**
  * @brief  IAP write memory.
  * @param  None
  * @retval None
  */
void COMMAND_DOWNLOAD(void)
{
  /* Open the binary file to be downloaded */
  if (f_open(&fileR, DOWNLOAD_FILENAME, FA_READ) == FR_OK)
  {
    if (fileR.fsize > USER_FLASH_SIZE)
    {
      /* Toggle Red LED in infinite loop: No available Flash memory size for 
         the binary file */
      Fail_Handler();
    }
    else
    {  
      /* Erase FLASH sectors to download image */
      if (FLASH_If_EraseSectors(APPLICATION_ADDRESS) != 0x00)
      {
        /* Toggle Red LED in infinite loop: Flash erase error */
        Fail_Handler();
      }
      
      STM_EVAL_LEDOff(LED3);
      /* Program flash memory */
      COMMAND_ProgramFlashMemory();
      
      /* Set Green LED ON: Download Done */
      STM_EVAL_LEDOn(LED3); 

      /* Close file and filesystem */
      f_close (&fileR);
    }
  }
  else
  {
    /* Toggle Red LED in infinite loop: the binary file is not available */
    Fail_Handler();
  }
}

/**
  * @brief  IAP jump to user program.
  * @param  None
  * @retval None
  */
void COMMAND_JUMP(void)
{
  /* Software reset */
  NVIC_SystemReset();
}

/**
  * @brief  Programs the internal Flash memory. 
  * @param  None
  * @retval None
  */
void COMMAND_ProgramFlashMemory(void)
{
  __IO uint32_t programcounter = 0x00;
  uint8_t readflag = TRUE;
  uint16_t BytesRead;
  
  /* RAM Address Initialization */
  RamAddress = (uint32_t) & RAM_Buf;
  
  /* Erase address init */
  LastPGAddress = APPLICATION_ADDRESS;
  
  /* While file still contain data */
  while ((readflag == TRUE) && (HCD_IsDeviceConnected(&USB_OTG_Core) == 1))
  {
    /* Read maximum 512 Kbyte from the selected file */
    f_read (&fileR, RAM_Buf, BUFFER_SIZE, (void *)&BytesRead);
    
    /* Temp variable */
    TmpReadSize = BytesRead;
    
    /* The read data < "BUFFER_SIZE" Kbyte */
    if (TmpReadSize < BUFFER_SIZE)
    {
      readflag = FALSE;
    }
    
    /* Program flash memory */
    for (programcounter = TmpReadSize; programcounter != 0; programcounter -= 4)
    {
      TmpProgramCounter = programcounter;
      /* Write word into flash memory */
      if (FLASH_If_ProgramWord((LastPGAddress - TmpProgramCounter + TmpReadSize), \
        *(__IO uint32_t *)(RamAddress - programcounter + TmpReadSize)) != FLASH_COMPLETE)
      {
        /* Toggle Red LED in infinite loop: Flash programming error */
        Fail_Handler();
      }
    }
    /* Update last programmed address value */
    LastPGAddress = LastPGAddress + TmpReadSize;
  }
}

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
