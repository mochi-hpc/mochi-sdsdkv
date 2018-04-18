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
#include "ssg.h"
#include "ssg-mpi.h"
#include "sdskv-server.h"

#include <string>

#define SDSDKV_SERVER_VERBOSE

static void
group_update_cb(
    ssg_membership_update_t update,
    void *cb_dat
) {
    int world_id = *(int *)cb_dat;

    switch (update.type) {
        case SSG_MEMBER_ADD:
            printf("%d SSG update: ADD member %lu\n", world_id, update.member);
            break;
        case SSG_MEMBER_REMOVE:
            printf("%d SSG update: ADD member %lu\n", world_id, update.member);
            break;
    }
}

struct sdsdkv_server : public personality {
private:
    //
    sdskv_provider_t m_provider;
    //
    sdskv_database_id_t m_dbid;
    //
    hg_addr_t m_margo_addr;
    //
    std::string m_margo_addr_str;
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
        int rc = m_margo_set_addrs();
        if (rc != SDSDKV_SUCCESS) {
            return rc;
        }
        //
        return SDSDKV_SUCCESS;
    }
    //
    int
    m_ssg_init(void)
    {
        int rc = ssg_init(m_mid);
        if (rc != SSG_SUCCESS) {
            return SDSDKV_ERR_SERVICE;
        }
        //
        int world_id = m_mpi->get_world_id();
        m_gid = ssg_group_create_mpi(
                    m_config->group_name.c_str(),
                    // TODO(skg) is this correct?
                    m_mpi->get_peronality_comm(),
                    &group_update_cb,
                    &world_id
                );
        if (m_gid == SSG_GROUP_ID_NULL) {
            return SDSDKV_ERR_SERVICE;
        }
        //
        return SDSDKV_SUCCESS;
    }
    //
    int
    m_margo_set_addrs(void)
    {
        int rc = SDSDKV_SUCCESS;
        char self_addr_str[128];
        hg_size_t self_addr_str_sz = sizeof(self_addr_str);
        //
        hg_return_t hrc = margo_addr_self(m_mid, &m_margo_addr);
        if (hrc != HG_SUCCESS) {
            rc = SDSDKV_ERR_SERVICE;
            goto err;
        }
        //
        hrc = margo_addr_to_string(
                  m_mid,
                  self_addr_str,
                  &self_addr_str_sz,
                  m_margo_addr
              );
        if (hrc != HG_SUCCESS) {
            rc = SDSDKV_ERR_SERVICE;
            goto err;
        }
        m_margo_addr_str = std::string(self_addr_str);
        //
        return rc;
err:
        margo_addr_free(m_mid, m_margo_addr);
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
        int rc = sdskv_provider_add_database(
                     m_provider,
                     m_config->db_name.c_str(),
                     sdsdkv_iconfig::get_real_db_type(m_config->db_type),
                     NULL /* sdskv_compare_fn comp_fn */,
                     &m_dbid
                 );
        if (rc != SDSKV_SUCCESS) {
            margo_finalize(m_mid);
            return SDSDKV_ERR_SERVICE;
        }
        //
        return SDSDKV_SUCCESS;
    }

public:
    //
    sdsdkv_server(void) = default;
    //
    virtual
    ~sdsdkv_server(void)
    {
        margo_addr_free(m_mid, m_margo_addr);
        margo_finalize(m_mid);
    }
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
        rc = m_ssg_init();
        if (rc != SDSDKV_SUCCESS) {
            return rc;
        }
        //
        rc = xchange_gid();
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
        hg_size_t gsize = ssg_get_group_size(m_gid);
        printf("hi from server %s, size %lu\n", m_margo_addr_str.c_str(), gsize);
#endif
        margo_wait_for_finalize(m_mid);
        //
        return SDSDKV_SUCCESS;
    }
    //
    int
    xchange_gid(void)
    {
        char *gid_bits = NULL;
        size_t gid_size = 0;
        //
        ssg_group_id_serialize(m_gid, &gid_bits, &gid_size);
        if (!gid_bits || gid_size == 0) {
            return SDSDKV_ERR_SERVICE;
        }
        //
        unsigned long gid_size_ul = gid_size;
        const int root = m_mpi->get_server_delegate_world_id();
        int rc = SDSDKV_SUCCESS;
        // Send size.
        rc = m_mpi->bcast(
                 &gid_size_ul,
                 1,
                 MPI_UNSIGNED_LONG,
                 root,
                 m_mpi->get_world_comm()
             );
        if (rc != SDSDKV_SUCCESS) goto out;
        // Send data.
        rc = m_mpi->bcast(
                 gid_bits,
                 gid_size_ul,
                 MPI_BYTE,
                 root,
                 m_mpi->get_world_comm()
             );
        if (rc != SDSDKV_SUCCESS) goto out;
    out:
        free(gid_bits);
        //
        return rc;
    }
};

/*
 * vim: ft=cpp ts=4 sts=4 sw=4 expandtab
 */
