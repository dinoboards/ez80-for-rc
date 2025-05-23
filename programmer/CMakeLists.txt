# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(PICO_COPY_TO_RAM,true)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NEVER EDIT THE NEXT LINES for Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.0.0)
set(toolchainVersion 13_2_Rel1)
set(picotoolVersion 2.0.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico CACHE STRING "Board type")
# set(PICO_COPY_TffO_RAM true CACHE BOOL "Copy to RAM")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(programmer C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

# Set optimization level for all C files
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")

# Set optimization level for all C++ files (if applicable)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

# Define the path to the .hex file and the generated .c file
set(HEX_FILE_PATH "../ez80-rc-firmware-release.hex")
set(GENERATED_C_FILE "./ez80_rc_firmware_release.c")


# Add a custom command to generate the .c file from the .hex file
add_custom_command(
    OUTPUT ${GENERATED_C_FILE}
    COMMAND ${CMAKE_COMMAND} -E echo "Converting ${HEX_FILE_PATH} to ${GENERATED_C_FILE}"
    COMMAND bash -c "../convert_hex_to_c.sh ${HEX_FILE_PATH} ${GENERATED_C_FILE}"
    DEPENDS ${HEX_FILE_PATH}
)

# Define the path to the .inc file and the generated .h file
set(INC_FILE_PATH "../firmware-version.inc")
set(GENERATED_H_FILE "../firmware_version.h")

# Add a custom command to generate the .inc file from the .h file
add_custom_command(
    COMMAND echo "-----------------"
    OUTPUT ${GENERATED_H_FILE}
    COMMAND ${CMAKE_COMMAND} -E echo "Converting ${INC_FILE_PATH} to ${GENERATED_H_FILE}"
    COMMAND bash -c "../convert_inc_to_c.sh ${INC_FILE_PATH} ${GENERATED_H_FILE}"
    DEPENDS ${INC_FILE_PATH}
    COMMAND echo "Version Embedded: "
    COMMAND cat ${INC_FILE_PATH}
)

set_source_files_properties(
  main.c
  read_line.c
  zdi.c
  command_dispatcher.c
  hex_record.c
  ifl.c
  utils.c
  ez80_rc_firmware_release.c
  PROPERTIES COMPILE_OPTIONS "-Werror;-Wall;-Wextra;-O3"
)

add_executable(programmer
    main.c
    read_line.c
    zdi.c
    command_dispatcher.c
    hex_record.c
    ifl.c
    utils.c
    firmware_version.h
    ez80_rc_firmware_release.c
)

# pull in common dependencies
target_link_libraries(programmer pico_stdlib)

if (PICO_CYW43_SUPPORTED)
    target_link_libraries(programmer pico_cyw43_arch_none)
endif()

# create map/bin/hex file etc.
pico_add_extra_outputs(programmer)

# add url via pico_set_program_url
pico_enable_stdio_usb(programmer 1)
pico_enable_stdio_uart(programmer 0)

