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

// TODO:
// - Linux version not yet thread-safe.

#define IDLIB_PROCESS_PRIVATE (1)
#include "idlib/process.h"

// fprintf, stderr
#include <stdio.h>

// malloc, free
#include <malloc.h>

// memcmp, memcpy
#include <string.h>

#if (IDLIB_OPERATING_SYSTEM == IDLIB_OPERATING_SYSTEM_LINUX)  || \
    (IDLIB_OPERATING_SYSTEM == IDLIB_OPERATING_SYSTEM_CYGWIN) || \
    (IDLIB_OPERATING_SYSTEM == IDLIB_OPERATING_SYSTEM_MACOS)

  // uint64_t
  #include <stdint.h>

  #include <pthread.h>

#elif IDLIB_OPERATING_SYSTEM_WINDOWS == IDLIB_OPERATING_SYSTEM

  #define WIN32_LEAN_AND_MEAN
  #include <Windows.h>

  // uint64_t
  #include <stdint.h>

#else

  #error("operating system not (yet) supported")

#endif

typedef struct _entry _entry;

typedef struct _entries _entries;

struct _entry {
  _entry* next;
  void *p;
  size_t n;
  void* v;
};

struct _entries {
  _entry* entries;
};

struct idlib_process {
  uint64_t reference_count;
  _entries entries; 
};

static idlib_status
initialize_entries(_entries* entries) {
  entries->entries = NULL;
  return IDLIB_SUCCESS;
}

static idlib_status
uninitialize_entries(_entries* entries) {
  return IDLIB_SUCCESS;
}

static idlib_process* g = NULL;

#if (IDLIB_OPERATING_SYSTEM == IDLIB_OPERATING_SYSTEM_LINUX)  || \
    (IDLIB_OPERATING_SYSTEM == IDLIB_OPERATING_SYSTEM_CYGWIN) || \
    (IDLIB_OPERATING_SYSTEM == IDLIB_OPERATING_SYSTEM_MACOS)

  static pthread_mutex_t g_lock = PTHREAD_MUTEX_INITIALIZER;

#elif IDLIB_OPERATING_SYSTEM_WINDOWS == IDLIB_OPERATING_SYSTEM

  // The system will free this handle automatically when the process exits.
  static volatile HANDLE g_lock = NULL;

  __declspec(dllexport) idlib_status
  acquire_impl
    (
      idlib_process** process
    )
  {
    if (!process) {
      return IDLIB_ARGUMENT_INVALID;
    }
    if (!InterlockedCompareExchangePointer((volatile void*)&g_lock,NULL, NULL)) {
      HANDLE mutex = CreateMutex(NULL, FALSE, NULL);
      if (!mutex) {
        return IDLIB_ENVIRONMENT_FAILED;
      }
      if (NULL != InterlockedCompareExchangePointer((volatile void*)&g_lock, (void*)mutex, NULL)) {
        CloseHandle(mutex);
        mutex = NULL;
      }
    }
    if (WAIT_FAILED == WaitForSingleObject(InterlockedCompareExchangePointer((volatile void*)&g_lock, NULL, NULL), INFINITE)) {
      return IDLIB_LOCKED;
    }
    if (!g) {
      idlib_process* p = malloc(sizeof(idlib_process));
      if (!p) {
        ReleaseMutex(InterlockedCompareExchangePointer((volatile void*)&g_lock, NULL, NULL));
        return IDLIB_ALLOCATION_FAILED;
      }
      initialize_entries(&p->entries);
      p->reference_count = 0;
      g = p;
    }
    if (UINT64_MAX == g->reference_count) {
      ReleaseMutex(InterlockedCompareExchangePointer((volatile void*)&g_lock, NULL, NULL));
      return IDLIB_OVERFLOW;
    }
    g->reference_count++;
    *process = g;
    ReleaseMutex(InterlockedCompareExchangePointer((volatile void*)&g_lock, NULL, NULL));
    return IDLIB_SUCCESS;
  }

  __declspec(dllexport) idlib_status
  relinquish_impl
    (
      idlib_process** process
    )
  {
    if (WAIT_FAILED == WaitForSingleObject(InterlockedCompareExchangePointer((volatile void*)&g_lock, NULL, NULL), INFINITE)) {
      return IDLIB_LOCKED;
    }
    if (!g) {
      return IDLIB_OPERATION_INVALID;
    }
    if (0 == g->reference_count) {
      ReleaseMutex(InterlockedCompareExchangePointer((volatile void*)&g_lock, NULL, NULL));
      return IDLIB_UNDERFLOW;
    }
    if (0 == --g->reference_count) {
      uninitialize_entries(&g->entries);
      free(g);
      g = NULL;
    }
    ReleaseMutex(InterlockedCompareExchangePointer((volatile void*)&g_lock, NULL, NULL));
    return IDLIB_SUCCESS;
  }

#else

  #error("operating system not (yet) supported")

#endif

