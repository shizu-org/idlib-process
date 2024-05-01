// Copyright (c) 2018 Michael Heilmann
#pragma once

#include "Nucleus/Annotations.h"
#include "Nucleus/Status.h"

// Forward declaration.
typedef struct Nucleus_Concurrency_ConditionImpl Nucleus_Concurrency_ConditionImpl;
typedef struct Nucleus_Concurrency_Mutex Nucleus_Concurrency_Mutex;

typedef struct Nucleus_Concurrency_Condition Nucleus_Concurrency_Condition;
struct Nucleus_Concurrency_Condition
{
    Nucleus_Concurrency_ConditionImpl *impl;
}; // struct Nucleus_Concurrency_Condition

Nucleus_NonNull() Nucleus_Status
Nucleus_Concurrency_Condition_initialize
    (
        Nucleus_Concurrency_Condition *condition
    );

Nucleus_NonNull() void
Nucleus_Concurrency_Condition_uninitialize
    (
        Nucleus_Concurrency_Condition *condition
    );

Nucleus_NonNull() Nucleus_Status
Nucleus_Concurrency_Condition_wait
    (
        Nucleus_Concurrency_Condition *condition,
        Nucleus_Concurrency_Mutex *mutex
    );

Nucleus_NonNull() Nucleus_Status
Nucleus_Concurrency_Condition_signalOne
    (
        Nucleus_Concurrency_Condition *condition
    );

Nucleus_NonNull() Nucleus_Status
Nucleus_Concurrency_Condition_signalAll
    (
        Nucleus_Concurrency_Condition *condition
    );
