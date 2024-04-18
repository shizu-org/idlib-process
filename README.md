# IdLib Process
*IdLib Process* provides cross-platform (Windows and Linux) intra-process functionality for embedding into C programs:
- singleton root object that is thread-safe, reference counted, and retains its singleton property also when using DLLs in Windows.
- singleton-scoped recursive intra-process mutex.
*IdLib Process* is licensed under the [zlib License](LICENSE).

## Integration into your CMake project
Integrating *IdLib Process* into the sources of your CMake-based project is the intended use of *IdLib Process*.

Follow these steps to integrate this library into your CMake-based project:

- Create a directory for external dependencies within your project directory. For example `3rdparty`.
- Create a folder `idlib-process` in that directory.
- Download/checkout *IdLib Process* into that folder such that the contents of the directory in which this `README.md` file is located in directly resides under `3rdparty/process`.
- Add to your project the `3rdparty/idlib-process/CMakeLists.txt` using `add_subdirectory`.
- Link your target `YourTarget` to *IdLib Process* using `target_link_libraries(YourTarget idlib-process)`.
- In your C/C++ code, you now can include now the functionality of *IdLib Process* by adding the preprocessor include directive `#include "idlib/process.h"`.

## Building
For development (or if you really want to use prebuilt binaries), you can do standalone builds of *IdLib Process*.

*IdLib Process* has been tested to build and run both under *Windows 11* and *Linux*.
Find the build instrutions for the respective systems below:
- [Building *IdLib Process*: Windows 11/Visual Studio Community](building-under-windows-11-visual-studio-community-20222)
- [Building *IdLib Process*: Linux](building-under-linux)

## Documentation
The documentation is provided as a set of MarkDown files directly in this repository.

Start reading the documentation here [documentation/idlib-process.md](documentation/idlib-process.md).
