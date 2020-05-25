/**
  @page Touch_Panel  Touch Panel example

  @verbatim
  ******************** (C) COPYRIGHT 2013 STMicroelectronics *******************
  * @file    Touch_Panel/readme.txt 
  * @version V1.0.1
  * @date    11-November-2013
  * @brief   Description of the Touch Panel paint Example
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

 This example describes how to configure and use the touch panel mounted on 
 STM32F429I-DISCO boards.
 
 After LCD initialization and Touch panel configuration, user can paint using a 
 color that he can choose from the displayed list of colors in the bottom of the
 LCD. The LCD will look like:
  ______________________________
 |                              |
 |                              |    1: BLUE2 color
 |                              |    2: CYAN color 
 |                              |    3: YELLOW color
 |                              |    4: RED color
 |                              |    5: BLUE color
 |                              |    6: GREEN color
 |                              |    7: BLACK color
 |                              |    8: MAGENTA color
 |                              |    C: Clear LCD
 |                              |
 |                              |
 |______________________________|
 |   |   |   | Touch Panel Paint|
 | 1 | 2 | 3 | Example_______   |
 |___|___|___|________|     |   |
 | 4 | 5 | 6 | 7 | 8  |  C  |   |
 |___|___|___|___|____|_____|___|

 The default color is the BLACK.
 

@par Directory contents 

  - Touch_Panel/system_stm32f4xx.c   STM32F4xx system clock configuration file
  - Touch_Panel/stm32f4xx_conf.h     Library Configuration file
  - Touch_Panel/stm32f4xx_it.h       Interrupt handlers header file
  - Touch_Panel/stm32f4xx_it.c       Interrupt handlers
  - Touch_Panel/main.c               Main program
  - Touch_Panel/main.h               Main program header file


@par Hardware and Software environment 

  - This example runs on STM32F429x Devices RevY.
  
  - This example has been tested with STM32F429I-DISCO (MB1075) RevB and can be
    easily tailored to any other development board.

@par How to use it? 

In order to make the program work, you must do the following :
 + EWARM
    - Open the Touch_Panel.eww workspace 
    - Rebuild all files: Project->Rebuild all
    - Load project image: Project->Debug
    - Run program: Debug->Go(F5)
  
 + MDK-ARM
    - Open the Touch_Panel.uvproj project
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
