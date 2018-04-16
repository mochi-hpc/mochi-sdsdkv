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

#include "sdsdkv-config.h"

#include "margo.h"

#include "sdskv-server.h"

#define SDSDKV_SERVER_VERBOSE

struct sdsdkv_server {
private:
    static int
    init_margo(
        sdsdkv_config &config,
        margo_instance_id &mid
    ) {
        // Mercury host address with port number.
        // TODO(skg)
        auto *addr_str = (const char *)"tcp";
        static const int use_progress_thread = 0;
        // A value of -1 directs Margo to use the same execution context as that
        // used for Mercury progress.
        static const int rpc_thread_count = -1;
        //
        mid = margo_init(
                  addr_str,
                  MARGO_SERVER_MODE,
                  use_progress_thread,
                  rpc_thread_count
              );
        if (mid == MARGO_INSTANCE_NULL) {
            return SDSDKV_ERR_SERVICE;
        }
        //
        margo_enable_remote_shutdown(mid);
        //
        return SDSDKV_SUCCESS;
    }

public:
    static int
    open(
        sdsdkv_config &config
    ) {
        margo_instance_id mid;
        //
        int rc = init_margo(config, mid);
        if (rc != SDSDKV_SUCCESS) {
            return rc;
        }
        // Get server info. TODO(skg) Need to share via MPI?
        hg_addr_t self_addr;
        char self_addr_str[128];
        hg_size_t self_addr_str_sz = sizeof(self_addr_str);
        //
        hg_return_t hrc = margo_addr_self(mid, &self_addr);
        if (hrc != HG_SUCCESS) {
            margo_finalize(mid);
            return SDSDKV_ERR_SERVICE;
        }
        //
        hrc = margo_addr_to_string(mid, self_addr_str, &self_addr_str_sz, self_addr);
        if(hrc != HG_SUCCESS) {
            margo_addr_free(mid, self_addr);
            margo_finalize(mid);
            return SDSDKV_ERR_SERVICE;
        }
        // No longer needed.
        margo_addr_free(mid, self_addr);
#ifdef SDSDKV_SERVER_VERBOSE
        printf("hi from server %s\n", self_addr_str);
#endif
        return SDSDKV_SUCCESS;
    }
};

/*
 * vim: ft=cpp ts=4 sts=4 sw=4 expandtab
 */
