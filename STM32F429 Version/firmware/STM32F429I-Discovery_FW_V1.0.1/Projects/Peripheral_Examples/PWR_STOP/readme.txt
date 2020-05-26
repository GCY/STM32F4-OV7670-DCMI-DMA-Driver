/**
  @page PWR_STOP PWR_STOP Example

  @verbatim
  ******************** (C) COPYRIGHT 2013 STMicroelectronics *******************
  * @file    PWR_STOP/readme.txt 
  * @version V1.0.1
  * @date    11-November-2013
  * @brief   Description of the PWR_STANDBY example
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

@par PWR_STOP Description 

This example shows how to enter the system to STOP mode and wake-up from this
mode using RTC Wakeup Timer Event connected to EXTI Line 22.

In the associated software
  - the system clock is set to 180 MHz
  - the EXTI_Line0 is configured to generate interrupt on falling edge
  - the EXTI_Line22 connected internally to the RTC Wakeup event is configured
    to generate an interrupt on rising edge each 4s
  - the SysTick is programmed to generate an interrupt each 250 ms 
In the SysTick interrupt handler, LED3 is toggled, this is used to indicate whether
the MCU is in STOP or RUN mode.

The system enters STOP mode and will wait for the RTC Wakeup event to be 
generated each 5s, or Key push button is pressed. 
 - If the RTC WakeUp event (EXTI_Line22) is the source of wakeup from STOP, LED4 is toggled.
 - If the Key button (EXTI_Line0) is the source of wakeup from STOP, LED4 is toggled.

This behavior is repeated in an infinite loop.

LEDs are used to monitor the system state as following:
 - LED3 toggling: system in RUN mode
 - LED4 toggled: system woken up from STOP using EXTI_Line0 (user-push button) or
                 system woken up from STOP using RTC WakeUp Interrupt.


These Steps are repeated in an infinite loop.

@note To measure the current consumption in STANDBY mode, please refer to 
      @subpage PWR_CurrentConsumption example.

@note This example can not be used in DEBUG mode, this is due to the fact 
      that the Cortex-M4 core is no longer clocked during low power mode 
      so debugging features are disabled

@par Directory contents 

  - PWR_STOP/system_stm32f4xx.c   STM32F4xx system clock configuration file
  - PWR_STOP/stm32f4xx_conf.h     Library Configuration file
  - PWR_STOP/stm32f4xx_it.c       Interrupt handlers
  - PWR_STOP/stm32f4xx_it.h       Header for stm32f4xx_it.c
  - PWR_STOP/main.c               Main program
  - PWR_STOP/main.h               Header for main.c



@par Hardware and Software environment 

  - This example runs on STM32F429x Devices RevY.
  - Use the user-push button connected to pin PA0 (EXTI Line0)
  - This example has been tested with STM32F429I-DISCO (MB1075) RevB and can be
    easily tailored to any other development board.

@par How to use it ? 

In order to make the program work, you must do the following :

 + EWARM
    - Open the PWR_STOP.eww workspace 
    - Rebuild all files: Project->Rebuild all
    - Load project image: Project->Debug
    - Run program: Debug->Go(F5)
  
 + MDK-ARM
    - Open the PWR_STOP.uvproj project
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
