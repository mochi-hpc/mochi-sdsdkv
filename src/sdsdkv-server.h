/*
 * Copyright (c) 2018      Los Alamos National Security, LLC
 *                         All rights reserved.
 *
 * This file is part of the sdsdkv project. See the LICENSE file at the
 * top-level directory of this distribution.
 */

/**
 * @file sdsdkv-server.h
 */

#pragma once

#include "sdsdkv-personality.h"
#include "sdsdkv-config.h"

#include "margo.h"
#include "sdskv-server.h"

#include <string>

#define SDSDKV_SERVER_VERBOSE

struct sdsdkv_server : public personality {
private:
    //
    sdskv_provider_t m_provider;
    //
    sdskv_database_id_t m_dbid;
    //
    std::string m_margo_addr;
    //
    int
    m_margo_init(void)
    {
#if 0
        static const int use_progress_thread = 0;
        // A value of -1 directs Margo to use the same execution context as that
        // used for Mercury progress.
        static const int rpc_thread_count = -1;
        //
        m_mid = margo_init(
                       m_config->comm_protocol,
                       MARGO_SERVER_MODE,
                       use_progress_thread,
                       rpc_thread_count
                   );
        if (m_mid == MARGO_INSTANCE_NULL) {
            return SDSDKV_ERR_SERVICE;
        }
        //
        margo_enable_remote_shutdown(m_mid);
        //
        int rc = m_margo_set_addr();
        if (rc != SDSDKV_SUCCESS) {
            return rc;
        }
        //
#endif
        return SDSDKV_SUCCESS;
    }
    //
    int
    m_margo_set_addr(void)
    {
        int rc = SDSDKV_SUCCESS;
        // Get server info. TODO(skg) Need to share via MPI?
        hg_addr_t self_addr;
        char self_addr_str[128];
        hg_size_t self_addr_str_sz = sizeof(self_addr_str);
        //
        hg_return_t hrc = margo_addr_self(m_mid, &self_addr);
        if (hrc != HG_SUCCESS) {
            rc = SDSDKV_ERR_SERVICE;
            goto err;
        }
        //
        hrc = margo_addr_to_string(
                  m_mid,
                  self_addr_str,
                  &self_addr_str_sz,
                  self_addr
              );
        if (hrc != HG_SUCCESS) {
            rc = SDSDKV_ERR_SERVICE;
            goto err;
        }
        // No longer needed.
        margo_addr_free(m_mid, self_addr);
        m_margo_addr = std::string(self_addr_str);
        //
        return rc;
err:
        margo_addr_free(m_mid, self_addr);
        margo_finalize(m_mid);
        return rc;
    }
    //
    int
    m_keyval_register_provider(void)
    {
        int rc = sdskv_provider_register(
                     m_mid,
                     1,
                     SDSKV_ABT_POOL_DEFAULT,
                     &m_provider
                 );
        if (rc != SDSKV_SUCCESS) {
            margo_finalize(m_mid);
            return SDSDKV_ERR_SERVICE;
        }
        //
        return SDSDKV_SUCCESS;
    }
    //
    int
    m_keyval_add_db(void)
    {
#if 0
        int rc = sdskv_provider_add_database(
                     m_provider,
                     m_config->db_name,
                     config_get_real_db_type(*m_config),
                     NULL /* sdskv_compare_fn comp_fn */,
                     &m_dbid
                 );
        if (rc != SDSKV_SUCCESS) {
            margo_finalize(m_mid);
            return SDSDKV_ERR_SERVICE;
        }
#endif
        //
        return SDSDKV_SUCCESS;
    }

public:
    //
    sdsdkv_server(void) = default;
    //
    virtual
    ~sdsdkv_server(void) = default;
    //
    int
    open(void)
    {
        //
        int rc = m_margo_init();
        if (rc != SDSDKV_SUCCESS) {
            return rc;
        }
        //
        rc = xchange_addrs();
        if (rc != SDSDKV_SUCCESS) {
            return rc;
        }
        //
        rc = m_keyval_register_provider();
        if (rc != SDSDKV_SUCCESS) {
            return rc;
        }
        //
        rc = m_keyval_add_db();
        if (rc != SDSDKV_SUCCESS) {
            return rc;
        }
#ifdef SDSDKV_SERVER_VERBOSE
        printf("hi from server %s\n", m_margo_addr.c_str());
#endif
        //margo_wait_for_finalize(m_mid);
        //
        return SDSDKV_SUCCESS;
    }
    //
    int
    xchange_addrs(void)
    {
        return SDSDKV_SUCCESS;
    }
};

/*
 * vim: ft=cpp ts=4 sts=4 sw=4 expandtab
 */
