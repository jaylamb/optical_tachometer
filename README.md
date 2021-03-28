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

## Dependencies
[avr-gcc](https://gcc.gnu.org/wiki/avr-gcc)

[avr-libc](https://www.nongnu.org/avr-libc/)

binutils-avr

## License [Apache 2.0](http://www.apache.org/licenses/)
