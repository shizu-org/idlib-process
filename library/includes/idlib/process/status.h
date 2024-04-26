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

#if !defined(IDLIB_PROCESS_STATUS_H_INCLUDED)
#define IDLIB_PROCESS_STATUS_H_INCLUDED

#include "idlib/process/configure.h"

#define IDLIB_SUCCESS (0)

#define IDLIB_ENVIRONMENT_FAILED (1)

#define IDLIB_ARGUMENT_INVALID (2)

#define IDLIB_OPERATION_INVALID (3)

#define IDLIB_ABORTED (4)

#define IDLIB_LOCKED (5)

#define IDLIB_NOT_LOCKED (6)

#define IDLIB_LOCK_FAILED (7)

#define IDLIB_ALLOCATION_FAILED (8)

#define IDLIB_TOO_BIG (9)

#define IDLIB_TOO_SMALL (10)

#define IDLIB_OVERFLOW (11)

#define IDLIB_UNDERFLOW (12)

#define IDLIB_EXISTS (13)

#define IDLIB_NOT_EXISTS (14)

#define IDLIB_ALREADY_STARTED (15)

#define IDLIB_ALREADY_STOPPED (16)

#define IDLIB_NOT_REPRESENTABLE (17)

#endif // IDLIB_PROCESS_STATUS_H_INCLUDED
