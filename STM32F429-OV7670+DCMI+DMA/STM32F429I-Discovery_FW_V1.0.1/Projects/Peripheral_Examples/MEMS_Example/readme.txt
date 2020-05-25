/**
  @page MEMS_Example  MEMS example 
  
  @verbatim
  ******************** (C) COPYRIGHT 2013 STMicroelectronics *******************
  * @file    MEMS_Example/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    11-November-2013
  * @brief   Description of the MEMS example
  ******************************************************************************
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

@par MEMS_Example Description
This example shows a simple test of how to use the MEMS sensor(L3GD20) mounted on the 
STM32F429I-DISCO board.

After LCD configuration and MEMS sensor enabling, the gyroscope angular data rate
is read infinitely. According to the angle value, an arrow will be displayed on 
LCD.

A calibration is done to remove the offset on X and Y data rate values so for a 
better behaviour we advise to run this example while the board is in an horizontal  
position.
If not, the example works but the offset value are different of zero.

@par Directory contents

  - MEMS_Example/system_stm32f4xx.c   STM32F4xx system clock configuration file
  - MEMS_Example/stm32f4xx_conf.h     Library Configuration file
  - MEMS_Example/stm32f4xx_it.h       Interrupt handlers header file
  - MEMS_Example/stm32f4xx_it.c       Interrupt handlers
  - MEMS_Example/main.c               Main program
  - MEMS_Example/main.h               Main program header file


@par Hardware and Software environment

  - This example runs on STM32F429x Devices RevY.
  
  - This example has been tested with STMicroelectronics STM32F429I-DISCO board
    (MB1075) RevB and can be easily tailored to any other supported device and 
    development board.


@par How to use it? 

In order to make the program work, you must do the following :
 
 + EWARM
    - Open the MEMS_Example.eww workspace 
    - Rebuild all files: Project->Rebuild all
    - Load project image: Project->Debug
    - Run program: Debug->Go(F5)
  
 + MDK-ARM
    - Open the MEMS_Example.uvproj project
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
