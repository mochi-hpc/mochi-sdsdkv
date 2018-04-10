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

#include "mpi.h"

#include <cstdlib>

struct sdsdkv_mpi {
private:
    // Dup of initializing communicator.
    MPI_Comm m_commchan;
public:
    //
    static int
    create(
        sdsdkv_mpi **mpi,
        MPI_Comm c_comm
    ) {
        if (!mpi) return SDSDKV_ERR_INVLD_ARG;

        sdsdkv_mpi *tmpi = (sdsdkv_mpi *)calloc(1, sizeof(*tmpi));
        if (!tmpi) return SDSDKV_ERR_OOR;

        int erc = MPI_Comm_dup(c_comm, &(tmpi->m_commchan));
        if (erc != MPI_SUCCESS) {
            destroy(tmpi);
            return SDSDKV_ERR_MPI;
        }

        *mpi = tmpi;

        return SDSDKV_SUCCESS;
    }
    //
    static int
    destroy(
        sdsdkv_mpi *mpi
    ) {
        int rc = SDSDKV_SUCCESS;

        if (mpi) {
            int erc = MPI_Comm_free(&(mpi->m_commchan));
            if (erc != MPI_SUCCESS) {
                rc = SDSDKV_ERR_MPI;
            }
            free(mpi);
        }

        return rc;
    }
    //
    sdsdkv_mpi(void) = delete;
    //
    ~sdsdkv_mpi(void) = delete;
};
