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

struct sdsdkv_client : public personality {
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
                       m_config->comm_protocol.c_str(),
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
    int
    m_ssg_init(void)
    {
        int rc = ssg_init(m_mid);
        if (rc != SSG_SUCCESS) {
            return SDSDKV_ERR_SERVICE;
        }
        rc = ssg_group_attach(m_gid);
        if (rc != SSG_SUCCESS) {
            return SDSDKV_ERR_SERVICE;
        }
        //
        return SDSDKV_SUCCESS;
    }
public:
    //
    sdsdkv_client(void) = default;
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
        rc = xchange_gid();
        if (rc != SDSDKV_SUCCESS) {
            return rc;
        }
        rc = m_ssg_init();
        if (rc != SDSDKV_SUCCESS) {
            return rc;
        }
#ifdef SDSDKV_CLIENT_VERBOSE
        hg_size_t gsize = ssg_get_group_size(m_gid);
        printf("hi from client size %lu\n", gsize);
#endif
        //
        return SDSDKV_SUCCESS;
    }
    //
    int
    xchange_gid(void)
    {
        char *gid_bits = NULL;
        unsigned long gid_size_ul = 0;
        //
        const int root = m_mpi->get_server_delegate_world_id();
        int rc = SDSDKV_SUCCESS;
        // Get size.
        rc = m_mpi->bcast(
                 &gid_size_ul,
                 1,
                 MPI_UNSIGNED_LONG,
                 root,
                 m_mpi->get_world_comm()
             );
        if (rc != SDSDKV_SUCCESS) goto out;
        //
        gid_bits = (char *)calloc(gid_size_ul, sizeof(*gid_bits));
        if (!gid_bits) return SDSDKV_ERR_OOR;
        // Get data.
        rc = m_mpi->bcast(
                 gid_bits,
                 gid_size_ul,
                 MPI_BYTE,
                 root,
                 m_mpi->get_world_comm()
             );
        if (rc != SDSDKV_SUCCESS) goto out;
        //
        ssg_group_id_deserialize(gid_bits, gid_size_ul, &m_gid);
    out:
        free(gid_bits);
        //
        return rc;
    }
};

/*
 * vim: ft=cpp ts=4 sts=4 sw=4 expandtab
 */
