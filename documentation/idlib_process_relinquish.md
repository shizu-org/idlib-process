# `idlib_process_relinquish`

## C Signature
```
idlib_status
idlib_process_relinquish
  (
    idlib_process* process
  );
```

## Description
Relinquish the `idlib_process` singleton object.

## Parameters
- `process` A pointer to the `idlib_process` singleton object previously acquired by a successful call to `idlib_process_acquire`.

## Return value
`IDLIB_SUCCESS` on success. A non-zero IdLib status code on failure.

## Success
The caller relinquished his reference to the `idlib_process` singleton object.

## Remarks
This function is thread-safe.
