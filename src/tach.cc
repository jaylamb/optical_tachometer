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

//File: src/tach.cc
//Description: Optical tachometer library implementation

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

#include "../include/tach.h"

void Tach::tach_init( void )
{
    timer_value = 0;
    overflow_count = 0;

    configure_external_interrupt();

    configure_timer0();
}

uint32_t Tach::read_tachometer( void )
{
    _delay_ms( 100 );
    return measured_time;
}

bool Tach::wait_for_tach_trigger( void )
{
    tach_trigger = false;
    reset_timer();

    while( !tach_trigger )
    {
        if ( overflow_count > FIVE_SECONDS ) { return false; }
    }

    return true;
}

void Tach::configure_external_interrupt( void )
{
    DDRD &= ( 0 << PD2 );

    EICRA |= ( 1 << ISC01 | 1 << ISC00 );
    EIMSK |= ( 1 << INT0 );
}

void Tach::configure_timer0( void )
{
    TCCR0B |= ( 1 << CS01  );
    TIMSK0 |= ( 1 << TOIE0 );
}

uint32_t get_current_time( void )
{
    timer_value = TCNT0;
    return ( overflow_count * 256 ) + timer_value;
}

void reset_timer( void )
{
    timer_value = 0;
    overflow_count = 0;
}

/**
 * \brief Interrupt service routine for external interrupt INT0.
 */
 ISR( INT0_vect )
 {
    measured_time = get_current_time();

    if( !tach_trigger ){ tach_trigger = true; }

    reset_timer();
 }

/**
 * \brief Interrupt service routine for Timer/Counter0 overflow.
 */
 ISR( TIMER0_OVF_vect )
 {
     overflow_count++;
 }
