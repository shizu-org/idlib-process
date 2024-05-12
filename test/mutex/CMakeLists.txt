#
# IdLib Process
# Copyright (C) 2018-2024 Michael Heilmann. All rights reserved.
#
# This software is provided 'as-is', without any express or implied
# warranty.  In no event will the authors be held liable for any damages
# arising from the use of this software.
#
# Permission is granted to anyone to use this software for any purpose,
# including commercial applications, and to alter it and redistribute it
# freely, subject to the following restrictions:
#
# 1. The origin of this software must not be misrepresented; you must not
#    claim that you wrote the original software. If you use this software
#    in a product, an acknowledgment in the product documentation would be
#    appreciated but is not required.
# 2. Altered source versions must be plainly marked as such, and must not be
#    misrepresented as being the original software.
# 3. This notice may not be removed or altered from any source distribution.
#

cmake_minimum_required(VERSION 3.20)

include(${idlib-process.source-dir}/cmake/all.cmake)

set(name idlib-process.test.mutex)
begin_executable()

if (${${name}.compiler_c} STREQUAL ${${name}.compiler_c_msvc})
  set("IDLIB_COMPILER_C" "IDLIB_COMPILER_C_MSVC")
elseif (${${name}.compiler_c} STREQUAL ${${name}.compiler_c_gcc})
  set("IDLIB_COMPILER_C" "IDLIB_COMPILER_C_GCC")
elseif (${${name}.compiler_c} STREQUAL ${${name}.compiler_c_clang})
  set("IDLIB_COMPILER_C" "IDLIB_COMPILER_C_CLANG")
elseif (${${name}.compiler_c} STREQUAL ${${name}.compiler_c_unknown})
  set("IDLIB_COMPILER_C" "IDLIB_COMPILER_C_UNKNOWN")
else()
  message(FATAL_ERROR "C compiler detection not executed")
endif()

if (${${name}.instruction_set_architecture} STREQUAL ${${name}.instruction_set_architecture_x64})
  set("IDLIB_INSTRUCTION_SET_ARCHITECTURE" "IDLIB_INSTRUCTION_SET_ARCHITECTURE_X64")
elseif (${${name}.instruction_set_architecture} STREQUAL ${${name}.instruction_set_architecture_x86})
  set("IDLIB_INSTRUCTION_SET_ARCHITECTURE" "IDLIB_INSTRUCTION_SET_ARCHITECTURE_X86")
elseif (${${name}.instruction_set_architecture} STREQUAL ${${name}.instruction_set_architecture_unknown})
  set("IDLIB_INSTRUCTION_SET_ARCHITECTURE" "IDLIB_INSTRUCTION_SET_ARCHITECTURE_UNKNOWN")
else()
  message(FATAL_ERROR "instruction set architecture detection not executed")
endif()

if (${${name}.operating_system} STREQUAL ${${name}.operating_system_windows})
  set("IDLIB_OPERATING_SYSTEM" "IDLIB_OPERATING_SYSTEM_WINDOWS")
elseif (${${name}.operating_system} STREQUAL ${${name}.operating_system_linux})
  set("IDLIB_OPERATING_SYSTEM" "IDLIB_OPERATING_SYSTEM_LINUX")
elseif (${${name}.operating_system} STREQUAL ${${name}.operating_system_cygwin})
  set("IDLIB_OPERATING_SYSTEM" "IDLIB_OPERATING_SYSTEM_CYGWIN")
elseif (${${name}.operating_system} STREQUAL ${${name}.operating_system_unknown})
  set("IDLIB_OPERATING_SYSTEM" "IDLIB_OPERATING_SYSTEM_UNKNOWN")
else()
  message(FATAL_ERROR "operating system detection not executed")
endif()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/includes/configure.h.in ${CMAKE_CURRENT_BINARY_DIR}/includes/configure.h)

list(APPEND ${name}.configuration_files "${CMAKE_CURRENT_BINARY_DIR}/includes/configure.h")
list(APPEND ${name}.source_files "${CMAKE_CURRENT_SOURCE_DIR}/sources/main.c")

end_executable()

source_group(TREE ${CMAKE_CURRENT_BINARY_DIR} FILES ${${name}.configuration_files})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${${name}.header_files})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${${name}.source_files})

target_link_libraries(${name} PRIVATE idlib-process)

if (${${name}.compiler_c} STREQUAL ${${name}.compiler_c_msvc})
  set_property(TARGET ${name} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:${name}>")
endif()

add_test(NAME ${name}
         WORKING_DIRECTORY $<TARGET_FILE_DIR:${name}>
         COMMAND ${name})

# Copy the assets to the current binary directory.
file(GLOB_RECURSE files_to_copy RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/assets" "${CMAKE_CURRENT_SOURCE_DIR}/assets/*.*" )

foreach (file_to_copy ${files_to_copy})
  # Copy the test data into the SAME directory in which the executable resides in by using the generator expression $<TARGET_FILE_DIR:${name}>.
  add_custom_command(
    TARGET ${name} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/assets/${file_to_copy}"
                                                   "$<TARGET_FILE_DIR:${name}>/assets/${file_to_copy}"
    COMMAND_EXPAND_LISTS
  )
endforeach()
