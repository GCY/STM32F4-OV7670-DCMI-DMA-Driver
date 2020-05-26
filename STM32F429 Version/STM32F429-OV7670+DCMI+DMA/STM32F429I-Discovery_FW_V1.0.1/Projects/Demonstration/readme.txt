/**
  @page STM32F429I demonstration firmware
  
  @verbatim
  ******************** (C) COPYRIGHT 2013 STMicroelectronics *******************
  * @file    Demonstration/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    11-November-2013
  * @brief   Description of the STM32429I-Discovery demonstration firmware. 
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


@par Demonstration Description

The demonstration firmware, based on the STemWin GUI library, shows a menu
based on IconView Widget composed of: Video player, Image browser, Clock/Calendar,
Game, Performance and System Info modules. The status bar indicates the CPU
usage, USB disk flash connection state, alarm and time.

Follow the sequence below to configure the STM32F429I-DISCO board and launch the
demo application:
 - Check jumper position on the board, JP3 on and CN4 on (DISCOVERY selected).
 - Connect the STM32F429I-DISCO board to a PC with a USB cable type'A to mini-B'
   through the USB connector CN1 to power the board. Red LED LD2 (PWR) then
   lights up.
 - Play with the applications displayed on the screen:
   + Video player: 
     - Connect a USB key on the USB USER connector CN6, then, videos in the root
       directory can be played.
     - Supported video format is MJPEG (Motion JPEG).
     - Several free software tools can be used to convert different types of
       video files to MJPEG format. As an example, there may be mentioned,
       "mp4-to-avi-converter"
       (http://www.pazera-software.com/products/mp4-to-avi-converter).
     - *.mjpeg Files under the directory \Media can also be reused.
     - Initial display of the video player presents all possible control buttons
       (Play/Pause, Next, Previous, Stop, Close), and also an interactive scroll
       bar.
     - To switch to full screen mode, just touch the video screen. To exit the
       full screen mode touch once again the LCD.
   + Image browser
     - Connect a USB key on the USB USER connector CN6, then, images in the root
       directory can be displayed.
     - Supported image format is JPG.
     - some *.jpg samples are provided with the demo (under \Media).
     - Initial display of the image browser includes the following buttons:
       + Previous
       + Next
       + Automatic/Manual: to choose the image transition mode
     - To switch to full screen mode, just touch the image screen. If the
       Automatic mode is already selected it will still be valid until the
       browser is closed. To exit the full screen mode touch once again the LCD.
   + Clock/Calendar:
     - Set the date, time and alarm by Selecting the "Settings" button. Edition
       of the different field can then be made via the Up/Down arrows of the
       spinbox or directly via the Numerical pad.
     - Before selecting the "Apply" button make sure that the right Checkbox is
       selected (corresponding to Date, Time and/or Alarm). 
     - If the alarm is set, a small alarm icon is displayed in the status bar.
     - When alarm time is reached a popup window appears and the Red LED LD4
       lights up. Then click on "Close" button to set the LED LD4 off an close
       the alarm popup window. 
   + Game:
     - Reversi game.
   + Performance:
     - Allows to watch the real time CPU load and run a graphical benchmark.
   + System info:
     - Displays some information about the used hardware.

 @note The system clock (SYSCLK) is configured to 168 MHz to provide 48 MHz clock needed
       for the USB operation. 
       Please note that the USB is not functional if the system clock is set to 180 MHz. 


@par Firmware architecture

The Demonstration is built using the following software components:

  - STM32F4xx DSP and Standard Peripherals Library
  - STM32 USB On-The-Go Host and Device Library
  - STemWin Library (this is a customized version of STemWin Library V5.22 and limited
                     for use only with STM32429I-Discovery demonstration firmware.)
  - FreeRTOS
  - FatFS file system


@par Hardware and Software environment

  - This Demonstration runs on STMF429xx Devices.

  - This Demonstration has been tested with STMicroelectronics STM32F429I-DISCO
    board and can be easily tailored to any other supported device and
    development board.

  - The HardwareToRunExample.jpg image provided under the demonstration folder root
    shows the hardware setup. 


@par How to use it? 

In order to make the program works, you must do the following:

 + EWARM
    - Open the Template.eww workspace.
    - Rebuild all files: Project->Rebuild all.
    - Load project image: Project->Debug.
    - Run program: Debug->Go(F5).

 + MDK-ARM
    - Open the xxx.uvproj project.
    - Rebuild all files: Project->Rebuild all target files.
    - Load project image: Debug->Start/Stop Debug Session.
    - Run program: Debug->Run (F5).

 + TrueSTUDO
    - Open the TrueSTUDIO toolchain.
    - Click on File->Switch Workspace->Other and browse to TrueSTUDIO workspace
      directory.
    - Click on File->Import, select General->'Existing Projects into Workspace'
      and then click "Next".
    - Browse to the TrueSTUDIO workspace directory, select the project.
    - Rebuild all project files: Select the project in the "Project explorer" 
      window then click on Project->build project menu.
    - Run program: Run->Debug (F11).
 
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
