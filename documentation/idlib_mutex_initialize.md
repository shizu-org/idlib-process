# `idlib_mutex_initialize`

## C Signature
```
idlib_status
idlib_mutex_initialize
  (
    idlib_mutex* mutex
  );
```

## Description
Initialize an `idlib_mutex` object.

## Parameters
- `mutex` A pointer to the `idlib_mutex` object.

## Return value
`IDLIB_SUCCESS` on success. A non-zero IdLib status code on failure.

## Success
The `idlib_mutex` object pointed to by `mutex` was initialized.

## Remarks
The behaviour is undefined if `mutex` is not a null pointer and does not point to an uninitialized `idlib_mutex` object.
This function is thread-safe.
