#if !defined(IDLIB_PROCESS_H_INCLUDED)
#define IDLIB_PROCESS_H_INCLUDED

#include "idlib/process/configure.h"

/**
 * @brief Symbolic constant indicating success.
 * This constant is the only error constant guaranteed to evaluate to the value zero.
 */
#define IDLIB_SUCCESS (0)

/**
 * @brief Symbolic constant indicating an unknown error.
 * The error was raised, however, the reason is not known.
 */
#define IDLIB_UNKNOWN_ERROR (1)

/**
 * @brief Symbolic constant indicating an error.
 * The error was raised because an argument was not valid.
 */
#define IDLIB_ARGUMENT_INVALID (2)

/**
 * @brief Symbolic constant indicating an error.
 * The error was raised because something was aborted.
 */
#define IDLIB_ABORTED (4)

/**
 * @brief Symbolic constant indicating an error.
 * The error was raised because something was locked.
 */
#define IDLIB_LOCKED (5)

/**
 * @brief Symbolic constant indicating an error.
 * The error was raised because something was not locked.
 */
#define IDLIB_NOT_LOCKED (6)

/**
 * @brief Symbolic constant indicating an error.
 * The error was raised because something locking failed.
 */
#define IDLIB_LOCK_FAILED (7)

/**
 * @brief Symbolic constant indicating an error.
 * The error was raised because an allocation failed.
 */
#define IDLIB_ALLOCATION_FAILED (8)

/** 
 * @brief Symbolic constant indicating an error.
 * The error was raised because something was too big.
 */
#define IDLIB_TOO_BIG (9)

/**
 * @brief Symbolic constant indicating an error.
 * The error was raised because something was too big.
 */
#define IDLIB_TOO_SMALL (10)

/**
 * @brief Symbolic constant indicating an error.
 * The error was raised because an overflow occurred.
 */
#define IDLIB_OVERFOW (11)

/**
 * @brief Symbolic constant indicating an error.
 * The error was raised because an underflow occurred.
 */
#define IDLIB_UNDERFLOW (12)

/**
 * @brief Symbolic constant indicating an error.
 * The error was raised because something exists.
 */
#define IDLIB_EXISTS (13)

/**
 * @brief Symbolic constant indicating an error.
 * The error was raised because something not exists.
 */
#define IDLIB_NOT_EXISTS (14)

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
int
idlib_acquire_process
  (
    idlib_process** process
  );

/**
 * @since 1.0
 * Relinquish a reference to the process singleton.
 * @param process A pointer to an idlib_process value.
 * @return #IDLIB_SUCCESS on success. A non-zero return value on failure.
 */
int
idlib_relinquish_process
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
