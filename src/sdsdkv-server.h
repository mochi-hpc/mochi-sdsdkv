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

struct sdsdkv_server : public sdsdkv_personality {
private:
    //
    sdsdkv_server(void) = delete;
    //
    int
    init_margo(void) {
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
        return SDSDKV_SUCCESS;
    }
    //
    int
    get_margo_addr(
        std::string &addr_str
    ) {
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
        addr_str = std::string(self_addr_str);
        //
        return rc;
err:
        margo_addr_free(m_mid, self_addr);
        margo_finalize(m_mid);
        return rc;
    }

public:
    //
    sdsdkv_server(
        const sdsdkv_config &config
    ) : sdsdkv_personality(config) { }
    //
    virtual
    ~sdsdkv_server(void) = default;
    //
    int
    open(void) {
        //
        int rc = init_margo();
        if (rc != SDSDKV_SUCCESS) {
            return rc;
        }
        //
        std::string self_addr_str;
        rc = get_margo_addr(self_addr_str);
        if (rc != SDSDKV_SUCCESS) {
            return rc;
        }
#ifdef SDSDKV_SERVER_VERBOSE
        printf("hi from server %s\n", self_addr_str.c_str());
#endif
        return SDSDKV_SUCCESS;
    }
};

/*
 * vim: ft=cpp ts=4 sts=4 sw=4 expandtab
 */
