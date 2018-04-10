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

#include <iostream>

/** Type definition. */
struct sdsdkv {
    // Dup of initializing communicator.
    MPI_Comm m_comm;
    //
    static int
    create(
        sdsdkv_context *c,
        MPI_Comm c_comm
    ) {
        if (!c) return SDSDKV_ERR_INVLD_ARG;

        return SDSDKV_SUCCESS;
    }
    //
    static int
    destroy(
        sdsdkv_context c
    ) {
        if (!c) return SDSDKV_ERR_INVLD_ARG;

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
    MPI_Comm c_comm
) {
    return sdsdkv::create(c, c_comm);
}

////////////////////////////////////////////////////////////////////////////////
int
sdsdkv_destroy(
    sdsdkv_context c
) {
    return sdsdkv::destroy(c);
}
