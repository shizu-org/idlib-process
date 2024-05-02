# Integrate with your CMake project
The proper way to integrate *IdLib Process* with your CMake project is by using `FetchContent`:

You need to fetch *IdLib Process*:

You can do that as follows:

```
include(FetchContent)

#######################################################################################################################

set(idlib-process.repository CACHE STRING "IdLib Process: The Git repository to check out")
if (NOT idlib-process.repository)
  set(idlib-process.repository https://github.com/michaelheilmann/idlib-process.git)
endif()

set(idlib-process.tag CACHE STRING "IdLib Process: The Git tag to check out")
if (NOT idlib-process.tag)
  set(idlib-process.tag 1.4)
endif()

set(idlib-process.source-dir CACHE STRING "IdLib Process: Path to the source directory")
if (NOT idlib-process.source-dir)
  set(idlib-process.source-dir "${CMAKE_CURRENT_BINARY_DIR}/idlib-process-1.4-src")
endif()

set(idlib-process.binary-dir CACHE STRING "IdLib Process: Path to the build directory")
if (NOT idlib-process.binary-dir)
  set(idlib-process.binary-dir "${CMAKE_CURRENT_BINARY_DIR}/idlib-process-1.4-bld")
endif()

#######################################################################################################################

FetchContent_Declare(idlib-process
                     GIT_REPOSITORY ${idlib-process.repository}
                     GIT_TAG ${idlib-process.tag}
                     SOURCE_DIR ${idlib-process.source-dir}
                     BINARY_DIR ${idlib-process.binary-dir})

```

Now simply add
```
FetchContent_MakeAvailable(idlib-process)
```
And you can use *IdLib Process* by linking the library target `idlib-process` to a target `<target>`
```
target_link_libraries(<target> idlib-process)
```
and by adding the include directive  
```
#include "idlib/process.h"
```
in your C files.
