/**
  @page ADC_DualModeInterleaved  Use ADC1 and ADC2 in Dual interleaved mode and DMA mode3

  @verbatim
  ******************** (C) COPYRIGHT 2013 STMicroelectronics *******************
  * @file    ADC_DualModeInterleaved/readme.txt 
  * @version V1.0.1
  * @date    11-November-2013
  * @brief   Description of the Dual interleaved mode and DMA mode3 Example
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

This example provides a short description of how to use the ADC peripheral to
convert a regular channel in Dual interleaved mode using DMA in mode 3 with 6Msps.

DMA mode 3 is used in interleaved mode in 6-bit and 8-bit resolutions.

The Dual interleaved delay is configured 6 ADC clk cycles.

On each DMA request (two data items are available) two bytes representing two 
ADC-converted data items are transferred as a half word.
The data transfer order is similar to that of the DMA mode 2.

A DMA request is generated each time 2 data items are available
1st request: ADC_CDR[15:0] = (ADC2_DR[7:0] << 8) | ADC1_DR[7:0] 
2nd request: ADC_CDR[15:0] = (ADC2_DR[7:0] << 8) | ADC1_DR[7:0]

The ADC1 and ADC2 are configured to convert ADC Channel 13, with conversion 
triggered by software.
By this way, ADC channel 13 is converted each 6 cycles.

To get the maximum ADC performance (6 MSPS, at 2.4V to 3.6V supply range),  
the ADC clock must be set to 36MHz. As ADC clock is equal to APB2/2, then APB2 
value will be 72MHz which lead to maximum AHB (System clock) at 144MHz. 

==> Conversion Time = 36M/6cyc = 6Msps.

@note Refer to "simulation.xls" file to have the diagram simulation of the example.


@par Directory contents 

  - ADC_DualModeInterleaved/system_stm32f4xx.c   STM32F4xx system clock configuration file
  - ADC_DualModeInterleaved/stm32f4xx_conf.h     Library Configuration file
  - ADC_DualModeInterleaved/stm32f4xx_it.h       Interrupt handlers header file
  - ADC_DualModeInterleaved/stm32f4xx_it.c       Interrupt handlers
  - ADC_DualModeInterleaved/main.c               Main program
  - ADC_DualModeInterleaved/main.h               Main program header file


@par Hardware and Software environment 

  - This example runs on STM32F429x Devices RevY.
  
  - This example has been tested with STM32F429I-DISCO (MB1075) RevB and can be
    easily tailored to any other development board.

  - STM32F429I-DISCO Set-up
    - Connect the external signal(ranges from 0 to 3.0V) to the ADC1 pin (PC.03) to be converted.
    
@par How to use it ? 

In order to make the program work, you must do the following :

 + EWARM
    - Open the ADC_DualModeInterleaved.eww workspace 
    - Rebuild all files: Project->Rebuild all
    - Load project image: Project->Debug
    - Run program: Debug->Go(F5)
  
 + MDK-ARM
    - Open the ADC_DualModeInterleaved.uvproj project
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
