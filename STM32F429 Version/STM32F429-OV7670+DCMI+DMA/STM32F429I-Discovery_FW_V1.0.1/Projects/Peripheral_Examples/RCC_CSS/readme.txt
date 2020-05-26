/**
  @page RCC_CSS RCC example

  @verbatim
  ******************** (C) COPYRIGHT 2013 STMicroelectronics *******************
  * @file    RCC_CSS/readme.txt 
  * @version V1.0.1
  * @date    11-November-2013
  * @brief   Description of the RCC example.
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

  This example shows how to use, for debug purpose, the RCC firmware to retrieve 
  the current status and frequencies of different on chip clocks. 
  You can see the "RCC_ClockFreq" structure content, which hold the frequencies of 
  different on chip clocks, using your toolchain debugger.

  This example handles also the High Speed External clock (HSE) failure detection:
  when the HSE clock disappears (remove resistance on SB18); HSE, PLL
  are disabled (but no change on PLL config), HSI selected as system clock source
  and an interrupt (NMI) is generated. In the NMI ISR, the HSE, HSE ready interrupts
  are enabled, and once HSE clock recovers, the HSERDY interrupt is generated and in
  the RCC ISR routine the system clock is reconfigured to its previous state (before
  HSE clock failure). You can monitor the HSE clock on MCO1 pin(PA8).

  LED3 and LED4 are toggled with a timing defined by the Delay function.

@note On the STM32F429I-DISCO boards, to generate the HSE failure you can 
      remove the HSE quartz from the socket.


@par Directory contents 

  - RCC_CSS/system_stm32f4xx.c   STM32F4xx system clock configuration file
  - RCC_CSS/stm32f4xx_conf.h     Library Configuration file
  - RCC_CSS/stm32f4xx_it.h       Interrupt handlers header file
  - RCC_CSS/stm32f4xx_it.c       Interrupt handlers
  - RCC_CSS/main.c               Main program
  - RCC_CSS/main.h               Main program header file


@par Hardware and Software environment 

  - This example runs on STM32F429x Devices RevY.
  
  - This example has been tested with STM32F429I-DISCO (MB1075) RevB and can be
    easily tailored to any other development board.

@par How to use it ? 

In order to make the program work, you must do the following :
   
 + EWARM
    - Open the RCC_CSS.eww workspace 
    - Rebuild all files: Project->Rebuild all
    - Load project image: Project->Debug
    - Run program: Debug->Go(F5)
  
 + MDK-ARM
    - Open the RCC_CSS.uvproj project
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
