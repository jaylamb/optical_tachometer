# optical_tachometer

#  Copyright 2021 Jay Lamb, jaylamb@gmail.com
#
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.

#File: cmake_avr_functions.cmake
#Description: CMake AVR functions

# options
option(WITH_MCU "Add the MCU type to the target file name." ON)

# executables in use
find_program(AVR_CC avr-gcc)
find_program(AVR_CXX avr-g++)
find_program(AVR_OBJCOPY avr-objcopy)
find_program(AVR_SIZE_TOOL avr-size)
find_program(AVR_OBJDUMP avr-objdump)

# toolchain starts with defining madatory variables
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR, avr)
set(CMAKE_C_COMPILER ${AVR_CC})
set(CMAKE_CXX_COMPILER ${AVR_CXX})

# add exe
# Creates targets and dependencies for AVR toolchain, building an
# executable. Calls add_exe with ELF file as target name so any link
# dependencies ned to be using that target, e.g. for
# target_link_libraries(<EXECUTABLE_NAME>-${AVR_MCU).elf ...)

function(add_avr_executable EXECUTABLE_NAME)
    if (NOT ARGN)
        message(FATAL_ERROR "No source files given for ${EXECUTABLE_NAME}.")
    endif(NOT ARGN)

    # set file names
    set(elf_file     ${EXECUTABLE_NAME}${MCU_TYPE_FOR_FILENAME}.elf)
    set(hex_file     ${EXECUTABLE_NAME}${MCU_TYPE_FOR_FILENAME}.hex)
    set(map_file     ${EXECUTABLE_NAME}${MCU_TYPE_FOR_FILENAME}.map)
    set(eeprom_image ${EXECUTABLE_NAME}${MCU_TYPE_FOR_FILENAME}.eeprom)

    # elf file
    add_executable(${elf_file} EXCLUDE_FROM_ALL ${ARGN})

    set_target_properties(
        ${elf_file}
        PROPERTIES
            COMPILE_FLAGS "-mmcu=${AVR_MCU}"
            LINK_FLAGS "-mmcu=${AVR_MCU} -Wl,--gc-sections -mrelax -Wl,-Map,${map_file}"
    )

    add_custom_command(
        OUTPUT ${hex_file}
        COMMAND
            ${AVR_OBJCOPY} -j .text -j .data -O ihex ${elf_file} ${hex_file}
        COMMAND
            ${AVR_SIZE_TOOL} ${AVR_SIZE_ARGS} ${elf_file}
        DEPENDS ${elf_file}
    )

    # eeprom
    add_custom_command(
        OUTPUT ${eeprom_image}
        COMMAND
        ${AVR_OBJCOPY} -j .eeprom --set-section-flags=.eeprom=alloc,load
                       -O ihex ${elf_file} ${eeprom_image}
        DEPENDS ${elf_file}
    )

    add_custom_target(
        ${EXECUTABLE_NAME}
        ALL
        DEPENDS ${hex_file} ${eeprom_image}
    )

    set_target_properties(
        ${EXECUTABLE_NAME}
        PROPERTIES
            OUTPUT_NAME "${elf_name}"
    )

    # clean up
    get_directory_property(clean_files ADDITIONAL_MAKE_CLEAN_FILES)
    set_directory_properties(
        PROPERTIES
            ADDITIONAL_MAKE_CLEAN_FILES "${map_file}"
    )

    # send it
    add_custom_target(
        upload_${EXECUTABLE_NAME}
        ${AVR_UPLOADTOOL} -p ${AVR_MCU} -c ${AVR_PROGRAMMER} ${AVR_UPLOADTOOL_OPTIONS}
            -U flash:w:${hex_file}
            -P ${AVR_UPLOADTOOL_PORT}
        DEPENDS ${hex_file}
        COMMENT "Uploading ${hex_file} to ${AVR_MCU} using ${AVR_PROGRAMMER}"
    )

    # eeprom only
    add_custom_target(
        upload_eeprom
        ${AVR_UPLOADTOOL} -p ${AVR_MCU} -c ${AVR_PROGRAMMER} ${AVR_UPLOADTOOL_OPTIONS}
            -U eeprom:w:${eeprom_image}
            -P ${AVR_UPLOADTOOL_PORT}
        DEPENDS ${eeprom_image}
        COMMENT "Uploading ${eeprom_image} to ${AVR_MCU} using ${AVR_PROGRAMMER}"
    )

    # status
    add_custom_target(
        get_status
        ${AVR_UPLOADTOOL} -p ${AVR_MCU} -c ${AVR_PROGRAMMER} -P ${AVR_UPLOADTOOL_PORT} -n -v
        COMMENT "Get status from ${AVR_MCU}"
    )

    # fuses
    add_custom_target(
        get_fuses
        ${AVR_UPLOADTOOL} -p ${AVR_MCU} -c ${AVR_PROGRAMMER} -P ${AVR_UPLOADTOOL_PORT} -n
            -U lfuse:r:-:b
            -U hfuse:r:-:b
        COMMENT "Get fuses from ${AVR_MCU}"
    )

    # Still FE: Get oscillator calibration
    add_custom_target(
        get_calibration
            ${AVR_UPLOADTOOL} -p ${AVR_MCU} -c ${AVR_PROGRAMMER} -P ${AVR_UPLOADTOOL_PORT}
            -U calibration:r:${AVR_MCU}_calib.tmp:r
            COMMENT "Program caibration status of internal oscillator from ${AVR_MCU}_calib.hex"
    )

    # Set oscillator calibration
    add_custom_target(
        set_calibration
        ${AVR_UPLOADTOOL} -p ${AVR_MCU} -c ${AVR_PROGRAMMER} -P ${AVR_UPLOADTOOL_PORT}
            -U calibration:w:${AVR_MCU}_calib.hex
            COMMENT "Program calibration status of internal oscillator from ${AVR_MCU}_calib.hex."
    )

    # disassemble
    add_custom_target(
        dissasemble_${EXECUTABLE_NAME}
        ${AVR_OBJDUMP} -h -S ${elf_file} > ${EXECUTABLE_NAME}.lst
        DEPENDS ${elf_file}
    )

endfunction(add_avr_executable)
