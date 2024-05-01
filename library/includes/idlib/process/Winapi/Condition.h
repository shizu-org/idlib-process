// Copyright (c) 2018 Michael Heilmann
#pragma once

#include "Nucleus/Configuration.h"

#if (Nucleus_OperatingSystem == Nucleus_OperatingSystem_WINDOWS) && \
    !defined(Nucleus_Threads_Pthreads)

#include "Nucleus/Concurrency/Condition.h"
#include "Nucleus/IncludesWindows.h"

// Forward declaration.
typedef struct Nucleus_Concurrency_MutexImpl Nucleus_Concurrency_MutexImpl;

typedef struct Nucleus_Concurrency_ConditionImpl Nucleus_Concurrency_ConditionImpl;

struct Nucleus_Concurrency_ConditionImpl
{
    CONDITION_VARIABLE conditionVariable;
    CRITICAL_SECTION criticalSection;
}; // Nucleus_Concurrency_ConditionImpl

Nucleus_NonNull() Nucleus_Status
Nucleus_Concurrency_ConditionImpl_create
    (
        Nucleus_Concurrency_ConditionImpl **condition
    );

Nucleus_NonNull() void
Nucleus_Concurrency_ConditionImpl_destroy
    (
        Nucleus_Concurrency_ConditionImpl *condition
    );

Nucleus_NonNull() Nucleus_Status
Nucleus_Concurrency_ConditionImpl_wait
    (
        Nucleus_Concurrency_ConditionImpl *condition,
        Nucleus_Concurrency_MutexImpl *mutex
    );

Nucleus_NonNull() Nucleus_Status
Nucleus_Concurrency_ConditionImpl_signalOne
    (
        Nucleus_Concurrency_ConditionImpl *condition
    );

Nucleus_NonNull() Nucleus_Status
Nucleus_Concurrency_ConditionImpl_signalAll
    (
        Nucleus_Concurrency_ConditionImpl *condition
    );

#endif
