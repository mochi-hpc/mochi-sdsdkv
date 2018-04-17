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

#include "mpi.h"

#include <cstdlib>
#include <stdexcept>

struct sdsdkv_mpi {
private:
    //
    sdsdkv_mpi(void) = delete;
    // Dup of initializing communicator.
    MPI_Comm m_commchan = MPI_COMM_NULL;
public:
    //
    sdsdkv_mpi(
        MPI_Comm c_comm
    ) {
        int rc = MPI_Comm_dup(c_comm, &m_commchan);
        if (rc != MPI_SUCCESS) {
            throw std::runtime_error("MPI_Comm_dup");
        }
    }
    //
    ~sdsdkv_mpi(void)
    {
        (void)MPI_Comm_free(&m_commchan);
    }
};

/*
 * vim: ft=cpp ts=4 sts=4 sw=4 expandtab
 */
