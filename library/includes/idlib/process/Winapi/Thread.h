// Copyright (c) 2018 Michael Heilmann
// TODO: An n:r 1:w-lock is more efficient than a mutex. Ironically we implement the mutex using such a n:r 1:w-lock.
#pragma once

#include "Nucleus/Configuration.h"

#if (Nucleus_OperatingSystem == Nucleus_OperatingSystem_WINDOWS) && \
    !defined(Nucleus_Threads_Pthreads)

#include "Nucleus/Concurrency/Condition.h"
#include "Nucleus/Concurrency/Mutex.h"
#include "Nucleus/Concurrency/Thread.h"
#include "Nucleus/Concurrency/Shared/ThreadState.h"
#include "Nucleus/Types/Boolean.h"
#include "Nucleus/IncludesWindows.h"

typedef struct Nucleus_Concurrency_ThreadImpl Nucleus_Concurrency_ThreadImpl;

struct Nucleus_Concurrency_ThreadImpl
{
    Nucleus_Concurrency_Thread_CallbackContext *callbackContext;
    Nucleus_Concurrency_Thread_CallbackFunction *callbackFunction;
    Nucleus_Status status;
    Nucleus_Concurrency_Mutex mutex;
    HANDLE thread;
    Nucleus_Concurrency_ThreadState state;
}; // struct Nucleus_Concurrency_ThreadImpl

Nucleus_NonNull() Nucleus_Status
Nucleus_Concurrency_ThreadImpl_create
    (
        Nucleus_Concurrency_ThreadImpl **thread,
        Nucleus_Concurrency_Thread_CallbackContext *callbackContext,
        Nucleus_Concurrency_Thread_CallbackFunction *callbackFunction
    );

Nucleus_NonNull() void
Nucleus_Concurrency_ThreadImpl_destroy
    (
        Nucleus_Concurrency_ThreadImpl *thread
    );

Nucleus_NonNull() Nucleus_Status
Nucleus_Concurrency_ThreadImpl_join
    (
        Nucleus_Concurrency_ThreadImpl *thread
    );

Nucleus_NonNull() Nucleus_Status
Nucleus_Concurrency_ThreadImpl_getStatus
    (
        Nucleus_Concurrency_ThreadImpl *thread,
        Nucleus_Status *status
    );

Nucleus_NonNull() Nucleus_Status
Nucleus_Concurrency_ThreadImpl_start
    (
        Nucleus_Concurrency_ThreadImpl *thread
    );

#endif
