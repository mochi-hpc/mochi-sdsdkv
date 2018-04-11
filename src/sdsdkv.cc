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
#include "sdsdkv-config.h"
#include "sdsdkv-mpi.h"

#include <iostream>

/** Type definition. */
struct sdsdkv {
    //
    sdsdkv_mpi *mpi;
    //
    static int
    create(
        sdsdkv_context *c,
        sdsdkv_config *config
    ) {
        if (!c) return SDSDKV_ERR_INVLD_ARG;

        if (!config_valid(*config)) {
            return SDSDKV_ERR_INVLD_CONFIG;
        }

        sdsdkv *tc = (sdsdkv *)calloc(1, sizeof(*tc));
        if (!tc) return SDSDKV_ERR_OOR;

        int erc = sdsdkv_mpi::create(&(tc->mpi), config->init_comm);
        if (erc != SDSDKV_SUCCESS) {
            destroy(tc);
            return erc;
        }

        *c = tc;

        return SDSDKV_SUCCESS;
    }
    //
    static int
    destroy(
        sdsdkv_context c
    ) {
        if (c) {
            sdsdkv_mpi::destroy(c->mpi);
            free(c);
        }

        return SDSDKV_SUCCESS;
    }
    //
    sdsdkv(void) = delete;
    //
    ~sdsdkv(void) = delete;
};

////////////////////////////////////////////////////////////////////////////////
int
sdsdkv_create(
    sdsdkv_context *c,
    sdsdkv_config *config
) {
    return sdsdkv::create(c, config);
}

////////////////////////////////////////////////////////////////////////////////
int
sdsdkv_destroy(
    sdsdkv_context c
) {
    return sdsdkv::destroy(c);
}

/*
 * vim: ft=cpp ts=4 sts=4 sw=4 expandtab
 */
