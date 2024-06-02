# IdLib Process
*IdLib Process* provides cross-platform (Windows and Linux) intra-process functionality:
- singleton root object that is thread-safe, reference counted, and retains its singleton property also when using DLLs in Windows.
- singleton-scoped recursive intra-process mutex.
The library is designed as *backing* library to be integrated into other C programs.
*IdLib Process* is licensed under the [zlib License](LICENSE).

## Integrating into CMake projects
Integrating *IdLib Process* into the sources of your CMake-based project is the intended use of *IdLib Process*.
For information on the integration of *IdLib Process* with your CMake-based project, see
[Integrating *Process* in CMake-based projects](integrating-into-cmake-projects.md).

## Development
For development (or if you really want to use prebuilt binaries), you can do standalone builds of *IdLib Process*.

*IdLib Process* has been tested to build and run both under *Windows 11* and *Linux*.
Find the build instrutions for the respective systems below:
- [Building *IdLib Process* under Windows 11/Visual Studio Community](building-under-windows-11-visual-studio-community-20222)
- [Building *IdLib Process* under Linux](building-under-linux)

## Documentation
The documentation is provided as a set of MarkDown files directly in this repository.

Start reading the documentation here [documentation/idlib-process.md](documentation/idlib-process.md).
