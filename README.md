# Atmel-microcontroller-library-
  ///////////////////////////
   //////  Setup guide  //////
   ///////////////////////////

   0:
   	If on windows, install winAVR to install and setup AVR-C compiler as well as other toolchain components neccecary to compile and flash your programs
	1:
		Locate the name of the ISP device file/port i.e. 'COM2', '/dev/ttyACM0'.
		2: 
			Open "makefile" and change the device to the correct name
			3:
				Open a terminal emulator and change directory to the root of this project folder.
				4:
					To compile and flash the AVR, type in 'make' and press enter. 
						To compile without flashing the AVR, type in 'make object' and press enter.
