# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
# set(PICO_SDK_PATH "/home/jabba/pico-sdk")

set(PICO_BOARD pico CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.4.0")
  message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.4.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

project(pico-widgets-test C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1
add_executable(pico-widgets-test
        ${PROJECT_SOURCE_DIR}/pico-touchscr-sdk/lib/assert.c
        ${PROJECT_SOURCE_DIR}/pico-touchscr-sdk/ili9341/ili9341.c
        ${PROJECT_SOURCE_DIR}/pico-touchscr-sdk/touch/msp2807_touch.c
        ${PROJECT_SOURCE_DIR}/pico-touchscr-sdk/touch/msp2807_calibration.c
)

target_sources(pico-widgets-test PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/ui/ui_init.c
        ${CMAKE_CURRENT_LIST_DIR}/ui/ui_manage.c
        ${CMAKE_CURRENT_LIST_DIR}/widgets/widgets_utility.c
        ${CMAKE_CURRENT_LIST_DIR}/widgets/TopBarEventProc.c
        ${CMAKE_CURRENT_LIST_DIR}/widgets/SettingsEventProc.c
        ${CMAKE_CURRENT_LIST_DIR}/widgets/APRSEventProc.c
        ${CMAKE_CURRENT_LIST_DIR}/widgets/PSKEventProc.c
        ${CMAKE_CURRENT_LIST_DIR}/widgets/PhoneEventProc.c
        ${CMAKE_CURRENT_LIST_DIR}/widgets/CallsignEventProc.c
        ${CMAKE_CURRENT_LIST_DIR}/widgets/CalibrationEventProc.c
        ${CMAKE_CURRENT_LIST_DIR}/widgets/TerminalEventProc.c
        ${CMAKE_CURRENT_LIST_DIR}/ISR.c
        ${CMAKE_CURRENT_LIST_DIR}/test.c
        )

pico_set_program_name(pico-widgets-test "pico-widgets-test")
pico_set_program_version(pico-widgets-test "0.15")

pico_enable_stdio_uart(pico-widgets-test 1)
#pico_enable_stdio_usb(pico-widgets-test 0)

#include_directories(${PROJECT_SOURCE_DIR}/lib)

# Add the standard include files to the build
target_include_directories(pico-widgets-test PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/pico-touchscr-sdk
        ${CMAKE_CURRENT_LIST_DIR}/pico-touchscr-sdk/ili9341
        ${CMAKE_CURRENT_LIST_DIR}/pico-touchscr-sdk/touch
        ${CMAKE_CURRENT_LIST_DIR}/frame
        ${CMAKE_CURRENT_LIST_DIR}/ui
        #${CMAKE_CURRENT_LIST_DIR}/widgets
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts or any other standard includes, if required
)

# Add any user requested libraries
target_link_libraries(
        pico-widgets-test
        pico_stdlib
        hardware_adc
        hardware_spi
        pico_sync
        hardware_timer
        hardware_clocks
        hardware_spi
        )

pico_add_extra_outputs(pico-widgets-test)
