/**
  @page SysTick_Example  SysTick configuration Example

  @verbatim
  ******************** (C) COPYRIGHT 2013 STMicroelectronics *******************
  * @file    SysTick_Example/readme.txt 
  * @version V1.0.1
  * @date    11-November-2013
  * @brief   Description of the SysTick configuration Example
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

  This example shows how to configure the SysTick to generate a time base equal to
  1 ms. The system clock is set to 180 MHz, the SysTick is clocked by the AHB clock
  (HCLK).

  A "Delay" function is implemented based on the SysTick end-of-count event.
  Two LEDs are toggled with a timing defined by the Delay function.


@par Directory contents 

  - SysTick_Example/system_stm32f4xx.c   STM32F4xx system clock configuration file
  - SysTick_Example/stm32f4xx_conf.h     Library Configuration file
  - SysTick_Example/stm32f4xx_it.h       Interrupt handlers header file
  - SysTick_Example/stm32f4xx_it.c       Interrupt handlers
  - SysTick_Example/main.c               Main program
  - SysTick_Example/main.h               Main program header file


@par Hardware and Software environment 

  - This example runs on STM32F429x Devices RevY.
  
  - This example has been tested with STM32F429I-DISCO (MB1075) RevB and can be
    easily tailored to any other development board.

@par How to use it ? 

In order to make the program work, you must do the following :
    
 + EWARM
    - Open the SysTick_Example.eww workspace 
    - Rebuild all files: Project->Rebuild all
    - Load project image: Project->Debug
    - Run program: Debug->Go(F5)
  
 + MDK-ARM
    - Open the SysTick_Example.uvproj project
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