idlib_status
idlib_process_acquire
  (
    idlib_process** process
  )
{
  if (!process) {
    return IDLIB_ARGUMENT_INVALID;
  }
#if (IDLIB_OPERATING_SYSTEM == IDLIB_OPERATING_SYSTEM_LINUX)  || \
    (IDLIB_OPERATING_SYSTEM == IDLIB_OPERATING_SYSTEM_CYGWIN) || \
    (IDLIB_OPERATING_SYSTEM == IDLIB_OPERATING_SYSTEM_MACOS)

  if (pthread_mutex_lock(&g_lock)) {
    return IDLIB_LOCK_FAILED;
  }
  
  if (!g) {
    g = malloc(sizeof(idlib_process));
    if (!g) {
      pthread_mutex_unlock(&g_lock);
      return IDLIB_ALLOCATION_FAILED;
    }
    initialize_entries(&g->entries);
    g->reference_count = 0;
  }
  if (UINT64_MAX == g->reference_count) {
    pthread_mutex_unlock(&g_lock);
    return IDLIB_OVERFLOW;
  }
  g->reference_count++;
  *process = g;
  pthread_mutex_unlock(&g_lock);

#elif IDLIB_OPERATING_SYSTEM_WINDOWS == IDLIB_OPERATING_SYSTEM

  // We use GetModuleHandle(NULL) for that we always use the same instance of acquire_impl
  // that always uses the same instance of g_lock.
  HMODULE module = GetModuleHandle(NULL);
  if (!module) {
    return IDLIB_ENVIRONMENT_FAILED;
  }
  idlib_status (*f)(idlib_process**) = (idlib_status(*)(idlib_process**))GetProcAddress(module, "acquire_impl");
  if (!f) {
    return IDLIB_NOT_EXISTS;
  }
  return (*f)(process);

#else

  #error("operating system not (yet) supported")

#endif

  return IDLIB_SUCCESS;
}

idlib_status
idlib_process_relinquish
  (
    idlib_process* process
  )
{

#if (IDLIB_OPERATING_SYSTEM == IDLIB_OPERATING_SYSTEM_LINUX)  || \
    (IDLIB_OPERATING_SYSTEM == IDLIB_OPERATING_SYSTEM_CYGWIN) || \
    (IDLIB_OPERATING_SYSTEM == IDLIB_OPERATING_SYSTEM_MACOS)

  if (pthread_mutex_lock(&g_lock)) {
    return IDLIB_LOCK_FAILED;
  }
  if (!g) {
    pthread_mutex_unlock(&g_lock);
    return IDLIB_OPERATION_INVALID;
  }
  if (0 == g->reference_count) {
    pthread_mutex_unlock(&g_lock);
    return IDLIB_UNDERFLOW;
  }
  if (0 == --g->reference_count) {
    uninitialize_entries(&g->entries);
    free(g);
    g = NULL;
  }
  pthread_mutex_unlock(&g_lock);

#elif IDLIB_OPERATING_SYSTEM_WINDOWS == IDLIB_OPERATING_SYSTEM

  // We use GetModuleHandle(NULL) for that we always use the same instance of relinquish_impl
  // that always uses the same instance of g_lock.
  HMODULE module = GetModuleHandle(NULL);
  if (!module) {
    return IDLIB_ENVIRONMENT_FAILED;
  }
  idlib_status(*f)(idlib_process*) = (idlib_status (*)(idlib_process*))GetProcAddress(module, "relinquish_impl");
  if (!f) {
    return IDLIB_NOT_EXISTS;
  }
  return (*f)(process);

#else

  #error("operating system not (yet) supported")

#endif

  return IDLIB_SUCCESS;
}

idlib_status
idlib_add_global
  (
    idlib_process* process,
    void const* p,
    size_t n,
    void* v
  )
{
  _entry* entry = process->entries.entries;
  while (entry) {
    if (entry->n == n) {
      if (!memcmp(entry->p, p, n)) {
        return IDLIB_EXISTS;
      }
    }
    entry = entry->next;
  }
  entry = malloc(sizeof(_entry));
  if (!entry) {
    return IDLIB_ALLOCATION_FAILED;
  }
  entry->p = malloc(n > 0 ? n : 1);
  if (!entry->p) {
    free(entry);
    entry = NULL;
    return IDLIB_ALLOCATION_FAILED;
  }
  memcpy(entry->p, p, n);
  entry->n = n;
  entry->v = v;
  entry->next = process->entries.entries;
  process->entries.entries = entry;
  return IDLIB_SUCCESS;
}
 
idlib_status
idlib_get_global
  (
    idlib_process* process,
    void const* p,
    size_t n,
    void** v
  )
{
  _entry* entry = process->entries.entries;
  while (entry) {
    if (entry->n == n) {
      if (!memcmp(entry->p, p, n)) {
        *v = entry->v;
        return IDLIB_SUCCESS;
      }
    }
    entry = entry->next;
  }
  return IDLIB_NOT_EXISTS;
}

idlib_status
idlib_remove_global
  (
    idlib_process* process,
    void const* p,
     size_t n
  )
{
  if (!process || !p) {
    return IDLIB_ARGUMENT_INVALID;
  }
  _entry** previous = &process->entries.entries;
  _entry* current = process->entries.entries;
  while (current) {
    if (current->n == n) {
      if (!memcmp(current->p, p, n)) {
        _entry* node = current;
        *previous = current->next;
        free(node);
        return IDLIB_SUCCESS;
      }
    }
    previous = &current->next;
    current = current->next;
  }
  return IDLIB_NOT_EXISTS;
}
