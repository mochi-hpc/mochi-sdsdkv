/*
 * Copyright (c) 2018      Los Alamos National Security, LLC
 *                         All rights reserved.
 *
 * This file is part of the sdsdkv project. See the LICENSE file at the
 * top-level directory of this distribution.
 */

/**
 * @file sdsdkv-client.h
 */

#pragma once

#include "sdsdkv-config.h"
#include "sdsdkv-personality.h"

#include "sdskv-client.h"

#define SDSDKV_CLIENT_VERBOSE

struct sdsdkv_client : public sdsdkv_personality {
private:
    //
    sdskv_client_t m_kvcl;
    //
    int
    m_margo_init(void)
    {
        static const int use_progress_thread = 0;
        // A value of -1 directs Margo to use the same execution context as that
        // used for Mercury progress.
        static const int rpc_thread_count = -1;
        //
        m_mid = margo_init(
                       m_config->comm_protocol,
                       MARGO_CLIENT_MODE,
                       use_progress_thread,
                       rpc_thread_count
                   );
        if (m_mid == MARGO_INSTANCE_NULL) {
            return SDSDKV_ERR_SERVICE;
        }
        //
        int erc = sdskv_client_init(m_mid, &m_kvcl);
        if(erc != SDSKV_SUCCESS) {
            margo_finalize(m_mid);
            return SDSDKV_ERR_SERVICE;
        }
        //
        return SDSDKV_SUCCESS;
    }
public:
    //
    sdsdkv_client(
        const sdsdkv_config &config
    ) : sdsdkv_personality(config) { }
    //
    virtual
    ~sdsdkv_client(void) = default;
    //
    int
    open(void) {
        int rc = m_margo_init();
        if (rc != SDSDKV_SUCCESS) {
            return rc;
        }
        //
        rc = xchange_addrs();
        if (rc != SDSDKV_SUCCESS) {
            return rc;
        }
#ifdef SDSDKV_CLIENT_VERBOSE
        printf("hi from client\n");
#endif
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
