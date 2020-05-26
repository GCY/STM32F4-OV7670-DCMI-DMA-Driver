/**
  @page IWDG_Example  IWDG Example

  @verbatim
  ******************** (C) COPYRIGHT 2013 STMicroelectronics *******************
  * @file    IWDG_Example/readme.txt 
  * @version V1.0.1
  * @date    11-November-2013
  * @date    28-February-2013
  * @brief   Description of the IWDG Example.
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

  This example shows how to update at regular period the IWDG reload counter and 
  how to simulate a software fault generating an MCU IWDG reset on expiry of a 
  programmed time period.

  The IWDG timeout is set to 250 ms (the timeout may vary due to LSI frequency 
  dispersion).

  First, the TIM5 timer is configured to measure the LSI frequency as the 
  LSI is internally connected to TIM5 CH4, in order to adjust the IWDG clock.

  The LSI measurement using the TIM5 is described below:
   - Configure the TIM5 to remap internally the TIM5 CH4 Input Capture to the LSI
     clock output.
   - Enable the TIM5 Input Capture interrupt: after one cycle of LSI clock, the
     period value is stored in a variable and compared to the HCLK clock to get
     its real value. 

  Then, the IWDG reload counter is configured as below to obtain 250 ms according 
  to the measured LSI frequency after setting the prescaler value:
  
       IWDG counter clock Frequency = LSI Frequency / Prescaler value

  The IWDG reload counter is refreshed each 240 ms in the main program infinite 
  loop to prevent a IWDG reset.
  
  LED4 is also toggled each 240 ms indicating that the program is running.

  An EXTI Line is connected to a GPIO pin, and configured to generate an interrupt
  on the rising edge of the signal.

  The EXTI Line is used to simulate a software failure: once the EXTI Line event 
  occurs, by pressing the user push-button, the corresponding interrupt is 
  served. In the ISR, a write to invalid address generates a Hardfault exception 
  containing an infinite loop and preventing to return to main program (the IWDG 
  reload counter is not refreshed).
  As a result, when the IWDG counter reaches 00h, the IWDG reset occurs.
  
  If the IWDG reset is generated, after the system resumes from reset, LED3 turns on.
  If the EXTI Line event does not occur, the IWDG counter is indefinitely refreshed
  in the main program infinite loop, and there is no IWDG reset.


@par Directory contents 

  - IWDG_Example/system_stm32f4xx.c   STM32F4xx system clock configuration file
  - IWDG_Example/stm32f4xx_conf.h     Library Configuration file
  - IWDG_Example/stm32f4xx_it.h       Interrupt handlers header file
  - IWDG_Example/stm32f4xx_it.c       Interrupt handlers
  - IWDG_Example/main.c               Main program
  - IWDG_Example/main.h               Main program header file


@par Hardware and Software environment 

  - This example runs on STM32F429x Devices RevY.
  
  - This example has been tested with STM32F429I-DISCO (MB1075) RevB and can be
    easily tailored to any other development board.

@par How to use it ? 

In order to make the program work, you must do the following :

  + EWARM
    - Open the IWDG_Example.eww workspace 
    - Rebuild all files: Project->Rebuild all
    - Load project image: Project->Debug
    - Run program: Debug->Go(F5)
  
 + MDK-ARM
    - Open the IWDG_Example.uvproj project
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
