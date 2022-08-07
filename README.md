# pinetime-blinky
A Work-in-Progress firmware for the PineTime smartwatch.

Motivated by my dislike for overly complex dependency trees and build systems, and general unhappiness with the current state of PineTime firmware.
Written purely in C and assembly, and compiled using a simple makefile.

## Dependencies
* PineTime smartwatch
* ST-Link v2 or compatible debugger
* make
* arm-none-eabi-gcc
* arm-none-eabi-binutils
* arm-none-eabi-gdb (for debugging)
* openocd (for flashing/debugging)
* doxygen (for documentation)
