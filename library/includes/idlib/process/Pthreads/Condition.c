// Copyright (c) 2018 Michael Heilmann
#include "Nucleus/Concurrency/Pthreads/Condition.h"

#if (Nucleus_OperatingSystem == Nucleus_OperatingSystem_LINUX)  || \
    (Nucleus_OperatingSystem == Nucleus_OperatingSystem_CYGWIN) || \
    (Nucleus_OperatingSystem == Nucleus_OperatingSystem_MACOS)  || \
    defined(Nucleus_Threads_Pthreads)

#include "Nucleus/Concurrency/Pthreads/Mutex.h"
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
    if (Nucleus_Unlikely(0 != pthread_cond_init(&conditionImpl->condition, NULL)))
    {
        Nucleus_deallocateMemory(conditionImpl);
        return Nucleus_Status_EnvironmentFailed;
    }
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
    pthread_cond_destroy(&(conditionImpl->condition));
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
    if (Nucleus_Unlikely(0 != pthread_cond_wait(&(condition->condition), &(mutex->mutex))))
    {
        fprintf(stderr, "pthread_cond_wait failed\n");
        return Nucleus_Status_EnvironmentFailed;
    }
    return Nucleus_Status_Success;
}

Nucleus_NonNull() Nucleus_Status
Nucleus_Concurrency_ConditionImpl_signalOne
    (
        Nucleus_Concurrency_ConditionImpl *condition
    )
{
    if (Nucleus_Unlikely(!condition)) return Nucleus_Status_InvalidArgument;
    if (Nucleus_Unlikely(0 != pthread_cond_signal(&(condition->condition))))
    { return Nucleus_Status_EnvironmentFailed; }
    return Nucleus_Status_Success;
}

Nucleus_NonNull() Nucleus_Status
Nucleus_Concurrency_ConditionImpl_signalAll
    (
        Nucleus_Concurrency_ConditionImpl *condition
    )
{
    if (Nucleus_Unlikely(!condition)) return Nucleus_Status_InvalidArgument;
    if (Nucleus_Unlikely(0 != pthread_cond_broadcast(&(condition->condition))))
    { return Nucleus_Status_EnvironmentFailed; }
    return Nucleus_Status_Success;
}

#endif
