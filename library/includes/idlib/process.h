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

#if !defined(IDLIB_PROCESS_H_INCLUDED)
#define IDLIB_PROCESS_H_INCLUDED

#include "idlib/process/configure.h"
#include "idlib/process/status.h"
#include "idlib/process/mutex.h"

#if IDLIB_OPERATING_SYSTEM_LINUX == IDLIB_OPERATING_SYSTEM || IDLIB_OPERATING_SYSTEM_CYGWIN == IDLIB_OPERATING_SYSTEM

  // size_t
  #include <stddef.h>

#endif

/**
 * @since 1.0
 * @brief The opaque type of the process singleton.
 */
typedef struct idlib_process idlib_process;

/**
 * @since 1.0
 * Acquire a reference to the process singleton.
 * @param process A pointer to a <code>idlib_process*</code> variable.
 * @return #IDLIB_SUCCESS on success. A non-zero return value on failure.
 * @success <code>*process</code> was a assigned a pointer to the idlib_process_manager value singleton.
 */
idlib_status
idlib_process_acquire
  (
    idlib_process** process
  );

/**
 * @since 1.0
 * Relinquish a reference to the process singleton.
 * @param process A pointer to an idlib_process value.
 * @return #IDLIB_SUCCESS on success. A non-zero return value on failure.
 */
idlib_status
idlib_process_relinquish
  (
    idlib_process* process
  );


/**
 * @since 1.0
 * Add an entry for the specified key and the specified value.
 * @param p A pointer to a sequence of <code>n</code> Bytes.
 * @param n The number of Bytes in the array pointed to by <code>p</code>.
 * @param v The value.
 * @return #IDLIB_SUCCESS on success. A non-zero return value on failure.
 * In particular, this function returns
 * - IDLIB_ARGUMENT_INVALID if `process`, `p`, or `value` is null
 * - IDLIB_EXISTS if an entry for the key (`p`, `n`) exists
 * @remarks 
 * This function is mt-safe.
 */
int
idlib_add_global
  (
    idlib_process* process,
    void const* p,
    size_t n,
    void* v
  );


/**
 * @since 1.0
 * Get a pointer to the value of the entry of the specified key.
 * @param p A pointer to a sequence of <code>n</code> Bytes.
 * @param n The number of Bytes in the array pointed to by <code>p</code>.
 * @param v [out] A pointer to a `void*` variable.
 * @return #IDLIB_SUCCESS on success. A non-zero return value on failure.
 * In particular, this function returns
 * - IDLIB_ARGUMENT_INVALID if `process`, `p`, or `value` is null
 * - IDLIB_NOT_EXISTS if no entry for the key (`p`, `n`) was found
 * @remarks 
 * This function is mt-safe.
 */
int
idlib_get_global
  (
    idlib_process* process,
    void const* p,
    size_t n,
    void** v
  );

/**
 * @since 1.0
 * Remove a global stored under the specified key.
 * @param p A pointer to a sequence of <code>n</code> Bytes.
 * @param n The number of Bytes in the array pointed to by <code>p</code>.
 * @return #IDLIB_SUCCESS on success. A non-zero value on failure.
 * In particular, this function returns
 * - IDLIB_ARGUMENT_INVALID if `process` or `p` is null
 * - IDLIB_NOT_EXISTS if no global is registered for the key `p` and `n` 
 */
int
idlib_remove_global
  (
    idlib_process* process,
    void const* p,
    size_t n
  );

#endif // IDLIB_PROCESS_H_INCLUDED
