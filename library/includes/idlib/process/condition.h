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

#if !defined(IDLIB_PROCESS_CONDITION_H_INCLUDED)
#define IDLIB_PROCESS_CONDITION_H_INCLUDED

#include "idlib/process/configure.h"
#include "idlib/process/status.h"
typedef struct idlib_condition idlib_condition;
typedef struct idlib_mutex idlib_mutex;

// The type of a condition.
typedef struct idlib_condition idlib_condition;

struct idlib_condition {
  void* pimpl;
}; // struct idlib_condition

idlib_status
idlib_condition_intialize
  (
    idlib_condition* condition
  );

idlib_status
idlib_condition_uninitialize
  (
    idlib_condition* condition
  );

idlib_status
idlib_condition_wait
  (
    idlib_condition* condition,
    idlib_mutex* mutex
  );

idlib_status
idlib_condition_signal_one
  (
    idlib_condition* condition
  );

idlib_status
idlib_condition_signal_all
  (
    idlib_condition* condition
  );

#endif  // IDLIB_PROCESS_CONDITION_H_INCLUDED
