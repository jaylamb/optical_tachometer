// optical_tachometer

//  Copyright 2021 Jay Lamb, jaylamb@gmail.com
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.

//File: test/optical_tachometer_test.cc
//Description: Optical tachometer test/demo program


#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>

#include "../include/tach.h"
#include "uart.h"

int main( void ){

uint32_t const TIMER_FREQUENCY = 2000000;
int32_t frequency_ticks = 0;
int32_t frequency_whole = 0;
int32_t frequency_tenths = 0;

char frequency_string[20];
char uart_initialized_string[] = "UART Initialized.\n\r";
char tach_initialized_string[] = "Tach Initialized.\n\r";
char timeout_string[] = "Timed out.\n\r";
char waiting_string[] = "Waiting for trigger.\n\r";

Tach optical_tachometer;

uart_init();
uart_send_string( uart_initialized_string );

optical_tachometer.tach_init();
uart_send_string( tach_initialized_string );

sei();

    while (1) {

        uart_send_string( waiting_string );
        if ( optical_tachometer.wait_for_tach_trigger())
        {
            frequency_ticks = optical_tachometer.read_tachometer();
            frequency_whole = TIMER_FREQUENCY / frequency_ticks;
            frequency_tenths = ( TIMER_FREQUENCY * 10 / frequency_ticks ) - ( frequency_whole * 10 );

            sprintf( frequency_string, "%lu.%lu\n\r", frequency_whole, frequency_tenths);
            uart_send_string( frequency_string );
        } // if
        else
            uart_send_string( timeout_string );

        _delay_ms( 2000 );
    } // while

return( 0 );

}
