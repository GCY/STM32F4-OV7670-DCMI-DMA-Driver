/**
  @page RCC_ClockConfig RCC Clock Config example

  @verbatim
  ******************** (C) COPYRIGHT 2013 STMicroelectronics *******************
  * @file    RCC_ClockConfig/readme.txt 
  * @version V1.0.1
  * @date    11-November-2013
  * @brief   Description of the RCC Clock Config example.
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

  This example shows how to configure the System clock(SYSCLK) to have different
  frequencies: 48MHz, 84MHz and 180MHz (common frequencies that covers
  the major of the applications). It shows also how to use HSE as system clock. 
  The SYSCLK frequency is selected by user in main.h file. The HSE clock is selected
  by default:
  #define SYSCLK_HSE
//#define SYSCLK_FREQ_48MHz
//#define SYSCLK_FREQ_84MHz
//#define SYSCLK_FREQ_180MHz

  This example shows how to use, for debug purpose, the RCC firmware to retrieve 
  the current status and frequencies of different on chip clocks. 
  You can see the "RCC_ClockFreq" structure content, which hold the frequencies of 
  different on chip clocks, using your toolchain debugger.

  This example handles also the High Speed External clock (HSE) failure detection:
  when the HSE clock disappears (broken or disconnected external Quartz); HSE, PLL
  are disabled (but no change on PLL config), HSI selected as system clock source
  and an interrupt (NMI) is generated. In the NMI ISR, the HSE, HSE ready interrupts
  are enabled, and once HSE clock recovers, the HSERDY interrupt is generated and in
  the RCC ISR routine the system clock is reconfigured to its previous state (before
  HSE clock failure). You can monitor the HSE clock on MCO1 pin(PA8).

  LED3 and LED4 are toggled with a timing defined by the Delay function.

@note On the STM32F429I-DISCO boards, to generate the HSE failure you can 
      remove the HSE quartz from the socket.


@par Directory contents 

  - RCC_ClockConfig/system_stm32f4xx.c   STM32F4xx system clock configuration file
  - RCC_ClockConfig/stm32f4xx_conf.h     Library Configuration file
  - RCC_ClockConfig/stm32f4xx_it.h       Interrupt handlers header file
  - RCC_ClockConfig/stm32f4xx_it.c       Interrupt handlers
  - RCC_ClockConfig/main.c               Main program
  - RCC_ClockConfig/main.h               Main program header file


@par Hardware and Software environment 

  - This example runs on STM32F429x Devices RevY.
  
  - This example has been tested with STM32F429I-DISCO (MB1075) RevB and can be
    easily tailored to any other development board.
  - Connect PA8 pin to an oscilloscope to visualize the selected system clock 
    frequency.  

@par How to use it ? 

In order to make the program work, you must do the following :
   
 + EWARM
    - Open the RCC_ClockConfig.eww workspace 
    - Rebuild all files: Project->Rebuild all
    - Load project image: Project->Debug
    - Run program: Debug->Go(F5)
  
 + MDK-ARM
    - Open the RCC_ClockConfig.uvproj project
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
