/*
 * Copyright (c) 2018      Los Alamos National Security, LLC
 *                         All rights reserved.
 *
 * This file is part of the sdsdkv project. See the LICENSE file at the
 * top-level directory of this distribution.
 */

/**
 * @file sdsdkv.cc
 */

#include "sdsdkv.h"
#include "sdsdkv-impl.h"

/** Type definition. */
class sdsdkv {
public:
    //
    sdsdkv(void) = delete;
    //
    ~sdsdkv(void) = delete;
    //
    static int
    create(
        sdsdkv_context *c,
        sdsdkv_config *config
    ) {
        if (!c || !config) return SDSDKV_ERR_INVLD_ARG;
        //
        *c = (sdsdkv_context)NULL;
        //
        sdsdkv_impl *impl = new sdsdkv_impl();
        int rc = impl->init(*config);
        if (rc != SDSDKV_SUCCESS) {
            delete impl;
            return rc;
        }
        // It's all good...
        *c = (sdsdkv_context)impl;
        return SDSDKV_SUCCESS;
    }
    //
    static int
    open(
        sdsdkv_context c
    ) {
        sdsdkv_impl *impl = (sdsdkv_impl *)c;
        return impl->open();
    }
    //
    static int
    put(
        sdsdkv_context c,
        const void *key,
        uint64_t key_size,
        const void *value,
        uint64_t value_size
    ) {
        sdsdkv_impl *impl = (sdsdkv_impl *)c;
        return impl->put(key, key_size, value, value_size);
    }
    //
    static int
    destroy(
        sdsdkv_context *c
    ) {
        if (c) {
            if (*c) {
                sdsdkv_impl *impl = (sdsdkv_impl *)*c;
                delete impl;
            }
            *c = NULL;
        }
        return SDSDKV_SUCCESS;
    }
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Exported API
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

int
sdsdkv_create(
    sdsdkv_context *c,
    sdsdkv_config *config
) {
    return sdsdkv::create(c, config);
}
//
int
sdsdkv_open(
    sdsdkv_context c
) {
    return sdsdkv::open(c);
}
//
int
sdsdkv_put(
    sdsdkv_context c,
    const void *key,
    uint64_t key_size,
    const void *value,
    uint64_t value_size
) {
    return sdsdkv::put(c, key, key_size, value, value_size);
}
//
int
sdsdkv_destroy(
    sdsdkv_context c
) {
    return sdsdkv::destroy(&c);
}

/*
 * vim: ft=cpp ts=4 sts=4 sw=4 expandtab
 */
