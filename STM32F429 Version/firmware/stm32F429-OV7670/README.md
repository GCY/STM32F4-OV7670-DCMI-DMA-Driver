stm32f429-usart1
===============

The program for Verify STM32F429I Discovery USART1 on PORTA 

Build instructions
==================
prepare, you need download and build somthing tools

1. <span>[STM32F429I-Discovery Firmware - STM32F429 Library](http://www.st.com/st-web-ui/static/active/en/st_prod_software_internet/resource/technical/software/firmware/stsw-stm32138.zip)</span>
2. <span>[Stlink - Flash tools](https://github.com/texane/stlink)</span>
3. <span><div>[GCC for ARM Embedded - Toolchain](https://launchpad.net/gcc-arm-embedded)</div><div>currently on Linux(Ubuntu) use [gcc 4.8.2](https://launchpad.net/gcc-arm-embedded/4.8/4.8-2014-q1-update/+download/gcc-arm-none-eabi-4_8-2014q1-20140314-linux.tar.bz2) 2014q1</div></span>

a demo step like the following

```
#
# create a folder
#
mkdir -p stm32f429
cd stm32f429
#
# Download STM32F429I-Discovery Firmware
# url:http://www.st.com/web/en/catalog/tools/PF259429
#
wget http://www.st.com/st-web-ui/static/active/en/st_prod_software_internet/resource/technical/software/firmware/stsw-stm32138.zip
unzip stsw-stm32138.zip
#
# get stlink source code & build
# url: https://github.com/texane/stlink
#
mkdir -p utils
cd utils
git clone https://github.com/texane/stlink.git
cd stlink
./autogen.sh
./configure
make
cd ..
mkdir -p bin
cd bin
cp -vf ../stlink/st-flash .
cp -vf ../stlink/st-info .
cp -vf ../stlink/st-term .
cp -vf ../stlink/st-util .
cd ..
cd ..
#
# download prebuilt toolchain
# url:https://launchpad.net/gcc-arm-embedded
#
mkdir -p toolchain
cd toolchain
wget https://launchpad.net/gcc-arm-embedded/4.8/4.8-2014-q1-update/+download/gcc-arm-none-eabi-4_8-2014q1-20140314-linux.tar.bz2 
tar xjvf gcc-arm-none-eabi-4_8-2014q1-20140314-linux.tar.bz2
cd ..
#
# add utils & toolchain into PATH
#
export PATH=`pwd`/toolchain/gcc-arm-none-eabi-4_8-2014q1/bin:`pwd`/utils/bin:$PATH
#
# download the usart1 source code and build and flash for STM32F429I-Discovery
# url:https://github.com/KunYi/stm32F429-usart1
#
git clone https://github.com/KunYi/stm32F429-usart1.git
cd stm32f429-usart1
make clean all
sudo make flash
```
