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

#include "idlib/process/thread.h"

#include "idlib/process/configure.h"
#include "idlib/process/status.h"
#include "idlib/process/mutex.h"

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

struct idlib_thread_impl {
  /// Pointer to a callback context object or the null pointer.
  idlib_thread_callback_context* callback_context;
  /// Pointer to the callback function.
  idlib_thread_callback_function* callback_function;
  idlib_mutex mutex;
#if (IDLIB_OPERATING_SYSTEM == IDLIB_OPERATING_SYSTEM_LINUX)  || \
    (IDLIB_OPERATING_SYSTEM == IDLIB_OPERATING_SYSTEM_CYGWIN) || \
    (IDLIB_OPERATING_SYSTEM == IDLIB_OPERATING_SYSTEM_MACOS)
  pthread_t backend;
#elif (IDLIB_OPERATING_SYSTEM == IDLIB_OPERATING_SYSTEM_WINDOWS)
  HANDLE backend;
#else
  #error("operating system not (yet) supported")
#endif
};

#if (IDLIB_OPERATING_SYSTEM == IDLIB_OPERATING_SYSTEM_LINUX)  || \
    (IDLIB_OPERATING_SYSTEM == IDLIB_OPERATING_SYSTEM_CYGWIN) || \
    (IDLIB_OPERATING_SYSTEM == IDLIB_OPERATING_SYSTEM_MACOS)
  static void *
  callback
    (
      void *p
    );
#elif (IDLIB_OPERATING_SYSTEM == IDLIB_OPERATING_SYSTEM_WINDOWS)
  static DWORD WINAPI
  callback
    (
      void *p
    );
#else
  #error("operating system not (yet) supported")
#endif

#if (IDLIB_OPERATING_SYSTEM == IDLIB_OPERATING_SYSTEM_LINUX)  || \
    (IDLIB_OPERATING_SYSTEM == IDLIB_OPERATING_SYSTEM_CYGWIN) || \
    (IDLIB_OPERATING_SYSTEM == IDLIB_OPERATING_SYSTEM_MACOS)
  static void *
  callback
    (
      void *p
    )
  {
  }
#elif (IDLIB_OPERATING_SYSTEM == IDLIB_OPERATING_SYSTEM_WINDOWS)    
  static DWORD WINAPI
  callback
    (
      void *p
    )
  {
    struct idlib_thread_impl *pimpl = (struct idlib_thread_impl*)p;
    InterlockedExchange8(&pimpl->state, IDLIB_THREAD_STATE_STARTED);
    pimpl->status = pimpl->callback_function(pimpl->callback_context);
    InterlockedExchange8(&pimpl->state, IDLIB_THREAD_TERMINATED);
    return pimpl;
  }
#else
  #error("operating system not (yet) supported")
#endif

int
idlib_thread_initialize
  (
    idlib_thread* thread,
    idlib_thread_callback_context* callback_context,
    idlib_thread_callback_function* callback_function
  )
{
  if (!thread || !callback_function) {
    return IDLIB_ARGUMENT_INVALID;
  }
  struct idlib_thread*_impl* pimpl = malloc(sizeof(struct idlib_thread_impl));
  if (!pimpl) {
     return IDLIB_ALLOCATION_FAILED;
  }
  pimpl->callback_context = callback_context;
  pimpl->callback_function = callback_function;
  pimpl->state = IDLIB_THREAD_STATE_INITIALIZED,
  int result = idlib_mutex_initialize(&pimpl->mutex);
  if (result) {
    free(pimpl);
    pimpl = NULL;
    return result;
  }
#if (IDLIB_OPERATING_SYSTEM == IDLIB_OPERATING_SYSTEM_LINUX)  || \
    (IDLIB_OPERATING_SYSTEM == IDLIB_OPERATING_SYSTEM_CYGWIN) || \
    (IDLIB_OPERATING_SYSTEM == IDLIB_OPERATING_SYSTEM_MACOS)
  if (pthread_create(&pimpl->thread, NULL, callback, (void*)pimpl)) {
    idlib_mutex_uninitialize(&pimpl->mutex);
    free(pimpl);
    pimpl = NULL;
    return IDLIB_ENVIRONMENT_FAILED;
  }
#elif (IDLIB_OPERATING_SYSTEM == IDLIB_OPERATING_SYSTEM_WINDOWS)
  pimpl->thread = CreateThread(NULL, 0, &callback, pimpl, CREATE_SUSPENDED, NULL);
  if (!pimpl->thread) {
    idlib_mutex_uninitialize(&pimpl->mutex);
    free(pimpl);
    pimpl = NULL;
    IDLIB_ENVIRONMENT_FAILED;
  }
#else
  #error("operating system not (yet) supported")
#endif
  return IDLIB_SUCCESS;
}

Nucleus_NonNull() void
Nucleus_Concurrency_Thread_uninitialize
    (
        Nucleus_Concurrency_Thread *thread
    )
{
    Nucleus_Concurrency_ThreadImpl_destroy(thread->impl);
    thread->impl = NULL;
}

Nucleus_NonNull() Nucleus_Status
Nucleus_Concurrency_Thread_join
    (
        Nucleus_Concurrency_Thread *thread
    )
{
    if (Nucleus_Unlikely(!thread)) return Nucleus_Status_InvalidArgument;
    return Nucleus_Concurrency_ThreadImpl_join(thread->impl);
}

Nucleus_NonNull() Nucleus_Status
Nucleus_Concurrency_Thread_getStatus
    (
        Nucleus_Concurrency_Thread *thread,
        Nucleus_Status *status
    )
{
    if (Nucleus_Unlikely(!thread)) return Nucleus_Status_InvalidArgument;
    return Nucleus_Concurrency_ThreadImpl_getStatus(thread->impl, status);
}

Nucleus_NonNull() Nucleus_Status
Nucleus_Concurrency_Thread_start
    (
        Nucleus_Concurrency_Thread *thread
    )
{
    if (Nucleus_Unlikely(!thread)) return Nucleus_Status_InvalidArgument;
    return Nucleus_Concurrency_ThreadImpl_start(thread->impl);
}
