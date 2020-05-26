/**
  @page FLASH_DualBoot  FLASH Dual Boot example

  @verbatim
  ******************** (C) COPYRIGHT 2013 STMicroelectronics *******************
  * @file    FLASH_DualBoot/readme.txt 
  * @version V1.0.1
  * @date    11-November-2013
  * @brief   Description of the FLASH Dual Boot example
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

@par Example Description 

This example provides a description of how to program bank1 and bank2 of the FLASH 
memory integrated within STM32F429I-DISCO Devices and swap between both of them.

Below are the steps to run this example:
1- Choose the FLASH_DualBoot_Bank2 project and generate its binary(ie: FLASH_DualBoot.bin)
2- Load this binary at the bank2 of the flash(at the address 0x08100000) using 
   STM32 ST-LINK Utilities(www.st.com) or any similar tool. 
3- Choose the FLASH_DualBoot_Bank1 project and run it, this project will be loaded
   in the bank1 of the flash: at the address 0x08000000
4- Click the user push-button to swap between the two banks

- If program in bank1 is selected, a message with a blue text back color will be 
  displayed on LCD and LED3 will remain toggling while LED4 is turn on.

- If program in bank2 is selected, a message with a red text back color will be 
  displayed on LCD and LED4 will remain toggling while LED3 is turn on.


@par Directory contents 

  - FLASH_DualBoot/system_stm32f4xx.c   STM32F4xx system clock configuration file
  - FLASH_DualBoot/stm32f4xx_conf.h     Library Configuration file
  - FLASH_DualBoot/stm32f4xx_it.h       Interrupt handlers header file
  - FLASH_DualBoot/stm32f4xx_it.c       Interrupt handlers
  - FLASH_DualBoot/main.c               Main program
  - FLASH_DualBoot/main.h               Main program header file


@par Hardware and Software environment 

  - This example runs on STM32F429x Devices RevY.
  
  - This example has been tested with STM32F429I-DISCO (MB1075) RevB and can be
    easily tailored to any other development board.

@par How to use it ? 

In order to make the program work, you must do the following :

    + EWARM
    - Open the FLASH_DualBoot.eww workspace 
    - Rebuild all files: Project->Rebuild all
    - Load project image: Project->Debug
    - Run program: Debug->Go(F5)
  
 + MDK-ARM
    - Open the FLASH_DualBoot.uvproj project
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
      
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
