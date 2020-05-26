/**
  @page ADC_DMA  ADC3 conversion using DMA for Data transfer

  @verbatim
  ******************** (C) COPYRIGHT 2013 STMicroelectronics *******************
  * @file    ADC_DMA/readme.txt 
  * @version V1.0.1
  * @date    11-November-2013
  * @brief   Description of the ADC DMA example.
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

This example describes how to use the ADC3 and DMA to transfer continuously 
converted data from ADC3 to memory.
The ADC3 is configured to convert continuously channel13.
Each time an end of conversion occurs the DMA transfers, in circular mode, the
converted data from ADC3 DR register to the ADC3ConvertedValue variable.

To get the maximum ADC performance (2.4 MSPS, at 2.4V to 3.6V supply range),  
the ADC clock must be set to 36MHz. As ADC clock is equal to APB2/2, then APB2 
value will be 72MHz which lead to maximum AHB (System clock) at 144MHz. 
Since the sampling time is set to 3 cycles and the conversion time to 12bit data
is 12 cycles, so the total conversion time is (12+3)/36= 0.41us(2.4Msps).

The converted voltage is displayed on the STM32F429I-DISCO board LCD (when the define USE_LCD
is enabled in main.h). It can also be monitored by adding the variable "ADC3ConvertedValue" 
to the debugger watch window. 


@par Directory contents 

  - ADC_DMA/system_stm32f4xx.c   STM32F4xx system clock configuration file
  - ADC_DMA/stm32f4xx_conf.h     Library Configuration file
  - ADC_DMA/stm32f4xx_it.h       Interrupt handlers header file
  - ADC_DMA/stm32f4xx_it.c       Interrupt handlers
  - ADC_DMA/main.c               Main program
  - ADC_DMA/main.h               Main program header file


@par Hardware and Software environment 

  - This example runs on STM32F429x Devices RevY.
  
  - This example has been tested with STM32F429I-DISCO (MB1075) RevB and can be
    easily tailored to any other development board.
    
  - STM32F429I-DISCO Set-up
    - Connect the external signal(ranges from 0 to 3.0V) to the ADC3 pin (PC.03) to be converted.

@par How to use it ? 

In order to make the program work, you must do the following :

 + EWARM
    - Open the ADC_DMA.eww workspace 
    - Rebuild all files: Project->Rebuild all
    - Load project image: Project->Debug
    - Run program: Debug->Go(F5)
  
 + MDK-ARM
    - Open the ADC_DMA.uvproj project
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
