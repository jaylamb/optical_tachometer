# Optical Tachometer
`optical_tachometer` is a C/C++ source/header pair which can be used in conjunction with
an AVR microcontroller and the
[`optical_tachometer_hardware`](https://github.com/jaylamb/optical_tachometer_hardware)
sensor printed circuit board.

## Usage
The sensor interface to the microcontroller is a single GPIO pin. By default, pin PD2 is
used. This software provides two primary utilities:
- Calculation of rotational speed (in revolutions per minute) detected by the sensor
- Wait for detection of reflective material in proximity to the sensor

### Interrupts
The following microcontroller interrupts are used by this software:
- External interrupt `INT0` is triggered upon the detection of a rising edge on the
pin.
- Timer/Counter0 and its overflow interrupt (`TIMER0_OVF0`) are used to keep track of time
  for frequency calculations.

## Test Program
This project includes a simple test program, written to use an ATmega328P microcontroller.
The software has been tested on the [Adafruit Metro Mini
328](https://www.adafruit.com/product/2590).

To run the test program:

1. Create and navigate to a build directory:

    `cd build && mkdir build/`

2. Point CMake to the AVR toolchain file:

    `cmake -DCMAKE_TOOLCHAIN_FILE=../cmake_avr_functions.cmake ..`

3. Make and deploy the executable:

    `make upload_optical_tachometer`

## Dependencies
[avr-gcc](https://gcc.gnu.org/wiki/avr-gcc)

[avr-libc](https://www.nongnu.org/avr-libc/)

binutils-avr

## License [Apache 2.0](http://www.apache.org/licenses/)
