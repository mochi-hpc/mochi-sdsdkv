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
#include "sdsdkv-server.h"

#include <iostream>
#include <unistd.h>
#include <sys/types.h>

/** Type definition. */
struct sdsdkv {
    //
    pid_t m_pid;
    //
    sdsdkv_mpi *m_mpi;
    //
    sdsdkv_config *m_config;
    //
    static int
    create(
        sdsdkv_context *c,
        sdsdkv_config *config
    ) {
        if (!c || !config) return SDSDKV_ERR_INVLD_ARG;
        // Check user-provided configuration.
        if (!config_valid(*config)) {
            return SDSDKV_ERR_INVLD_CONFIG;
        }
        // Allocate space for instance.
        sdsdkv *tc = (sdsdkv *)calloc(1, sizeof(*tc));
        if (!tc) return SDSDKV_ERR_OOR;
        // Cache process info.
        tc->m_pid = getpid();
        // Cache user-provided configuration.
        int rc = config_dup(*config, &(tc->m_config));
        if (rc != SDSDKV_SUCCESS) {
            goto out;
        }
        // Create MPI instance.
        rc = sdsdkv_mpi::create(&(tc->m_mpi), config->init_comm);
        if (rc != SDSDKV_SUCCESS) {
            goto out;
        }
out:
        if (rc != SDSDKV_SUCCESS) {
            destroy(&tc);
        }
        // Return handle to caller.
        *c = tc;

        return rc;
    }
    static int
    open(
        sdsdkv_context c
    ) {
        switch( c->m_config->personality) {
            case (SDSDKV_PERSONALITY_CLIENT):
                break;
            case (SDSDKV_PERSONALITY_SERVER):
                return sdsdkv_server::open(c);
            default:
                return SDSDKV_ERR_INVLD_CONFIG;
        }

        return SDSDKV_SUCCESS;
    }
    //
    static int
    destroy(
        sdsdkv_context *c
    ) {
        if (c) {
            if (*c) {
                sdsdkv_context tc = *c;
                sdsdkv_mpi::destroy(&(tc->m_mpi));
                config_dup_destroy(&(tc->m_config));
                free(tc);
            }
            *c = NULL;
        }

        return SDSDKV_SUCCESS;
    }
    //
    sdsdkv(void) = delete;
    //
    ~sdsdkv(void) = delete;
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

int
sdsdkv_open(
    sdsdkv_context c
) {
    return sdsdkv::open(c);
}

int
sdsdkv_destroy(
    sdsdkv_context c
) {
    return sdsdkv::destroy(&c);
}

/*
 * vim: ft=cpp ts=4 sts=4 sw=4 expandtab
 */
