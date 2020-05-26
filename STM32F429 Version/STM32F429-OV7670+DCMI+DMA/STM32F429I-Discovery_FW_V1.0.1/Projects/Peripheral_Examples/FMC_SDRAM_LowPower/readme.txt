/**
  @page FMC_SDRAM_LowPower SDRAM memory low power mode use (self refresh mode)
  
  @verbatim
  ******************** (C) COPYRIGHT 2013 STMicroelectronics *******************
  * @file    FMC_SDRAM_LowPower/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    11-November-2013
  * @brief   Description of the FMC SDRAM_LowPower example.
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

@par FMC_SDRAM_LowPower Description

  This example shows how to configure the FMC to drive the IS42S16400J-7TL SDRAM
  memory in low power mode (self refresh mode) mounted on STM32F429I-DISCO board.
            
  In this example, a simple application of the FMC SDRAM low power mode (self 
  refresh mode) while the MCU is in a low power mode (STOP mode). 
  
  The purpose is to allow the SDRAM to retain data written after entering STOP mode. 
  STOP mode is a CPU low power mode which stops all clocks in the 1.2 V domain, only 
  internal SRAM and registers content are preserved.
  
  After initialization, the data is written to the SDRAM memory before sending 
  a self refresh command to it. Once the data is written, the CPU enters in STOP 
  mode and LED3 is turned on. The wakeup from STOP mode is done when pushing wakeup 
  Button and the CPU returns to RUN mode. At this time, the system clock is reconfigured. 
  Finally, a normal mode command is send to SDRAM memory to exit self refresh mode. 
  The data written to SDRAM is read back and checked.  
   
  LED3 are used to indicate the system state as following:
  - LED3 and LED4 On  : CPU enters STOP mode.
  - LED3 On and LED4 Off : correct data transfer (PASS). 
  - LED4 On and LED3 Off : incorrect data transfer (FAIL).


@par Directory contents 
  
  - FMC_SDRAM_LowPower/stm32f4xx_conf.h      Library Configuration file
  - FMC_SDRAM_LowPower/stm32f4xx_it.c        Interrupt handlers
  - FMC_SDRAM_LowPower/stm32f4xx_it.h        Interrupt handlers header file
  - FMC_SDRAM_LowPower/main.c                Main program
  - FMC_SDRAM_LowPower/main.h                Header of main.c   
  - FMC_SDRAM_LowPower/system_stm32f4xx.c    STM32F4xx system clock configuration file
   

@par Hardware and Software environment 

  - This example runs on STM32F429x Devices RevY.
    
  - This example has been tested with STM32F429I-DISCO (MB1075) RevB and can be
    easily tailored to any other development board.

  - STM32F429I-DISCO Set-up
    user-push button, LED3 and LED4 are used in this example.

@par How to use it ? 

In order to make the program work, you must do the following :

  + EWARM
    - Open the FMC_SDRAM_LowPower.eww workspace 
    - Rebuild all files: Project->Rebuild all
    - Load project image: Project->Debug
    - Run program: Debug->Go(F5)
  
 + MDK-ARM
    - Open the FMC_SDRAM_LowPower.uvproj project
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
