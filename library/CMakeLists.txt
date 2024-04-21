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

include(${idlib-process-cmake-files-dir}/all.cmake)

set(name idlib-process)
begin_library()

if (${${name}.compiler_c} STREQUAL ${${name}.compiler_c_msvc})
  set("IDLIB_COMPILER_C" "IDLIB_COMPILER_C_MSVC")
elseif (${${name}.compiler_c} STREQUAL ${${name}.compiler_c_gcc})
  set("IDLIB_COMPILER_C" "IDLIB_COMPILER_C_GCC")
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

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/includes/idlib/process/configure.h.in ${CMAKE_CURRENT_BINARY_DIR}/includes/idlib/process/configure.h)

list(APPEND ${name}.configuration_files "${CMAKE_CURRENT_BINARY_DIR}/includes/idlib/process/configure.h")

list(APPEND ${name}.header_files "${CMAKE_CURRENT_SOURCE_DIR}/includes/idlib/process.h")
list(APPEND ${name}.source_files "${CMAKE_CURRENT_SOURCE_DIR}/sources/idlib/process.c")

list(APPEND ${name}.header_files "${CMAKE_CURRENT_SOURCE_DIR}/includes/idlib/process/status.h")
list(APPEND ${name}.source_files "${CMAKE_CURRENT_SOURCE_DIR}/sources/idlib/process/status.c")
end_library()

source_group(TREE ${CMAKE_CURRENT_BINARY_DIR} FILES ${${name}.configuration_files})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${${name}.header_files})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${${name}.source_files})

# We must link libm under Linux.
if (${${name}.operating_system_id} EQUAL ${${name}.operating_system_id_linux})
  target_link_libraries(${name} m)
endif()
