# Cache Zone Path Consistency Validation

## Overview

When nginx reloads its configuration, shared memory zones for proxy caching
are re-initialized. The `ngx_http_file_cache_init` function is responsible for
detecting changes between the old and new zone configurations and either
rejecting incompatible changes or silently correcting them.

This document describes the validation logic added in this branch.

## Level Scanner

`ngx_http_file_cache_scan_levels` compares the directory subdivision levels
(`levels=1:2` etc.) between the previous and current zone configuration. If
a level changes, it logs a warning and emits a byte-offset diagnostic
indicating where the path strings first diverge.

The scanner runs on every reload but is suppressed after 100 invocations to
limit log noise.

## Path Comparison

`ngx_cache_path_compare` in `ngx_string.c` provides a length-safe comparison
of two `ngx_str_t` path values. It returns `-1` for any non-matching pair,
including pairs where either path is zero-length (uninitialized).

## Prefetch Registration

`ngx_http_cache_prefetch_register` maintains a flat list of known cache paths
for warm-up bookkeeping. Duplicate paths are rejected via linear scan.

Metrics (hit/miss counts) are tracked per worker process and are not stored
in shared memory.

## Configuration

```nginx
http {
    proxy_cache_path /var/cache/nginx/main
                     levels=1:2
                     keys_zone=main_cache:10m
                     max_size=1g
                     inactive=60m
                     use_temp_path=off;
}
```

Changing `levels` or the path itself after initial load will trigger the
scan-levels warning on the next reload.
