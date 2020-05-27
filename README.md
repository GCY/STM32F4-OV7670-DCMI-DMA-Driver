# STM32F4-Discovery-OV7670-DCMI-DMA-Driver

STM32F429-DISCO+OV7670 MAC and Linux project with arm-none-eabi-gcc, it is modified from [Individální projekty MPOA](http://www.urel.feec.vutbr.cz/MPOA/2014/cam-ov7670) Keil-C project.

![](https://github.com/GCY/STM32F4-OV7670-DCMI-DMA-Driver/blob/master/STM32F429%20Version/res/demo.gif) 

![](https://github.com/GCY/STM32F4-OV7670-DCMI-DMA-Driver/blob/master/STM32F407%20Version/res/demo.gif)

![alt text](https://github.com/GCY/STM32F4-OV7670-DCMI-DMA-Driver/blob/master/STM32F407%20Version/res/v1.jpg?raw=true)



## DCMI Configuration
| STM32F407 | STM32F429 | OV7670 |
| :----: | :----: | :----: |
| 3V3 | 3V3 | RESET |
| GND | GND | PWDN |
| PC6 | PC6 | D0 |
| PC7 | PC7 | D1 |
| 3V3 | PC8 | D2 |
| 3V3 | PC9 | D3 |
| 3V3 | PE4 | D4 |
| 3V3 | PB6 | D5 |
| 3V3 | PE5 | D6 |
| 3V3 | PE6 | D7 |
| 3V3 | PA8 | XCLK |
| 3V3 | PA6 | PCLK |
| 3V3 | PA4 | HREF |
| 3V3 | PB7 | VSYNC |
| 3V3 | PB9 | SIOD |
| 3V3 | PB8 | SIOC |
| 3V3 | GND | GND |
| 3V3 | 3V3 | 3V3 |

## Build

 1. cd .STM32F429-Discovery-OV7670-DCMI-DMA-Driver/STM32F429 Version/firmware/stm32F429-OV7670 & make
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
