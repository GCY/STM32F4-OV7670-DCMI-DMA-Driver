/**
  @page ADC_TripleModeInterleaved  Use ADC1, ADC2 and ADC3 in Triple interleaved mode and DMA mode2 with 8.4Msps

  @verbatim
  ******************** (C) COPYRIGHT 2013 STMicroelectronics *******************
  * @file    ADC_TripleModeInterleaved/readme.txt 
  * @version V1.0.1
  * @date    11-November-2013
  * @brief   Description of the Triple interleaved mode and DMA mode2 Example
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
convert a regular channel in Triple interleaved mode using DMA in mode 2 with 
8.4Msps .

The Triple interleaved delay is configured 5 ADC clk cycles.

In Triple ADC mode, three DMA requests are generated: 
- On the first request, both ADC2 and ADC1 data are transferred (ADC2 data take 
  the upper half-word and ADC1 data take the lower half-word). 
- On the second request, both ADC1 and ADC3 data are transferred (ADC1 data take
  the upper half-word and ADC3 data take the lower half-word).
- On the third request, both ADC3 and ADC2 data are transferred (ADC3 data take 
  the upper half-word and ADC2 data take the lower half-word) and so on.

On each DMA request (two data items are available) two half-words representing 
two ADC-converted data items are transferred as a word.

A DMA request is generated each time 2 data items are available :
1st request: ADC_CDR[31:0] = (ADC2_DR[15:0] << 16) | ADC1_DR[15:0] (step1)
2nd request: ADC_CDR[31:0] = (ADC1_DR[15:0] << 16) | ADC3_DR[15:0] (step2)
3rd request: ADC_CDR[31:0] = (ADC3_DR[15:0] << 16) | ADC2_DR[15:0] (step3)
4th request: ADC_CDR[31:0] = (ADC2_DR[15:0] << 16) | ADC1_DR[15:0] (step1) and so on.

The conversion is triggered by software.

The ADC1, ADC2 and ADC3 are configured to convert ADC Channel 13.
By this way, the ADC can reach 7.2Msps, in fact the same channel is converted
each 5 cycles

To get the maximum ADC performance (7.2 MSPS, at 2.4V to 3.6V supply range),  
the ADC clock must be set to 36MHz. As ADC clock is equal to APB2/2, then APB2 
value will be 72MHz which lead to maximum AHB (System clock) at 144MHz. 
==> Conversion Time = 36M/5cyc = 7.2Msps

@note Refer to "simulation.xls" file to have the diagram simulation of the example.


@par Directory contents 

  - ADC_TripleModeInterleaved/system_stm32f4xx.c   STM32F4xx system clock configuration file
  - ADC_TripleModeInterleaved/stm32f4xx_conf.h     Library Configuration file
  - ADC_TripleModeInterleaved/stm32f4xx_it.h       Interrupt handlers header file
  - ADC_TripleModeInterleaved/stm32f4xx_it.c       Interrupt handlers
  - ADC_TripleModeInterleaved/main.c               Main program
  - ADC_TripleModeInterleaved/main.h               Main program header file


@par Hardware and Software environment 

  - This example runs on STM32F429x Devices RevY.
  
  - This example has been tested with STM32F429I-DISCO (MB1075) RevB and can be
    easily tailored to any other development board.
    
  - STM32F429I-DISCO Set-up
    - Connect the external signal(ranges from 0 to 3.0V) to the ADC pin (PC.03) to be converted.    

@par How to use it ? 

In order to make the program work, you must do the following :

 + EWARM
    - Open the ADC_TripleModeInterleaved.eww workspace 
    - Rebuild all files: Project->Rebuild all
    - Load project image: Project->Debug
    - Run program: Debug->Go(F5)
  
 + MDK-ARM
    - Open the ADC_TripleModeInterleaved.uvproj project
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
