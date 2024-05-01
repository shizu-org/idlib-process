# `idlib_mutex_uninitialize`

## C Signature
```
idlib_status
idlib_mutex_uninitialize
  (
    idlib_mutex* mutex
  );
```

## Description
Uninitialize an `idlib_mutex` object.

## Parameters
- `mutex` A pointer to the `idlib_mutex` object.

## Return value
`IDLIB_SUCCESS` on success. A non-zero IdLib status code on failure.

## Success
The `idlib_mutex` object pointed to by `mutex` was uninitialized.

## Remarks
The behaviour is undefined if `mutex` is not a null pointer and does not point to an initialized `idlib_mutex` object.
This function is thread-safe.
