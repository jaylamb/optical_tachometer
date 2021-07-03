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

//File: test/uart.h
//Description: Header for basic UART functions for use in debugging/testing

void uart_init( void );

void uart_transmit( unsigned char data );

void uart_send_string( char const * string );
