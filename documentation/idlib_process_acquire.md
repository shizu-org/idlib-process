# `idlib_process_acquire`

## C Signature
```
idlib_status
idlib_process_acquire
  (
    idlib_process** process
  );
```

## Description
Acquire the `idlib_process` singleton object.

## Parameters
- `process` A pointer to a `idlib_process*` variable.

## Return value
`IDLIB_SUCCESS` on success. A non-zero IdLib status code on failure.

## Success
`*process` was assigned a pointer to the `idlib_process` singleton object.
The caller acquired a reference to that object.

## Remarks
This function is thread-safe.
