# Atmel Microcontroller Library
### Setup guide

##### Windows - specific
1. Install winAVR
2. Setup AVR-C compiler as well as other toolchain components neccecary to compile and flash your programs

##### Linux
1. Sit back and laugh at people with windows

##### All
1. Locate the name of the ISP device file/port i.e. `COM2`, `/dev/ttyACM0`.
2. Open `makefile` and change the device to the correct port
3. `cd` to the root of this project folder.
* To compile and flash the AVR, type in 'make' and press enter. 
* To compile without flashing the AVR, type in 'make object' and press enter.
