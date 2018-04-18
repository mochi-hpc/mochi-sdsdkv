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

#include "margo.h"

class personality {
protected:
    //
    sdsdkv_iconfig *m_config;
    //
    margo_instance_id m_mid;
public:
    //
    personality(
        void
    ) : m_config(nullptr)
      , m_mid(MARGO_INSTANCE_NULL) { }
    //
    virtual
    ~personality(void)
    {
        delete m_config;
    }
    //
    virtual int
    init(
        const sdsdkv_config &config
    ) {
        m_config = new sdsdkv_iconfig();
        return m_config->init(config);
    }
    //
    virtual int
    open(void) = 0;
    //
    virtual int
    xchange_addrs(void) = 0;
};

/*
 * vim: ft=cpp ts=4 sts=4 sw=4 expandtab
 */
