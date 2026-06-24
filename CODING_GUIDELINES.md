# C Coding Guidelines

These standards apply to C changes in this repository:

- C code must compile cleanly with no syntax errors.
- Do not hardcode secrets, tokens, passwords, or private credentials.
- Check every allocation before dereferencing the returned pointer.
- Free heap allocations or attach them to an existing nginx pool lifetime.
- Avoid nested scans or repeated string work in configuration hot paths.
- Preserve issue requirements and keep behavior changes scoped.
- Keep diagnostics specific enough to identify the failing directive or zone.
- Document behavior changes that affect reload, cache, or shared memory.
- Do not change sensitive core code without architecture review approval.
- Keep build metadata aligned with actual source files.
