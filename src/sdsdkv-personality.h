/*
 * Copyright (c) 2018      Los Alamos National Security, LLC
 *                         All rights reserved.
 *
 * This file is part of the sdsdkv project. See the LICENSE file at the
 * top-level directory of this distribution.
 */

/**
 * @file sdsdkv-personality.h
 */

#pragma once

#include "sdsdkv-config.h"

#include "sdsdkv-mpi.h"

#include "margo.h"
#include "ssg.h"

class personality {
protected:
    /** Points to already initialized sdsdkv_mpi instance. */
    sdsdkv_mpi *m_mpi;
    /** Copy of internal representation of user-provided configuration. */
    sdsdkv_iconfig *m_config;
    /** Margo ID. */
    margo_instance_id m_mid;
    /** SSG group ID. */
    ssg_group_id_t m_gid;
public:
    //
    personality(void)
        : m_mpi(nullptr)
        , m_config(nullptr)
        , m_mid(MARGO_INSTANCE_NULL)
        , m_gid(SSG_GROUP_ID_NULL) { }
    //
    virtual
    ~personality(void)
    {
        delete m_config;
    }
    //
    virtual int
    init(
        sdsdkv_mpi *mpi,
        const sdsdkv_config &config
    ) {
        m_mpi = mpi;
        //
        m_config = new sdsdkv_iconfig();
        return m_config->init(config);
    }
    //
    virtual int
    xchange_gid(void) = 0;
    //
    virtual int
    open(void) = 0;
};

/*
 * vim: ft=cpp ts=4 sts=4 sw=4 expandtab
 */
