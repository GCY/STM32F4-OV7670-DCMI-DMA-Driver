/**
  @page TIM_PWMOutput TIM_PWMOutput example
  
  @verbatim
  ******************** (C) COPYRIGHT 2013 STMicroelectronics *******************
  * @file    TIM_PWMOutput/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    11-November-2013
  * @brief   Description of the TIM PWM_Output example.
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

This example shows how to configure the TIM peripheral in PWM (Pulse Width Modulation) 
mode.

The TIM4 input clock (TIM4CLK) is set to 4 * APB1 clock (PCLK1), since TIMPRE bit 
from RCC_DCKCFGR register is set.

The APB1 clock (PCLK1) is set to HCLK / 4, since APB1 Prescaler is set to 4.

So the TIM4CLK frequency is set to SystemCoreClock, to get TIM4 counter
clock at 21 MHz the Prescaler is computed as following:
   - Prescaler = (TIM4CLK / TIM4 counter clock) - 1

SystemCoreClock is set to 180 MHz for STM32F4xx Devices.

The TIM4 is running at 30 KHz: TIM4 Frequency = TIM4 counter clock/(ARR + 1)
                                              = 21 MHz / 700 = 30 KHz
The TIM4 CCR1 register value is equal to 350, so the TIM4 Channel 1 generates a 
PWM signal with a frequency equal to 30 KHz and a duty cycle equal to 50%:
TIM4 Channel1 duty cycle = (TIM4_CCR1/ TIM4_ARR + 1)* 100 = 50%

The TIM4 CCR2 register value is equal to 262, so the TIM4 Channel 2 generates a 
PWM signal with a frequency equal to 30 KHz and a duty cycle equal to 37.5%:
TIM4 Channel2 duty cycle = (TIM4_CCR2/ TIM4_ARR + 1)* 100 = 37.5%

The TIM4 CCR3 register value is equal to 175, so the TIM4 Channel 3 generates a 
PWM signal with a frequency equal to 30 KHz and a duty cycle equal to 25%:
TIM4 Channel3 duty cycle = (TIM4_CCR3/ TIM4_ARR + 1)* 100 = 25%

The TIM4 CCR4 register value is equal to 87, so the TIM4 Channel 4 generates a 
PWM signal with a frequency equal to 30 KHz and a duty cycle equal to 12.5%:
TIM4 Channel4 duty cycle = (TIM4_CCR4/ TIM4_ARR + 1)* 100 = 12.5%

The PWM waveform can be displayed using an oscilloscope.


@par Directory contents 
  
  - TIM_PWMOutput/stm32f4xx_conf.h  Library Configuration file
  - TIM_PWMOutput/stm32f4xx_it.c    Interrupt handlers
  - TIM_PWMOutput/stm32f4xx_it.h    Interrupt handlers header file
  - TIM_PWMOutput/main.c            Main program 
  - TIM_PWMOutput/system_stm32f4xx.c   STM32F4xx system clock configuration file
   

@par Hardware and Software environment 

  - This example runs on STM32F429x Devices RevY.
    
  - This example has been tested with STM32F429I-DISCO (MB1075) RevB and can be
    easily tailored to any other development board.
    
  - STM32F429I-DISCO Set-up
    - Connect the following pins to an oscilloscope to monitor the different 
      waveforms:
        - PD.12: (TIM4_CH1)
        - PD.13: (TIM4_CH2)
        - PD.14: (TIM4_CH3)
        - PD.15: (TIM4_CH4)  

@par How to use it ? 

In order to make the program work, you must do the following :

 + EWARM
    - Open the TIM_PWMOutput.eww workspace 
    - Rebuild all files: Project->Rebuild all
    - Load project image: Project->Debug
    - Run program: Debug->Go(F5)
  
 + MDK-ARM
    - Open the TIM_PWMOutput.uvproj project
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
