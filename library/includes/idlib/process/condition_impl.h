/*
  IdLib Process
  Copyright (C) 2018-2024 Michael Heilmann. All rights reserved.

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

#if !defined(IDLIB_PROCESS_CONDITION_IMPL_H_INCLUDED)
#define IDLIB_PROCESS_CONDITION_IMPL_H_INCLUDED

#include "idlib/process/configure.h"

#if (IDLIB_OPERATING_SYSTEM == IDLIB_OPERATING_SYSTEM_LINUX)  || \
    (IDLIB_OPERATING_SYSTEM == IDLIB_OPERATING_SYSTEM_CYGWIN) || \
    (IDLIB_OPERATING_SYSTEM == IDLIB_OPERATING_SYSTEM_MACOS)
  #include <pthread.h>
#elif (IDLIB_OPERATING_SYSTEM == IDLIB_OPERATING_SYSTEM_WINDOWS)
  #define WIN32_LEAN_AND_MEAN
  #include <Windows.h>
#else
  #error("operating system not (yet) supported")
#endif

typedef struct idlib_condition_impl {
#if (IDLIB_OPERATING_SYSTEM == IDLIB_OPERATING_SYSTEM_LINUX)  || \
    (IDLIB_OPERATING_SYSTEM == IDLIB_OPERATING_SYSTEM_CYGWIN) || \
    (IDLIB_OPERATING_SYSTEM == IDLIB_OPERATING_SYSTEM_MACOS)
  pthread_cond_t condition;
#elif (IDLIB_OPERATING_SYSTEM == IDLIB_OPERATING_SYSTEM_WINDOWS)
  CONDITION_VARIABLE condition_variable;
  CRITICAL_SECTION critical_section;
#else
  #error("operating system not (yet) supported")
#endif
} idlib_condition_impl;


#endif // IDLIB_PROCESS_CONDITION_IMPL_H_INCLUDED
