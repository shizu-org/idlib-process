# IdLib Processs
Welcome to *IdLib Process*.
*IdLib Process* provides cross-platform (Windows and Linux) intra-process functionality:
- singleton root object that is thread-safe, reference counted, and retains its singleton property also when using DLLs in Windows.
- singleton-scoped recursive intra-process mutex.
The library is designed as *backing* library to be integrated into other C programs.

## Documentation
- [idlib_status.md](idlib_status.md)
- [idlib_process.md](idlib_process.md)
- [idlib_process_acquire.md](idlib_process_acquire.md)
- [idlib_process_relinquish.md](idlib_process_relinquish.md)
- [idlib_mutex.md](idlib_mutex.md)
- [idlib_mutex_initialize.md](idlib_mutex_initialite.md)
- [idlib_mutex_uninitialize.md](idlib_mutex_uninitialize.md)
