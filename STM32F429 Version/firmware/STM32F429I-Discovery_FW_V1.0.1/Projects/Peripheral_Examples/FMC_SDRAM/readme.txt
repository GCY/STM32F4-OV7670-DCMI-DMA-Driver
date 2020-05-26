/**
  @page FMC_SDRAM FMC SDRAM example
  
  @verbatim
  ******************** (C) COPYRIGHT 2013 STMicroelectronics *******************
  * @file    FMC_SDRAM/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    11-November-2013
  * @brief   Description of the FMC SDRAM example.
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

@par FMC_SDRAM Description

This example shows how to write to the external SDRAM with 8bits AHB transaction
or 16bits AHB transaction.
After SDRAM initialization, SDRAM GPIO configuration and write protection
disabling, a write to external SDRAM is possible.

First, an erase is done byte by byte to all SDRAM. Then a write is performed. To
check the operation is well done, a read operation is performed. Then a comparison
between the written byte and the read one is made.
If the result is correct, LED3 is on and the following message is displayed on
LCD: "8-bits AHB Transaction Test -> OK". Otherwise, LED4 is on and the following
message is displayed on LCD: "8-bits AHB Transaction Test -> NOT OK"

The same operation is done with 16bits AHB transaction.
If the result of comparison is correct, LED3 is on and the following message is
displayed on LCD: "16-bits AHB Transaction Test -> OK". Otherwise, LED4 is on
and the following message is displayed: "16-bits AHB Transaction Test -> NOT OK"

To start, click the user-push button so the 8bits transaction is performed. Click
again on user-push button to perform 16bits transaction. This can repeated
infinitely.


@par Directory contents 
  
  - FMC_SDRAM/stm32f4xx_conf.h      Library Configuration file
  - FMC_SDRAM/stm32f4xx_it.c        Interrupt handlers
  - FMC_SDRAM/stm32f4xx_it.h        Interrupt handlers header file
  - FMC_SDRAM/main.c                Main program
  - FMC_SDRAM/main.h                Header of main.c   
  - FMC_SDRAM/system_stm32f4xx.c    STM32F4xx system clock configuration file
   

@par Hardware and Software environment 

  - This example runs on STM32F429x Devices RevY.
    
  - This example has been tested with STM32F429I-DISCO (MB1075) RevB and can be
    easily tailored to any other development board.
    
  - STM32F429I-DISCO Set-up
    LCD, user-push button, LED3 and LED4 are used in this example.

@par How to use it ? 

In order to make the program work, you must do the following :

  + EWARM
    - Open the FMC_SDRAM.eww workspace 
    - Rebuild all files: Project->Rebuild all
    - Load project image: Project->Debug
    - Run program: Debug->Go(F5)
  
 + MDK-ARM
    - Open the FMC_SDRAM.uvproj project
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
