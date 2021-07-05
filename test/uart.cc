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

//File: test/uart.cc
//Description: Basic UART functions for use in debugging/testing

#include <avr/io.h>
#include <string.h>

#define BAUD 9600                                 // define baud
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)          // set baud rate value for UBRR

void uart_init (void)
{
    UBRR0H  = (BAUDRATE>>8);                      // shift the register right by 8 bits
    UBRR0L  = BAUDRATE;                           // set baud rate
    UCSR0B |= (1<<TXEN0)|(1<<RXEN0);              // enable receiver and transmitter
    UCSR0C |= (1<<USBS0)|(3<<UCSZ00);             // 8 bit data format
}

void uart_transmit( unsigned char data )
{
    while ( !( UCSR0A & (1<<UDRE0)) );            // wait for transmit buffer to be empty
    UDR0 = data;                                  // place input character into tx buffer
}

void uart_send_string( char const * string )
{
    static constexpr char delimeter = ':';

    for( size_t i = 0; i < strlen(string); i++ )
        uart_transmit( string[ i ] );

    uart_transmit( delimeter );
}
