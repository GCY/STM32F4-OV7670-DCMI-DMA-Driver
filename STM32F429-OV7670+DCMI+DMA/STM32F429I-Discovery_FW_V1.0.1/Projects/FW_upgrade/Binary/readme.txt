/**
  @page FW_upgrade_Binary   Description of the "Upgrading STM32F429I-DISCO board firmware using a USB key" application's binary files

  @verbatim
  ******************** (C) COPYRIGHT 2013 STMicroelectronics *******************
  * @file    FW_upgrade/Binary/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    11-November-2013
  * @brief   Description of the "Upgrading STM32F429I DISCOVERY board firmware using a USB key"
  *          application's binary files.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
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
  @endverbatim

@par Description

Binary images of the user program
=================================
This folder contains the following images that can be loaded and executed by the 
FW upgrade application:  
    - STM32F429I-Discovery_LTDC_ColorKeying_0x08020000.bin
    - STM32F429I-Discovery_SysTick_0x08020000.bin

You have to load these binaries to the root directory of a USB key, then rename 
the binary to be executed to "image.bin".

After loading the program into  internal flash, the green LED (LED3) will be ON
indicating that the image.bin is well download into the flash. Push the user
button to start one of the examples below:
1- LTDC_ColorKeying: Is an example showing the color keying capability of LTDC.
                     Push the user button to see the effect on LCD.
2- SysTick example: Is an example to toggle Leds based on frequency of SysTick. 

Please refer to AN3990 which contains an overview of the firmware upgrade process 
and demonstrates how to run the firmware upgrade for STM32F4 discovery. the same 
process is used for STM32F429I discovery.
 
@par Hardware and Software environment  

  - This example runs on STM32F429xx Devices.
    
  - This example has been tested with STMicroelectronics STM32F429I-DISCO (MB1075B) 
    discovery boards and can be easily tailored to any other supported device and 
    development board.

@par Known limitations

  - For some USB Disk flashs, in the Full speed mode, the USB clock accuracy is 
    not in line with the clock variance recommended in the USB 2.0 specification.  
    Thus, the USB OTG FS core may not work properly on these devices. 
    It is recommended to use certified USB disks. 
 
@par How to use it ?
In order to make the program work, you must do the following :

 + EWARM
    - Open the STM32F429I-Discovery_FW_Upgrade.eww workspace 
    - Rebuild all files: Project->Rebuild all
    - Load project image: Project->Debug
    - Run program: Debug->Go(F5)
  
 + MDK-ARM
    - Open the STM32F429I-Discovery_FW_Upgrade.uvproj project
    - Rebuild all files: Project->Rebuild all target files
    - Load project image: Debug->Start/Stop Debug Session
    - Run program: Debug->Run (F5)  
    
 + TrueSTUDO
    - Open the TrueSTUDIO toolchain.
    - Click on File->Switch Workspace->Other and browse to TrueSTUDIO workspace directory.
    - Click on File->Import, select General->'Existing Projects into Workspace' and then click "Next". 
    - Browse to the TrueSTUDIO workspace directory, select the project.
    - Rebuild all project files: Select the project in the "Project explorer" 
      window then click on Project->build project menu.
    - Run program: Run->Debug (F11) 

For more information, please refer to the document "Upgrading STM32F4DISCOVERY board
 firmware using a USB key (AN3990)" available on www.st.com/stm32f4-discovery. 
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
