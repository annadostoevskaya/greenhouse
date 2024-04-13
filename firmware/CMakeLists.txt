cmake_minimum_required(VERSION 3.5)

# program names
set(AVRCPP   avr-g++)
set(AVRC     avr-gcc)
set(AVRSTRIP avr-strip)
set(OBJCOPY  avr-objcopy)
set(OBJDUMP  avr-objdump)
set(AVRSIZE  avr-size)
set(AVRDUDE  avrdude)

# Sets the compiler
# Needs to come before the project function
set(CMAKE_SYSTEM_NAME       Generic)
set(CMAKE_CXX_COMPILER      ${AVRCPP})
set(CMAKE_C_COMPILER        ${AVRC})
set(CMAKE_ASM_COMPILER      ${AVRC})

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project (blink C CXX ASM)

set(MCU         atmega328p)
add_definitions(-DF_CPU=16E6)
add_definitions(-D__AVR_ATtiny2313__)
add_definitions(-DBAUD=9600UL)

# Variables regarding the AVR chip
set(AVRDUDE_PROGTYPE    arduino)
if (WIN32)
    set(AVRDUDE_PORT    COM3)
elseif(UNIX)
    set(AVRDUDE_PORT    /dev/ttyACM0)
endif()
set(AVRDUDE_DEVICE      m328p)
set(AVRDUDE_BAUD        115200)
set(AVRDUDE_FUSES       ) # -U lfuse:w:0xde:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m)

# Files to be compiled
file(GLOB SRC_FILES "src/*.cpp" "src/*.c")

# Compiler flags
set(CSTANDARD "-std=gnu99")
set(CDEBUG    "-g -ggdb")
set(CWARN     "-Wall -Wextra -Werror -Wstrict-prototypes -Wl,--gc-sections -Wl,--relax")
set(CTUNING   "-funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -ffunction-sections -fdata-sections")
set(COPT      "-Os -lm -lprintf_flt")
set(CMCU      "-mmcu=${MCU}")

set(CFLAGS   "${CMCU} ${CDEBUG} ${COPT} ${CWARN} ${CSTANDARD} ${CTUNING}")
set(CXXFLAGS "${CMCU} ${CDEBUG} ${COPT} ${CTUNING}")

set(CMAKE_C_FLAGS   "${CFLAGS}")
set(CMAKE_CXX_FLAGS "${CXXFLAGS}")
set(CMAKE_ASM_FLAGS "${CFLAGS}")

# Project setup
add_executable(${PROJECT_NAME} ${SRC_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}.bin")

# Compiling targets
add_custom_target(flash ${AVRDUDE} -c ${AVRDUDE_PROGTYPE} -p ${AVRDUDE_DEVICE} -P ${AVRDUDE_PORT}
                        -b ${AVRDUDE_BAUD} -U flash:w:${PROJECT_NAME}.hex ${AVRDUDE_FUSES} DEPENDS hex)
add_custom_target(hex   ${OBJCOPY} -O ihex -R .eeprom ${PROJECT_NAME}.bin ${PROJECT_NAME}.hex)

set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${PROJECT_NAME}.hex;${PROJECT_NAME}.eeprom;${PROJECT_NAME}.lst;${PROJECT_NAME}.bin")
