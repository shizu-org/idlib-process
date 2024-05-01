// Copyright (c) 2018 Michael Heilmann
#include "Nucleus/Concurrency/Pthreads/Thread.h"

#if (Nucleus_OperatingSystem == Nucleus_OperatingSystem_LINUX)  || \
    (Nucleus_OperatingSystem == Nucleus_OperatingSystem_CYGWIN) || \
    (Nucleus_OperatingSystem == Nucleus_OperatingSystem_MACOS)  || \
    defined(Nucleus_Threads_Pthreads)

#include "Nucleus/Memory.h"

static void *
callback
    (
        void *thread
    );

static void *
callback
    (
        void *p
    )
{
    Nucleus_Concurrency_ThreadImpl *impl = (Nucleus_Concurrency_ThreadImpl *)p;

    Nucleus_Concurrency_Mutex_lock(&impl->mutex);

    // Block execution as long as the state is neither started not terminated.
    while (impl->state != Nucleus_Concurrency_ThreadState_Started &&
           impl->state != Nucleus_Concurrency_ThreadState_Terminated)
    {
        Nucleus_Concurrency_Condition_wait(&impl->condition, &impl->mutex);
    }

    if (impl->state != Nucleus_Concurrency_ThreadState_Terminated)
    {
        Nucleus_Concurrency_Mutex_unlock(&impl->mutex);
        impl->status = impl->callbackFunction(impl->callbackContext);
        impl->state = Nucleus_Concurrency_ThreadState_Terminated;
    }
    else
    {
        Nucleus_Concurrency_Mutex_unlock(&impl->mutex);
    }


    return impl;
}

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
    //
    threadImpl->callbackContext = callbackContext;
    threadImpl->callbackFunction = callbackFunction;
    threadImpl->state = Nucleus_Concurrency_ThreadState_Initialized;
    threadImpl->status = Nucleus_Status_Success;
    //
    status = Nucleus_Concurrency_Mutex_initialize(&threadImpl->mutex);
    if (status)
    {
        Nucleus_deallocateMemory(threadImpl);
        return status;
    }
    //
    status = Nucleus_Concurrency_Condition_initialize(&threadImpl->condition);
    if (status)
    {
        Nucleus_Concurrency_Condition_uninitialize(&threadImpl->condition);
        Nucleus_deallocateMemory(threadImpl);
        return status;
    }
    //
    if (Nucleus_Unlikely(0 != pthread_create(&(threadImpl->thread), NULL, &callback, (void *)threadImpl)))
    {
        Nucleus_Concurrency_Condition_uninitialize(&threadImpl->condition);
        Nucleus_Concurrency_Mutex_uninitialize(&threadImpl->mutex);
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
    Nucleus_Concurrency_ThreadImpl *threadImpl = thread;
    Nucleus_Concurrency_Mutex_lock(&thread->mutex);
    if (threadImpl->state < Nucleus_Concurrency_ThreadState_Terminated)
    {
        threadImpl->state = Nucleus_Concurrency_ThreadState_Terminated;
        Nucleus_Concurrency_Condition_signalAll(&threadImpl->condition);
    }
    Nucleus_Concurrency_Mutex_unlock(&thread->mutex);
    // Join the thread.
    Nucleus_Concurrency_ThreadImpl_join(threadImpl);
    Nucleus_Concurrency_Condition_uninitialize(&threadImpl->condition);
    Nucleus_Concurrency_Mutex_uninitialize(&threadImpl->mutex);
    Nucleus_deallocateMemory(threadImpl);
}

Nucleus_NonNull() Nucleus_Status
Nucleus_Concurrency_ThreadImpl_join
    (
        Nucleus_Concurrency_ThreadImpl *thread
    )
{
    void *result;
    Nucleus_Concurrency_Mutex_lock(&thread->mutex);
    if (thread->state == Nucleus_Concurrency_ThreadState_Initialized)
    {
        Nucleus_Concurrency_Mutex_unlock(&thread->mutex);
        return Nucleus_Status_NotStarted;
    }
    if (thread->state == Nucleus_Concurrency_ThreadState_Terminated)
    {
        Nucleus_Concurrency_Mutex_unlock(&thread->mutex);
        return Nucleus_Status_AlreadyStopped;
    }
    Nucleus_Concurrency_Mutex_unlock(&thread->mutex);
    pthread_join(thread->thread, &result);
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
    Nucleus_Concurrency_Mutex_lock(&thread->mutex);
    if (thread->state == Nucleus_Concurrency_ThreadState_Started)
    {
        Nucleus_Concurrency_Mutex_unlock(&thread->mutex);
        return Nucleus_Status_AlreadyStarted;
    }
    if (thread->state == Nucleus_Concurrency_ThreadState_Terminated)
    {
        Nucleus_Concurrency_Mutex_unlock(&thread->mutex);
        return Nucleus_Status_AlreadyStopped;
    }
    thread->state = Nucleus_Concurrency_ThreadState_Started;
    Nucleus_Concurrency_Condition_signalAll(&thread->condition);
    Nucleus_Concurrency_Mutex_unlock(&thread->mutex);
    return Nucleus_Status_Success;
}

#endif
