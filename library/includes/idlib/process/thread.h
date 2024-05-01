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

#if !defined(IDLIB_PROCESS_THREAD_H_INCLUDED)
#define IDLIB_PROCESS_THREAD_H_INCLUDED

#include "idlib/process/configure.h"
#include <inttypes.h>

#define IDLIB_THREAD_STATE_INITIALIZED (0)

#define IDLIB_THREAD_STATE_STARTED (1)

#define IDLIB_THREAD_STATE_TERMINATED (2)

typedef uin8_t idlib_thread_state;

typedef void idlib_thread_callback_context;

typedef bool (idlib_thread_callback_function)(idlib_thread_callback_context* context);

typedef struct idlib_thread idlib_thread;

struct idlib_thread {
  void* pimpl;
}; // struct idlib_thread

int
idlib_thread_initialize
  (
    idlib_thread* thread,
    idlib_thread_callback_context* context,
    idlib_thread_callback_function* function
  );

int
idlib_thread_uninitialize
  (
    idlib_thread* thread
  );

/*
 * This function returns immediatly if the thread is not started yet or terminated already.
 */
int
idlib_thread_join
  (
    idlib_thread* thread
  );

/*
 * The function returns immediatly if the thread started already or terminated already.
 * In the first case `Nucleus_Status_AlreadyStarted` is returned, in the second case `Nucleus_Status_AlreadyStopped` is returned.
 * This function is thread-safe.
 */
int
idlib_thread_start
  (
    idlib_thread* thread
  );
