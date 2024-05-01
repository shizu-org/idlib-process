/*
  IdLib Process
  Copyright (C) 2023-2024 Michael Heilmann. All rights reserved.

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

#include "idlib/process.h"

#include <stdlib.h>

static int
test1
  (
  )
{
  idlib_status status;
  idlib_mutex mutex;
  status = idlib_mutex_initialize(NULL);
  if (IDLIB_ARGUMENT_INVALID != status) {
    return IDLIB_ENVIRONMENT_FAILED;
  }
  status = idlib_mutex_initialize(&mutex);
  if (status) {
    return status;
  }
  status = idlib_mutex_lock(&mutex);
  if (status) {
    idlib_mutex_uninitialize(&mutex);
    return status;
  }
  status = idlib_mutex_unlock(&mutex);
  if (status) {
    idlib_mutex_uninitialize(&mutex);
    return status;
  }
  status = idlib_mutex_uninitialize(&mutex);
  if (status) {
    return status;
  }
  return IDLIB_SUCCESS;
}

int
main
  (
    int argc,
    char** argv
  )
{
  if (test1()) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

