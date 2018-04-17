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
#include "sdsdkv-personality.h"
#include "sdsdkv-client.h"
#include "sdsdkv-server.h"

#include <unistd.h>
#include <sys/types.h>

#include <iostream>
#include <stdexcept>

/** Type definition. */
struct sdsdkv {
    //
    pid_t m_pid;
    //
    sdsdkv_mpi *m_mpi;
    //
    sdsdkv_config *m_config;
    //
    sdsdkv_personality *m_personality;
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
    //
    static int
    open(
        sdsdkv_context c
    ) {
        try {
            // TODO(skg) Sync needed between clients and servers?
            switch(c->m_config->personality) {
                case (SDSDKV_PERSONALITY_CLIENT):
                    c->m_personality = new sdsdkv_client(*(c->m_config));
                    break;
                case (SDSDKV_PERSONALITY_SERVER): {
                    c->m_personality = new sdsdkv_server(*(c->m_config));
                    break;
                }
                default:
                    return SDSDKV_ERR_INVLD_CONFIG;
            }
        }
        catch (const std::runtime_error &e) {
            // TODO(skg) Do better.
            std::cerr << e.what() << std::endl;
            return SDSDKV_ERR;
        }
        //
        return c->m_personality->open();
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
                delete tc->m_personality;
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
