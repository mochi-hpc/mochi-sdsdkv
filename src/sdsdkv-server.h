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
#include "sdsdkv-misci.h"

#include "margo.h"
#include "ssg.h"
#include "ssg-mpi.h"
#include "sdskv-server.h"

#include <string>

#define SDSDKV_SERVER_VERBOSE

static void
finalize_cb(void *cba)
{
    ssg_finalize();
    ssg_group_destroy(*(ssg_group_id_t *)cba);
}

static void
group_update_cb(
    void *cb_dat,
    ssg_member_id_t member_id,
    ssg_member_update_type_t update
) {
    int world_id = *(int *)cb_dat;

    switch (update) {
        case SSG_MEMBER_JOINED:
            printf("%d SSG update: JOINED member %lu\n", world_id, member_id);
            break;
        case SSG_MEMBER_LEFT:
            printf("%d SSG update: RM member %lu\n", world_id, member_id);
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
    sdskv_config_t m_sdskv_config;
    //
    int
    m_margo_init(void)
    {
        static const int use_progress_thread = 0;
        //
        m_mid = margo_init(
                       m_config->comm_protocol.c_str(),
                       MARGO_SERVER_MODE,
                       use_progress_thread,
                       m_config->rpc_thread_count
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
    m_ssg_init(void)
    {
        int rc = ssg_init();
        if (rc != SSG_SUCCESS) return SDSDKV_ERR_SERVICE;
        //
        int world_id = m_mpi->get_world_id();
        m_gid = ssg_group_create_mpi(m_mid,
                    m_config->group_name.c_str(),
                    m_mpi->get_peronality_comm(),
                    //&group_update_cb,
                    //&world_id
                    NULL,
                    NULL,
                    NULL
                );
        if (m_gid == SSG_GROUP_ID_INVALID) return SDSDKV_ERR_SERVICE;
        //
        return SDSDKV_SUCCESS;
    }
    //
    int
    m_keyval_register_provider(void)
    {
        const int pid = m_mpi->get_pgroup_id();
        //
        int rc = sdskv_provider_register(
                     m_mid,
                     pid + 1,
                     SDSKV_ABT_POOL_DEFAULT,
                     &m_provider
                 );
        if (rc != SDSKV_SUCCESS) return sdskv2irc(rc);
        //
        return SDSDKV_SUCCESS;
    }
    //
    const char *
    m_get_cmp_fn_name(
        const std::string &i_name
    ) {
        if (i_name.empty()) return SDSDKV_COMPARE_DEFAULT;
        else return i_name.c_str();
    }
    //
    int
    m_keyval_add_db(void)
    {
        // TODO(skg) add a check that requires that all dbs get used. The
        // problem that we are trying to avoid is opening a created data store
        // that was created with a smaller number of servers.
        const int pid = m_mpi->get_pgroup_id();
        const std::string db_name = personality::m_get_db_name(pid);
        // TODO(skg) No overwrite? What's that?
        static const int db_no_overwrite = 0;
        m_sdskv_config = {
            db_name.c_str(),
            m_config->db_path.c_str(),
            sdsdkv_iconfig::get_real_db_type(m_config->db_type),
            m_get_cmp_fn_name(m_config->cmp_fn_name),
            db_no_overwrite
        };
        int rc = sdskv_provider_attach_database(
                     m_provider,
                     &m_sdskv_config,
                     &m_dbid
                 );
        if (rc != SDSKV_SUCCESS) return sdskv2irc(rc);
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
        // NOTE(skg): I know all this barrier stuff is ugly and complicated to
        // get right, but this helps fix an HG race condition...
        int rc = m_margo_init();
        if (rc != SDSDKV_SUCCESS) return rc;
        //
        //m_mpi->barrier(m_mpi->get_world_comm());
        //
        rc = m_ssg_init();
        if (rc != SDSDKV_SUCCESS) return rc;
        //
        rc = xchange_gid();
        if (rc != SDSDKV_SUCCESS) return rc;
        //
        //m_mpi->barrier(m_mpi->get_world_comm());
        //
        rc = m_keyval_register_provider();
        if (rc != SDSDKV_SUCCESS) return rc;
        //
        //m_mpi->barrier(m_mpi->get_world_comm());
        //
        rc = m_keyval_add_db();
        if (rc != SDSDKV_SUCCESS) return rc;
#ifdef SDSDKV_SERVER_VERBOSE
        {
            std::string addr_str = m_self_addr_to_string();
            printf(
                "SERVER(world_id=%d) %s\n",
                m_mpi->get_world_id(),
                addr_str.c_str()
            );
        }
#endif
        margo_push_finalize_callback(m_mid, finalize_cb, &m_mid);
        //
        margo_wait_for_finalize(m_mid);
#ifdef SDSDKV_SERVER_VERBOSE
        printf(
            "SERVER WAIT FOR FINALIZE COMPLETE (world_id=%d)\n",
            m_mpi->get_world_id()
        );
#endif
        return SDSDKV_SUCCESS;
    }
    //
    int
    close(void)
    {
        //
        return SDSDKV_SUCCESS;
    }
    //
    int
    xchange_gid(void)
    {
        char *gid_bits = nullptr;
        size_t gid_size = 0;
        //
        ssg_group_id_serialize(m_gid, 1, &gid_bits, &gid_size);
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
        if (gid_bits) free(gid_bits);
        //
        return rc;
    }
    //
    int
    put(
        const void *,
        uint64_t,
        const void *,
        uint64_t
    ) {
        return SDSDKV_ERR_NOT_SUPPORTED;
    }
    //
    int
    get(
        const void *,
        uint64_t,
        void *,
        uint64_t *
    ) {
        return SDSDKV_ERR_NOT_SUPPORTED;
    }
};

/*
 * vim: ft=cpp ts=4 sts=4 sw=4 expandtab
 */
