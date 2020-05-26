/**
  @page LTDC_AnimatedPictureFromUSB LTDC example Readme file
  
  @verbatim
  ******************************************************************************
  * @file    LTDC_AnimatedPictureFromUSB/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    11-November-2013
  * @brief   Description of the LTDC_AnimatedPictureFromUSB example.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
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

  This example describes how to display on LCD pictures saved under USB mass storage.
   
  The user has to put bitmap pictures that don't exceed LCD size(width: 240, high: 320)
  under root of USB mass storage (you can use pictures under LTDC_AnimatedPictureFromUSB\Media repository). 
          
  Once the LCD, USB OTG HS and file system are initialized and configured, LEDs 
  start blinking and a check of the existence and the content of the USB mass 
  storage directory is done.
 
  A list of USB mass storage content will be displayed followed by a message 
  indicating that a press of user button is needed to start the display of
  existing images.

@note : - Once all images are displayed, the process is repeated.
        - The system clock (SYSCLK) is configured to 168 MHz to provide 48 MHz clock needed
          for the USB operation. Please note that the USB is not functional if
          the system clock is set to 180 MHz. 
       
@par Project Directory contents

    - LTDC_AnimatedPictureFromUSB/stm32f4xx_it.h       Header for stm32f4xx_it.c
    - LTDC_AnimatedPictureFromUSB/stm32f4xx_conf.h     library configuration file 
    - LTDC_AnimatedPictureFromUSB/ffconf.h             Configuration file for FatFs module.
    - LTDC_AnimatedPictureFromUSB/fatfs_storage.h      Header for fatfs_storage.c
    - LTDC_AnimatedPictureFromUSB/main.c               Main program 
    - LTDC_AnimatedPictureFromUSB/fatfs_storage.c      Storage (FatFs) driver
    - LTDC_AnimatedPictureFromUSB/fatfs_drv.c          diskio interface
    - LTDC_AnimatedPictureFromUSB/stm32f4xx_it.c       Interrupt handlers
    - LTDC_AnimatedPictureFromUSB/system_stm32f4xx.c   STM32F4xx system clock configuration file

@par Hardware and Software environment  

  - This example runs on STM32F429x Devices RevY.
    
  - This example has been tested with STMicroelectronics STM32F429I-DISCO (MB1075B) 
    discovery boards and can be easily tailored to any other supported device and 
    development board.
  - Please, refer to HardwareToRunExample.png picture to know about hardware 
    needed to run the example.   
 
@par How to use it ?
In order to make the program work, you must do the following :

  + EWARM
    - Open the LTDC_AnimatedPictureFromUSB.eww workspace 
    - Rebuild all files: Project->Rebuild all
    - Load project image: Project->Debug
    - Run program: Debug->Go(F5)
  
 + MDK-ARM
    - Open the LTDC_AnimatedPictureFromUSB.uvproj project
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
     
@note Known Limitations
      This example retargets the C library printf() function to the Discovery board’s LCD
      screen (C library I/O redirected to LCD) to display some Library and user debug
      messages. TrueSTUDIO Lite version does not support I/O redirection, and instead
      have do-nothing stubs compiled into the C runtime library. 
      As consequence, when using this toolchain no debug messages will be displayed
      on the LCD (only some control messages in green will be displayed in bottom of
      the LCD). To use printf() with TrueSTUDIO Professional version, just include the
      TrueSTUDIO Minimal System calls file "syscalls.c" provided within the toolchain.
      It contains additional code to support printf() redirection.    
      
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
                                   