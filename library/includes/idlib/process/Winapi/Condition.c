// Copyright (c) 2018 Michael Heilmann
#include "Nucleus/Concurrency/Winapi/Condition.h"

#if (Nucleus_OperatingSystem == Nucleus_OperatingSystem_WINDOWS) && \
    !defined(Nucleus_Threads_Pthreads)

#include "Nucleus/Concurrency/Winapi/Mutex.h"
#include "Nucleus/Memory.h"
#include <stdio.h>

Nucleus_NonNull() Nucleus_Status
Nucleus_Concurrency_ConditionImpl_create
    (
        Nucleus_Concurrency_ConditionImpl **condition
    )
{
    if (Nucleus_Unlikely(!condition))
    { return Nucleus_Status_InvalidArgument; }
    //
    Nucleus_Status status;
    //
    Nucleus_Concurrency_ConditionImpl *conditionImpl;
    status = Nucleus_allocateMemory((void **)&conditionImpl, sizeof(Nucleus_Concurrency_ConditionImpl));
    if (Nucleus_Unlikely(status))
    { return status; }
    //
    InitializeConditionVariable(&(conditionImpl->conditionVariable));
    //
    *condition = conditionImpl;
    //
    return Nucleus_Status_Success;
}

Nucleus_NonNull() void
Nucleus_Concurrency_ConditionImpl_destroy
    (
        Nucleus_Concurrency_ConditionImpl *condition
    )
{
    Nucleus_Concurrency_ConditionImpl *conditionImpl = condition;
    Nucleus_deallocateMemory(conditionImpl);
}

Nucleus_NonNull() Nucleus_Status
Nucleus_Concurrency_ConditionImpl_wait
    (
        Nucleus_Concurrency_ConditionImpl *condition,
        Nucleus_Concurrency_MutexImpl *mutex
    )
{
    if (Nucleus_Unlikely(!condition || !mutex)) return Nucleus_Status_InvalidArgument;
    EnterCriticalSection(&condition->criticalSection);
    SleepConditionVariableCS(&condition->conditionVariable, &condition->criticalSection, INFINITE);
    LeaveCriticalSection(&condition->criticalSection);
    return Nucleus_Status_Success;
}

Nucleus_NonNull() Nucleus_Status
Nucleus_Concurrency_ConditionImpl_signalOne
    (
        Nucleus_Concurrency_ConditionImpl *condition
    )
{
    if (Nucleus_Unlikely(!condition)) return Nucleus_Status_InvalidArgument;
    WakeConditionVariable(&condition->conditionVariable);
    return Nucleus_Status_Success;
}

Nucleus_NonNull() Nucleus_Status
Nucleus_Concurrency_ConditionImpl_signalAll
    (
        Nucleus_Concurrency_ConditionImpl *condition
    )
{
    if (Nucleus_Unlikely(!condition)) return Nucleus_Status_InvalidArgument;
    WakeAllConditionVariable(&condition->conditionVariable);
    return Nucleus_Status_Success;
}

#endif
