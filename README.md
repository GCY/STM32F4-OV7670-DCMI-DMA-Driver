# STM32F429-Discovery-OV7670-DCMI-DMA-Driver

STM32F429-DISCO+OV7670 MAC and Linux project with arm-none-eabi-gcc, it is modified from [Individální projekty MPOA](http://www.urel.feec.vutbr.cz/MPOA/2014/cam-ov7670) Keil-C project.

![](https://github.com/GCY/STM32F429-Discovery-OV7670-DCMI-DMA-Driver/blob/master/res/demo.gif) 

## Hardware Configuration
| STM32F429 | OV7670 |
| :----: | :----: |
| 3V3 | RESET |
| GND | PWDN |
| PC6 | D0 |
| PC7 | D1 |
| PC8 | D2 |
| PC9 | D3 |
| PE4 | D4 |
| PB6 | D5 |
| PE5 | D6 |
| PE6 | D7 |
| PA8 | XCLK |
| PA6 | PCLK |
| PA4 | HREF |
| PB7 | VSYNC |
| PB9 | SIOD |
| PB8 | SIOC |
| GND | GND |
| 3V3 | 3V3 |

## Build

 1. cd ./STM32F429-Discovery-OV7670-DCMI-DMA-Driver/STM32F429-OV7670+DCMI+DMA/stm32F429-OV7670 & make
 2. load *.elf file with arm-none-eabi-gdb
 3. run

## Test

 1. First, power on and initialize STM32F429 OV7670 SCCB bus.
 2. When the LCD progress bar is complete, OV7670 SCCB command are initialized.
 3. Push "User" button.
 4. STM32F429 capture frame and display to LCD.

Licensing
=======
Copyright (C) 2020  TonyGUO <https://github.com/GCY>.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
