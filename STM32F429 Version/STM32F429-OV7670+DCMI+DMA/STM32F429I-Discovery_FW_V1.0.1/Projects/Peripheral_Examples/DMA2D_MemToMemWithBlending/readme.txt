/**
  @page DMA2D_MemToMemWithBlending DMA2D memory to memory with blending example

  @verbatim
  ******************** (C) COPYRIGHT 2013 STMicroelectronics *******************
  * @file    DMA2D_MemToMemWithBlending/readme.txt 
  * @version V1.0.1
  * @date    11-November-2013
  * @brief   Description of the DMA2D memory to memory with blending example
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

  This example provides a description of how to configure DMA2D peripheral in 
  Memory_to_Memory with blending transfer mode.
 
  In this transfer mode two input sources are fitched : foreground and background.
  In this example, the foreground and background are configured as following :
   - input memory address at FLASH memory 
   - Color mode : RGB565 format
   - The constant alpha for foreground is decreased to see the background.
   - The alpha mode for foreground and background is configured to see two 
     superposed images in the resulting image.
        
  The DMA2D blends the two sources pixels to compute the resulting pixel. 
  The transferred data to the output memory address is the result of the blending 
  operation.
 
  In this example, the layer 1 is configured to display the DMA2D output buffer. 
  (the result of the blending of the foreground and background) 
 
  Once the example is run, tow different image are displayed on LCD. Click the 
  user button to see the result of the blending.


@par Directory contents 

  - DMA2D_MemToMemWithBlending/system_stm32f4xx.c   STM32F4xx system clock configuration file
  - DMA2D_MemToMemWithBlending/stm32f4xx_conf.h     Library Configuration file
  - DMA2D_MemToMemWithBlending/stm32f4xx_it.h       Interrupt handlers header file
  - DMA2D_MemToMemWithBlending/stm32f4xx_it.c       Interrupt handlers
  - DMA2D_MemToMemWithBlending/main.c               Main program
  - DMA2D_MemToMemWithBlending/main.h               Main program header file


@par Hardware and Software environment 

  - This example runs on STM32F429x Devices RevY.
  
  - This example has been tested with STM32F429I-DISCO (MB1075) RevB and can be
    easily tailored to any other development board.

@par How to use it ? 

In order to make the program work, you must do the following :

 + EWARM
    - Open the DMA2D_MemToMemWithBlending.eww workspace 
    - Rebuild all files: Project->Rebuild all
    - Load project image: Project->Debug
    - Run program: Debug->Go(F5)
  
 + MDK-ARM
    - Open the DMA2D_MemToMemWithBlending.uvproj project
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
