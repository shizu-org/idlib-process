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

#include "idlib/process/mutex.h"

#include "idlib/process/mutex_impl.h"

#include <malloc.h>

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

idlib_status
idlib_mutex_initialize
  (
    idlib_mutex* mutex
  )
{
  if (!mutex) {
    return IDLIB_ARGUMENT_INVALID;
  }
  idlib_mutex_impl* pimpl = malloc(sizeof(idlib_mutex_impl));
  if (!pimpl) {
    return IDLIB_ALLOCATION_FAILED;
  }
#if (IDLIB_OPERATING_SYSTEM == IDLIB_OPERATING_SYSTEM_LINUX)  || \
    (IDLIB_OPERATING_SYSTEM == IDLIB_OPERATING_SYSTEM_CYGWIN) || \
    (IDLIB_OPERATING_SYSTEM == IDLIB_OPERATING_SYSTEM_MACOS)
  pthread_mutexattr_t attr;
  pthread_mutexattr_init(&attr);
  pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
  int result = pthread_mutex_init(&pimpl->mtx, &attr);
  pthread_mutexattr_destroy(&attr);
  if (result) {
    free(pimpl);
    pimpl = NULL;
    return IDLIB_ENVIRONMENT_FAILED;
  }
  mutex->pimpl = pimpl;
#elif (IDLIB_OPERATING_SYSTEM == IDLIB_OPERATING_SYSTEM_WINDOWS)
  pimpl->mtx = CreateMutex(NULL, FALSE, NULL);
  if (!pimpl->mtx) {
    free(pimpl);
    pimpl = NULL;
    return IDLIB_ENVIRONMENT_FAILED;
  }
  mutex->pimpl = pimpl;
#else
  #error("operating system not (yet) supported")
#endif
  return IDLIB_SUCCESS;
}

idlib_status
idlib_mutex_uninitialize
  (
    idlib_mutex* mutex
  )
{
  if (!mutex) {
    return IDLIB_ARGUMENT_INVALID;
  }
  idlib_mutex_impl* pimpl = (idlib_mutex_impl*)mutex->pimpl;
  mutex->pimpl = NULL;
#if (IDLIB_OPERATING_SYSTEM == IDLIB_OPERATING_SYSTEM_LINUX)  || \
    (IDLIB_OPERATING_SYSTEM == IDLIB_OPERATING_SYSTEM_CYGWIN) || \
    (IDLIB_OPERATING_SYSTEM == IDLIB_OPERATING_SYSTEM_MACOS)
  pthread_mutex_destroy(&pimpl->mtx);
#elif (IDLIB_OPERATING_SYSTEM == IDLIB_OPERATING_SYSTEM_WINDOWS)
  CloseHandle(pimpl->mtx);
  pimpl->mtx = NULL;
#else
  #error("operating system not (yet) supported")
#endif
  free(pimpl);
  pimpl = NULL;
  return IDLIB_SUCCESS;
}

idlib_status
idlib_mutex_lock
  (
    idlib_mutex* mutex
  )
{
  if (!mutex) {
    return IDLIB_ARGUMENT_INVALID;
  }
  idlib_mutex_impl* pimpl = (idlib_mutex_impl*)mutex->pimpl;
#if (IDLIB_OPERATING_SYSTEM == IDLIB_OPERATING_SYSTEM_LINUX)  || \
    (IDLIB_OPERATING_SYSTEM == IDLIB_OPERATING_SYSTEM_CYGWIN) || \
    (IDLIB_OPERATING_SYSTEM == IDLIB_OPERATING_SYSTEM_MACOS)
  int result = pthread_mutex_lock(&pimpl->mtx);
  if (result) {
    return IDLIB_LOCK_FAILED;
  }
#elif (IDLIB_OPERATING_SYSTEM == IDLIB_OPERATING_SYSTEM_WINDOWS)
  DWORD result = WaitForSingleObject(pimpl->mtx, INFINITE);
  switch (result) {
    case WAIT_OBJECT_0: {
      return IDLIB_SUCCESS;
    } break;
    default: {
      return IDLIB_LOCK_FAILED;
    } break;
  };
#else
  #error("operating system not (yet) supported")
#endif
}

idlib_status
idlib_mutex_unlock
  (
    idlib_mutex* mutex
  )
{
  if (!mutex) {
    return IDLIB_ARGUMENT_INVALID;
  }
  idlib_mutex_impl* pimpl = (idlib_mutex_impl*)mutex->pimpl;
#if (IDLIB_OPERATING_SYSTEM == IDLIB_OPERATING_SYSTEM_LINUX)  || \
    (IDLIB_OPERATING_SYSTEM == IDLIB_OPERATING_SYSTEM_CYGWIN) || \
    (IDLIB_OPERATING_SYSTEM == IDLIB_OPERATING_SYSTEM_MACOS)
  pthread_mutex_unlock(&pimpl->mtx);
#elif (IDLIB_OPERATING_SYSTEM == IDLIB_OPERATING_SYSTEM_WINDOWS)
  ReleaseMutex(pimpl->mtx);
#else
  #error("operating system not (yet) supported")
#endif
  return IDLIB_SUCCESS;
}
