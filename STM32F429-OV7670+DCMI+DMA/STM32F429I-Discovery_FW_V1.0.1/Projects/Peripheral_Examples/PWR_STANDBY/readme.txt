/**
  @page PWR_Standby PWR_STANDBY example

  @verbatim
  ******************** (C) COPYRIGHT 2013 STMicroelectronics *******************
  * @file    PWR_Standby/readme.txt 
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

@par PWR_Standby Description 

This example shows how to enters the system to STANDBY mode and wake-up from this
mode using: external RESET, RTC Alarm A or WKUP pin.

In the associated software, the system clock is set to 180 MHz, an EXTI line
is configured to generate an interrupt on falling edge and the SysTick is programmed
to generate an interrupt each 250 ms. In the SysTick interrupt handler, the LED3 is
toggled, this is used to indicate whether the MCU is in STANDBY or RUN mode.

When a falling edge is detected on the EXTI line an interrupt is generated. In the 
EXTI handler routine the RTC is configured to generate an Alarm event in 5 second
then the system enters STANDBY mode causing the LED3 to stop toggling. 
A rising edge on WKUP pin or an external RESET will wake-up the system from
STANDBY. If within 5 second neither rising edge on WKUP pin nor external RESET
are generated, the RTC Alarm A will wake-up the system. 

After wake-up from STANDBY mode, program execution restarts in the same way as after
a RESET, the RTC configuration (clock source, prescaler,...) is kept and LED3 restarts
toggling. As result there is no need to configure the RTC.

two leds LED3 and LED4 are used to monitor the system state as following:
 - LED4 ON: RTC configuration failed (system will go to an infinite loop)
 - LED3 toggling: system in RUN mode
 - LED3 off : system in STANDBY mode

These Steps are repeated in an infinite loop.

@note To measure the current consumption in STANDBY mode, please refer to 
      @subpage PWR_CurrentConsumption example.

@note This example can not be used in DEBUG mode, this is due to the fact 
      that the Cortex-M4 core is no longer clocked during low power mode 
      so debugging features are disabled

@par Directory contents 

  - PWR_Standby/system_stm32f4xx.c   STM32F4xx system clock configuration file
  - PWR_Standby/stm32f4xx_conf.h     Library Configuration file
  - PWR_Standby/stm32f4xx_it.c       Interrupt handlers
  - PWR_Standby/stm32f4xx_it.h       Header for stm32f4xx_it.c
  - PWR_Standby/main.c               Main program
  - PWR_Standby/main.h               Header for main.c



@par Hardware and Software environment 

  - This example runs on STM32F429x Devices RevY.
  - Use the user-push button connected to pin PA0 (EXTI Line0)
  - This example has been tested with STM32F429I-DISCO (MB1075) RevB and can be
    easily tailored to any other development board.

@par How to use it ? 

In order to make the program work, you must do the following :

 + EWARM
    - Open the PWR_Standby.eww workspace 
    - Rebuild all files: Project->Rebuild all
    - Load project image: Project->Debug
    - Run program: Debug->Go(F5)
  
 + MDK-ARM
    - Open the PWR_Standby.uvproj project
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
