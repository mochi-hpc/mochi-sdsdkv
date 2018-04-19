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
#include "ch-placement.h"

#include <iostream> // TODO(skg) Not really needed. Just for debug.
#include <vector>
#include <utility>

#define SDSDKV_CLIENT_VERBOSE

struct sdsdkv_client : public personality {
private:
    //
    sdskv_client_t m_kvcl;
    //
    struct ch_placement_instance *m_place;
    //
    std::vector<
        std::pair<sdskv_provider_handle_t, sdskv_database_id_t>
    > m_ph_dbs;
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
        if (m_mid == MARGO_INSTANCE_NULL) return SDSDKV_ERR_SERVICE;
        //
        int erc = sdskv_client_init(m_mid, &m_kvcl);
        if(erc != SDSKV_SUCCESS) return SDSDKV_ERR_SERVICE;
        //
        return SDSDKV_SUCCESS;
    }
    //
    int
    m_ssg_init(void)
    {
        int rc = ssg_init(m_mid);
        if (rc != SSG_SUCCESS) return SDSDKV_ERR_SERVICE;
        //
        rc = ssg_group_attach(m_gid);
        if (rc != SSG_SUCCESS) return SDSDKV_ERR_SERVICE;
        //
        return SDSDKV_SUCCESS;
    }
    //
    int
    m_db_init(void)
    {
        hg_size_t gsize = ssg_get_group_size(m_gid);
        //
        for (decltype(gsize) i = 0; i < gsize; ++i) {
            hg_addr_t server_addr = ssg_get_addr(m_gid, i);
            std::string addr_str = m_addr_to_string(server_addr);
            printf(
                "CLIENT(world_id=%d) %s\n",
                m_mpi->get_world_id(),
                addr_str.c_str()
            );
            // TODO(skg) Do I need to do this for each one? Is one sufficient?
            sdskv_provider_handle_t kvph;
            int rc = sdskv_provider_handle_create(
                          m_kvcl,
                          server_addr,
                          1,
                          &kvph
                      );
            if (rc != SDSKV_SUCCESS) return SDSDKV_ERR_SERVICE;
            //
            sdskv_database_id_t db_id;
            rc = sdskv_open(kvph, m_config->db_name.c_str(), &db_id);
            if (rc != SDSKV_SUCCESS) return SDSDKV_ERR_SERVICE;
            //
            m_ph_dbs.push_back(std::make_pair(kvph, db_id));
            //
            printf(
                "CLIENT(world_id=%d) DB_OPEN!\n",
                m_mpi->get_world_id()
            );
        }
        //
        return SDSDKV_SUCCESS;
    }
    // TODO(skg) Add ability to change placement implementation.
    int
    m_placement_init(void)
    {
        hg_size_t gsize = ssg_get_group_size(m_gid);
        // TODO(skg) Understand what this means.
        static const int virt_factor = 4;
        static const int seed = 0;
        m_place = ch_placement_initialize(
                      "hash_lookup3",
                      gsize,
                      virt_factor,
                      seed
                  );
        if (!m_place) return SDSDKV_ERR_SERVICE;
        //
        return SDSDKV_SUCCESS;
    }
    //
    unsigned long
    m_placement_find_closest(
        const void *key
    ) {
        static unsigned long server_indices[CH_MAX_REPLICATION];
        // TODO(skg) ???
        static const unsigned int replication = 1;
        //
        ch_placement_find_closest(
            m_place,
            // TODO(skg) How to properly handle arbitrary data?
            *(uint64_t *)(key),
            replication,
            server_indices
        );
        return server_indices[0];
    }
public:
    //
    sdsdkv_client(void) : m_place(nullptr) { }
    //
    virtual
    ~sdsdkv_client(void)
    {
        // TODO(skg) should be in finalize.
        //margo_finalize(m_mid);
    }
    //
    int
    open(void) {
        int rc = m_margo_init();
        if (rc != SDSDKV_SUCCESS) return rc;
        //
        rc = xchange_gid();
        if (rc != SDSDKV_SUCCESS) return rc;
        //
        rc = m_ssg_init();
        if (rc != SDSDKV_SUCCESS) return rc;
        //
        rc = m_placement_init();
        if (rc != SDSDKV_SUCCESS) return rc;
        //
        rc = m_db_init();
        if (rc != SDSDKV_SUCCESS) return rc;
        //
        return SDSDKV_SUCCESS;
    }
    //
    int
    xchange_gid(void)
    {
        char *gid_bits = nullptr;
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
        if (gid_bits) free(gid_bits);
        //
        return rc;
    }
    //
    int
    put(
        const void *key,
        uint64_t key_size,
        const void *value,
        uint64_t value_size
    ) {
        // Get ID for target server.
        const unsigned long sid = m_placement_find_closest(key);
        // Stash info needed for put.
        printf("put --> serverid=%lu\n", sid);
        const auto ph_db = m_ph_dbs[sid];
        const auto provider = ph_db.first;
        const auto db = ph_db.second;
        // Actually do the put.
        // TODO(skg) Capture return value and improve reporting to caller.
        int rc = sdskv_put(provider, db, key, key_size, value, value_size);
        if (rc != SDSKV_SUCCESS) return SDSDKV_ERR;
        //
        return SDSDKV_SUCCESS;
    }
    // TODO(skg) abstract find_closest
    int
    get(
        const void *key,
        uint64_t key_size,
        void *value,
        uint64_t *value_size
    ) {
        // Get ID for target server.
        const unsigned long sid = m_placement_find_closest(key);
        // Stash info needed for put.
        printf("get --> serverid=%lu\n", sid);
        const auto ph_db = m_ph_dbs[sid];
        const auto provider = ph_db.first;
        const auto db = ph_db.second;
        // Actually do the get.
        // TODO(skg) Capture return value and improve reporting to caller.
        int rc = sdskv_get(provider, db, key, key_size, value, value_size);
        if (rc != SDSKV_SUCCESS) return SDSDKV_ERR;
        //
        return SDSDKV_SUCCESS;
    }
};

/*
 * vim: ft=cpp ts=4 sts=4 sw=4 expandtab
 */
