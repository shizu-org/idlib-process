// Copyright (c) 2018 Michael Heilmann
#include "Nucleus/Concurrency/Winapi/Thread-private.c.in"

#if (Nucleus_OperatingSystem == Nucleus_OperatingSystem_WINDOWS) && \
    !defined(Nucleus_Threads_Pthreads)

#include "Nucleus/Memory.h"

Nucleus_NonNull() Nucleus_Status
Nucleus_Concurrency_ThreadImpl_create
    (
        Nucleus_Concurrency_ThreadImpl **thread,
        Nucleus_Concurrency_Thread_CallbackContext *callbackContext,
        Nucleus_Concurrency_Thread_CallbackFunction *callbackFunction
    )
{
    if (Nucleus_Unlikely(!thread || !callbackFunction)) return Nucleus_Status_InvalidArgument;
    //
    Nucleus_Status status;
    //
    Nucleus_Concurrency_ThreadImpl *threadImpl;
    status = Nucleus_allocateMemory((void **)&(threadImpl), sizeof (Nucleus_Concurrency_ThreadImpl));
    if (status)
    { return status; }
    // Initialize the fields.
    threadImpl->callbackContext = callbackContext;
    threadImpl->callbackFunction = callbackFunction;
    threadImpl->status = Nucleus_Status_Success;
    threadImpl->state = Nucleus_Concurrency_ThreadState_Initialized;
    // Initialize the mutex.
    status = Nucleus_Concurrency_Mutex_initialize(&threadImpl->mutex);
    if (status)
    {
        Nucleus_deallocateMemory(threadImpl);
        return status;
    }
    // Create the thread suspended.
    threadImpl->thread = CreateThread(NULL, 0, &callback, threadImpl, CREATE_SUSPENDED, NULL);
    if (Nucleus_Unlikely(!threadImpl->thread))
    {
        Nucleus_deallocateMemory(threadImpl);
        return Nucleus_Status_EnvironmentFailed;
    }
    //
    *thread = threadImpl;
    //
    return Nucleus_Status_Success;
}

Nucleus_NonNull() void
Nucleus_Concurrency_ThreadImpl_destroy
    (
        Nucleus_Concurrency_ThreadImpl *thread
    )
{
    // Join the thread.
    Nucleus_Concurrency_ThreadImpl_join(thread);
    Nucleus_deallocateMemory(thread);
}

Nucleus_NonNull() Nucleus_Status
Nucleus_Concurrency_ThreadImpl_join
    (
        Nucleus_Concurrency_ThreadImpl *thread
    )
{
    lock(thread);
    switch (thread->state)
    {
        case Nucleus_Concurrency_ThreadState_Initialized:
        {
            unlock(thread);
            return Nucleus_Status_NotStarted;
        }
        case Nucleus_Concurrency_ThreadState_Terminated:
        {
            unlock(thread);
            return Nucleus_Status_AlreadyStopped;
        }
    };
    unlock(thread);
    // We enter the waiting state here.
    // The thread is either running or has terminated.
    WaitForSingleObject(thread->thread, INFINITE);
    return Nucleus_Status_Success;
}

Nucleus_NonNull() Nucleus_Status
Nucleus_Concurrency_ThreadImpl_getStatus
    (
        Nucleus_Concurrency_ThreadImpl *thread,
        Nucleus_Status *status
    )
{
    if (Nucleus_Unlikely(!thread || !status)) return Nucleus_Status_InvalidArgument;
    *status = thread->status;
    return Nucleus_Status_Success;
}

Nucleus_NonNull() Nucleus_Status
Nucleus_Concurrency_ThreadImpl_start
    (
        Nucleus_Concurrency_ThreadImpl *thread
    )
{
    if (Nucleus_Unlikely(!thread)) return Nucleus_Status_InvalidArgument;
    lock(thread);
    switch (thread->state)
    {
        case Nucleus_Concurrency_ThreadState_Started:
            {
                unlock(thread);
                return Nucleus_Status_AlreadyStarted;
            }
        case Nucleus_Concurrency_ThreadState_Terminated:
            {
                unlock(thread);
                return Nucleus_Status_AlreadyStopped;
            }
    };
    DWORD dwResult = ResumeThread(thread->thread);
    if ((DWORD)-1 == dwResult)
    {
        unlock(thread);
        return Nucleus_Status_EnvironmentFailed;
    }
    unlock(thread);
    return Nucleus_Status_Success;
}

#endif
