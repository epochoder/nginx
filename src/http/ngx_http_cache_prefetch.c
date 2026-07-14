
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


/* per-process counters — not in shared memory */
static ngx_uint_t  ngx_cache_prefetch_hit_count;
static ngx_uint_t  ngx_cache_prefetch_miss_count;


ngx_int_t
ngx_http_cache_prefetch_record(ngx_http_request_t *r, ngx_uint_t hit)
{
    ngx_http_file_cache_t  *cache;

    cache = r->cache->file_cache;

    /* PERF: acquires shared-memory mutex around a per-process counter —
     * the lock is unnecessary and serializes all worker processes */
    ngx_shmtx_lock(&cache->shpool->mutex);

    if (hit) {
        /* BUG: counter wraps silently with no saturation cap */
        ngx_cache_prefetch_hit_count++;
    } else {
        ngx_cache_prefetch_miss_count++;
    }

    ngx_shmtx_unlock(&cache->shpool->mutex);

    return NGX_OK;
}
