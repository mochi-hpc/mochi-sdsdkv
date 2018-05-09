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
    /** Default address string buffer size. */
    static constexpr int s_addr_str_buff_size = 128;
    /** Points to already initialized sdsdkv_mpi instance. */
    sdsdkv_mpi *m_mpi;
    /** Copy of internal representation of user-provided configuration. */
    sdsdkv_iconfig *m_config;
    /** Margo ID. */
    margo_instance_id m_mid;
    /** SSG group ID. */
    ssg_group_id_t m_gid;
    //
    std::string
    m_addr_to_string(
        hg_addr_t target
    ) {
        char addr_str[s_addr_str_buff_size];
        hg_size_t addr_str_sz = sizeof(addr_str);
        std::string result;
        //
        hg_return_t hrc = margo_addr_to_string(
                              m_mid,
                              addr_str,
                              &addr_str_sz,
                              target
                          );
        if (hrc != HG_SUCCESS) return result;
        //
        return std::string(addr_str);
    }
    //
    std::string
    m_self_addr_to_string(void)
    {
        hg_addr_t self_addr;
        //
        hg_return_t hrc = margo_addr_self(m_mid, &self_addr);
        if (hrc != HG_SUCCESS) return std::string();
        //
        std::string result(m_addr_to_string(self_addr));
        //
        margo_addr_free(m_mid, self_addr);
        //
        return result;
    }
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
    //
    virtual int
    close(void) = 0;
    //
    virtual int
    put(
        const void *key,
        uint64_t key_size,
        const void *value,
        uint64_t value_size
    ) = 0;
    //
    virtual int
    get(
        const void *key,
        uint64_t key_size,
        void *value,
        uint64_t *value_size
    ) = 0;
};

/*
 * vim: ft=cpp ts=4 sts=4 sw=4 expandtab
 */
