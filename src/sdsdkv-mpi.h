/*
 * Copyright (c) 2018      Los Alamos National Security, LLC
 *                         All rights reserved.
 *
 * This file is part of the sdsdkv project. See the LICENSE file at the
 * top-level directory of this distribution.
 */

/**
 * @file sdsdkv-mpi.h
 */

#pragma once

#include "sdsdkv.h"
#include "sdsdkv-config.h"

#include "mpi.h"

#include <cstdlib>
#include <stdexcept>

struct sdsdkv_mpi {
private:
    /** Dup of world communicator. */
    MPI_Comm m_world_comm;
    /** Personality communicator. */
    MPI_Comm m_personality_comm;
    /** My ID in world communicator. */
    int m_world_size;
    /** My ID in world communicator. */
    int m_world_id;
public:
    //
    sdsdkv_mpi(void)
        : m_world_comm(MPI_COMM_NULL)
        , m_personality_comm(MPI_COMM_NULL)
        , m_world_size(0)
        , m_world_id(0) { }
    //
    int
    init(
        const sdsdkv_iconfig &config
    ) {
        int rc = MPI_Comm_dup(config.init_comm, &m_world_comm);
        if (rc != MPI_SUCCESS) goto err;
        //
        rc = MPI_Comm_size(m_world_comm, &m_world_size);
        if (rc != MPI_SUCCESS) goto err;
        //
        rc = MPI_Comm_rank(m_world_comm, &m_world_id);
        if (rc != MPI_SUCCESS) goto err;
        // Create sub-communicators based on personality.
        rc = MPI_Comm_split(
                 m_world_comm,
                 config.personality,
                 m_world_id,
                 &m_personality_comm
             );
        if (rc != MPI_SUCCESS) goto err;
        //
        return SDSDKV_SUCCESS;
err:
        return SDSDKV_ERR_MPI;
    }
    //
    ~sdsdkv_mpi(void)
    {
        (void)MPI_Comm_free(&m_world_comm);
        (void)MPI_Comm_free(&m_personality_comm);
    }
    //
    MPI_Comm
    get_world_comm(void) { return m_world_comm; }
    //
    MPI_Comm
    get_peronality_comm(void) { return m_personality_comm; }
    //
    int
    get_world_size(void) { return m_world_size; }
    //
    int
    get_world_id(void) { return m_world_id; }
};

/*
 * vim: ft=cpp ts=4 sts=4 sw=4 expandtab
 */
