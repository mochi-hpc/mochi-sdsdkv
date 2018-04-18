/*
 * Copyright (c) 2018      Los Alamos National Security, LLC
 *                         All rights reserved.
 *
 * This file is part of the sdsdkv project. See the LICENSE file at the
 * top-level directory of this distribution.
 */

/**
 * @file sdsdkv-impl.h
 */

#pragma once

#include "sdsdkv-personality-factory.h"
#include "sdsdkv-mpi.h"
#include "sdsdkv-config.h"

#include <unistd.h>
#include <sys/types.h>

#include <iostream>
#include <stdexcept>

/** sdsdkv Implementation. */
class sdsdkv_impl {
    //
    personality *m_personality;
    //
    sdsdkv_mpi *m_mpi;
    //
    sdsdkv_config *m_config;
    //
public:
    //
    sdsdkv_impl(
        void
    ) : m_personality(nullptr)
      , m_mpi(nullptr)
      , m_config(nullptr) { }
    //
    ~sdsdkv_impl(void)
    {
        delete m_personality;
        delete m_mpi;
        config_dup_destroy(&m_config);
    }
    //
    int
    init(
        const sdsdkv_config &config
    ) {
        // Check user-provided configuration.
        if (!config_valid(config)) {
            return SDSDKV_ERR_INVLD_CONFIG;
        }
        // Cache user-provided configuration.
        int rc = config_dup(config, &m_config);
        if (rc != SDSDKV_SUCCESS) {
            return rc;
        }
        // Initialize MPI infrastructure.
        m_mpi = new sdsdkv_mpi();
        rc = m_mpi->init(config.init_comm);
        if (rc != SDSDKV_SUCCESS) {
            return rc;
        }
        //
        m_personality = personality_factory::create(
                            config.personality
                        );
        if (!m_personality) {
            return SDSDKV_ERR_INVLD_CONFIG;
        }
        //
        return SDSDKV_SUCCESS;
    }
    //
    int
    open(void)
    {
        return m_personality->open();
    }
};

/*
 * vim: ft=cpp ts=4 sts=4 sw=4 expandtab
 */
