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
    sdsdkv_iconfig *m_config;
    //
public:
    //
    sdsdkv_impl(void)
        : m_personality(nullptr)
        , m_mpi(nullptr)
        , m_config(nullptr) { }
    //
    ~sdsdkv_impl(void)
    {
        delete m_personality;
        delete m_mpi;
        delete m_config;
    }
    //
    int
    init(
        const sdsdkv_config &config
    ) {
        // Cache user-provided configuration.
        m_config = new sdsdkv_iconfig();
        int rc = m_config->init(config);
        if (rc != SDSDKV_SUCCESS) {
            return rc;
        }
        // Create and initialize MPI infrastructure.
        m_mpi = new sdsdkv_mpi();
        rc = m_mpi->init(*m_config);
        if (rc != SDSDKV_SUCCESS) {
            return rc;
        }
        // Create and initialize personality instance based on configury.
        rc = personality_factory::create(
                 m_config->personality,
                 &m_personality
             );
        if (rc != SDSDKV_SUCCESS) {
            return rc;
        }
        rc = m_personality->init(m_mpi, config);
        if (rc != SDSDKV_SUCCESS) {
            return rc;
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
    //
    int
    close(void)
    {
        return m_personality->close();
    }
    //
    int
    put(
        const void *key,
        uint64_t key_size,
        const void *value,
        uint64_t value_size
    ) {
        return m_personality->put(key, key_size, value, value_size);
    }
    //
    int
    get(
        const void *key,
        uint64_t key_size,
        void *value,
        uint64_t *value_size
    ) {
        return m_personality->get(key, key_size, value, value_size);
    }
};

/*
 * vim: ft=cpp ts=4 sts=4 sw=4 expandtab
 */
