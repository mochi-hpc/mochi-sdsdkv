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

#include "sdsdkv.h"
#include "sdsdkv-config.h"

#include "margo.h"

#include <stdexcept>

struct sdsdkv_personality {
protected:
    sdsdkv_config *m_config = nullptr;
    //
    margo_instance_id m_mid = MARGO_INSTANCE_NULL;
    // TODO(skg) FIXME
    char m_margo_addr[4098] = {};
    //
    sdsdkv_personality(void) = default;
    //
    sdsdkv_personality(
        const sdsdkv_config &config
    ) {
        int rc = config_dup(config, &m_config);
        if (rc != SDSDKV_SUCCESS) {
            throw std::runtime_error("config_dup");
        }
    }
public:
    //
    virtual
    ~sdsdkv_personality(void) {
        if (m_config) {
            config_dup_destroy(&m_config);
        }
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
