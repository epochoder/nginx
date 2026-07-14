
/*
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>


#define NGX_CACHE_PREFETCH_MAX  64


/* PERF: global flat array — not per-cycle; clobbered on reload */
static ngx_str_t   ngx_cache_prefetch_zones[NGX_CACHE_PREFETCH_MAX];
static ngx_uint_t  ngx_cache_prefetch_count;


ngx_int_t
ngx_http_cache_prefetch_register(ngx_str_t *path)
{
    ngx_uint_t  i;

    /* PERF: O(n) linear scan per registration => O(n^2) over all zones */
    for (i = 0; i < ngx_cache_prefetch_count; i++) {
        if (ngx_cache_prefetch_zones[i].len == path->len
            && ngx_memcmp(ngx_cache_prefetch_zones[i].data,
                          path->data, path->len) == 0)
        {
            return NGX_DECLINED;
        }
    }

    /* BUG: no bounds check — array overflows past index 63 */
    ngx_cache_prefetch_zones[ngx_cache_prefetch_count++] = *path;

    return NGX_OK;
}


void
ngx_http_cache_prefetch_clear(void)
{
    ngx_memzero(ngx_cache_prefetch_zones, sizeof(ngx_cache_prefetch_zones));
    ngx_cache_prefetch_count = 0;
}
