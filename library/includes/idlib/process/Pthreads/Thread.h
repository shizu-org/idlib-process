// Copyright (c) 2018 Michael Heilmann
#pragma once

#include "Nucleus/Configuration.h"

#if (Nucleus_OperatingSystem == Nucleus_OperatingSystem_LINUX)  || \
    (Nucleus_OperatingSystem == Nucleus_OperatingSystem_CYGWIN) || \
    (Nucleus_OperatingSystem == Nucleus_OperatingSystem_MACOS)  || \
    defined(Nucleus_Threads_Pthreads)

#include "Nucleus/Concurrency/Condition.h"
#include "Nucleus/Concurrency/Mutex.h"
#include "Nucleus/Concurrency/Thread.h"
#include "Nucleus/Concurrency/Shared/ThreadState.h"
#include "Nucleus/Types/Boolean.h"
#include <pthread.h>

typedef struct Nucleus_Concurrency_ThreadImpl Nucleus_Concurrency_ThreadImpl;

struct Nucleus_Concurrency_ThreadImpl
{
    Nucleus_Concurrency_ThreadState state;
    Nucleus_Concurrency_Thread_CallbackContext *callbackContext;
    Nucleus_Concurrency_Thread_CallbackFunction *callbackFunction;
    Nucleus_Status status;
    Nucleus_Concurrency_Mutex mutex;
    Nucleus_Concurrency_Condition condition;
    pthread_t thread;
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
