/**
  @page DMA2D_MemToMemWithPFC  DMA2D memory to memory with PFC example

  @verbatim
  ******************** (C) COPYRIGHT 2013 STMicroelectronics *******************
  * @file    DMA2D_MemToMemWithPFC/readme.txt 
  * @version V1.0.1
  * @date    11-November-2013
  * @brief   Description of the DMA2D memory to memory with PFC example
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
  Memory_to_Memory with pixel format conversion transfer mode.
 
  In this basic example the goal is to explain the different fields of the DMA2D 
  structure in the case of Memory_to_Memory with pixel format conversion transfer mode
  and the difference between pixel coded on 16bits(RGB565) and coded on 16bits(ARGB4444).
 
  An image is transferred from flash memory to internal RAM and during the transfer,
  a pixel format conversion is applied from RGB565 to ARGB4444. 
  The original image and the transferred image are displayed on the LCD to see 
  the difference between an image coded on 16bits(RGB565) and coded on 16bits(ARGB4444).
 
 In this example two LTDC layers are used to display the original and the converted
 images as following :
  - Layer 1 is configured to display the original image with RGB565 as 
    pixel format and 240x150 size.
  - Layer 2 is configured to display the converted image with ARGB4444 as 
    pixel format and 240x150 size.

 
  @note :
  how to calculate the size of the transferred data ? 
  => selected color mode gives the number of bits per pixel and we have 
    the number of pixel per line and the number of line, therefore :
    
    data_size = (bits per pixel) X (pixel per line) X (number of line)
    
 How to convert pixel format from RGB565 to ARGB4444 ?
 => only the four MSB are taken into account for RGB components and 0xF is added 
    for alfa (A) component.  
 eg : 0x CD  12  34 --> 0xF   C   1   3    
         |_| |_| |_|     |_| |_| |_| |_|
          R   G   B       A   R   G   B
         
 In general, 
 => if the number of bits per pixel in source data is more then the number of 
    bits per pixel in destination data, only the MSB are taken into account
    
 => else, if the number of bits per pixel in source data is less then the number 
    of bits per pixel in destination data, a bit replication of MSB in LSB is applied
    
    eg : from ARGB1555 to ARGB8888       PFC
    0xAB35 --> 0b1 01010 11001 10101   -------> 0b11111111 01010010 11001110 10101101     
                 A   R     G     B          --> 0xFF       52       CE       AD
         
@par Directory contents 

  - DMA2D_MemToMemWithPFC/system_stm32f4xx.c   STM32F4xx system clock configuration file
  - DMA2D_MemToMemWithPFC/stm32f4xx_conf.h     Library Configuration file
  - DMA2D_MemToMemWithPFC/stm32f4xx_it.h       Interrupt handlers header file
  - DMA2D_MemToMemWithPFC/stm32f4xx_it.c       Interrupt handlers
  - DMA2D_MemToMemWithPFC/main.c               Main program
  - DMA2D_MemToMemWithPFC/main.h               Main program header file


@par Hardware and Software environment 

  - This example runs on STM32F429x Devices RevY.
  
  - This example has been tested with STM32F429I-DISCO (MB1075) RevB and can be
    easily tailored to any other development board.

@par How to use it ? 

In order to make the program work, you must do the following :

 + EWARM
    - Open the DMA2D_MemToMemWithPFC.eww workspace 
    - Rebuild all files: Project->Rebuild all
    - Load project image: Project->Debug
    - Run program: Debug->Go(F5)
  
 + MDK-ARM
    - Open the DMA2D_MemToMemWithPFC.uvproj project
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
