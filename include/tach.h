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

//File: include/tach.h
//Description: Optical tachometer library header

#ifndef TACH_H
#define TACH_H

#include <stdint.h>

class Tach
{
    public:
        /**
         * \brief Initialize tachometer.
         */
         void tach_init( void );

        /**
        * \brief Read the frequency measured by the tachometer.
        *
        * \return The frequency measured by the tachometer (in Hz)
        */
        uint32_t read_tachometer( void );

        /**
        * \brief Wait for a tach pulse.
        *
        * \return true once a pulse is received
        * \return false if a pulse is not received before the timeout
        */
        bool wait_for_tach_trigger( void );

    private:
        /**
         * \brief Configure external interrupt INT0.
         */
         void configure_external_interrupt( void );

        /**
         * \brief Configure Timer/Counter0.
         */
         void configure_timer0( void );

        /**
         * \brief Timer/Counter0 tick frequency.
         */
         uint32_t static const TIMER_FREQUENCY = 2000000;

         /**
          * \brief Timer/Counter0 overflows in five seconds (approximate).
          */
         uint32_t static const FIVE_SECONDS = 39063;
};

    /**
     * \brief Calculate the current time on Timer/Counter0
     *
     * \return The current time (in ticks) on Timer/Counter0.
     */
    uint32_t get_current_time( void );

    /**
     * \brief Reset timer.
     */
    void reset_timer( void );

    /**
     * \brief True when a reflection has been detected, false otherwise.
     */
    bool static volatile tach_trigger;

    /**
     * \brief The current value of the 8-bit Timer/Counter (TCNT0).
     */
    uint8_t static volatile timer_value;

    /**
     * \brief Number of times the Timer/Counter has overflowed.
     */
    uint32_t static volatile overflow_count;

    /**
     * \brief Number of ticks since the last tach trigger interrupt.
     */
    uint32_t static volatile measured_time;

#endif
