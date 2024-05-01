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

#include "idlib/process/condition.h"

#include "idlib/process/condition_impl.h"

// malloc, free
#include <malloc.h>

idlib_status
idlib_condition_intialize
  (
    idlib_condition* condition
  )
{
  if (!condition) {
    return IDLIB_ARGUMENT_INVALID;
  }
  idlib_condition_impl* pimpl = malloc(sizeof(idlib_condition_impl));
  if (!pimpl) {
    return IDLIB_ALLOCATION_FAILED;
  }
#if (IDLIB_OPERATING_SYSTEM == IDLIB_OPERATING_SYSTEM_LINUX)  || \
    (IDLIB_OPERATING_SYSTEM == IDLIB_OPERATING_SYSTEM_CYGWIN) || \
    (IDLIB_OPERATING_SYSTEM == IDLIB_OPERATING_SYSTEM_MACOS)
  if (pthread_cond_init(&pimpl->condition, NULL)) {
    free(pimpl);
    pimpl = NULL;
    return IDLIB_ENVIRONMENT_FAILED;
  }
#elif (IDLIB_OPERATING_SYSTEM == IDLIB_OPERATING_SYSTEM_WINDOWS)
  InitializeConditionVariable(&pimpl->condition_variable);
#else
  #error("operating system not (yet) supported")
#endif
  condition->pimpl = pimpl;
  return IDLIB_SUCCESS;
}

idlib_status
idlib_condition_uninitialize
  (
    idlib_condition* condition
  )
{
  if (!condition) {
    return IDLIB_ARGUMENT_INVALID;
  }
  idlib_condition_impl* pimpl = (idlib_condition_impl*)condition->pimpl;
  condition->pimpl = NULL;
#if (IDLIB_OPERATING_SYSTEM == IDLIB_OPERATING_SYSTEM_LINUX)  || \
    (IDLIB_OPERATING_SYSTEM == IDLIB_OPERATING_SYSTEM_CYGWIN) || \
    (IDLIB_OPERATING_SYSTEM == IDLIB_OPERATING_SYSTEM_MACOS)
  pthread_cond_destroy(&pimpl->condition);
#elif (IDLIB_OPERATING_SYSTEM == IDLIB_OPERATING_SYSTEM_WINDOWS)
  /* Intentionally empty. */
#else
  #error("operating system not (yet) supported")
#endif
  free(pimpl);
  pimpl = NULL;
  return IDLIB_SUCCESS;
}

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
