/*
 * Copyright (c) 2018      Los Alamos National Security, LLC
 *                         All rights reserved.
 *
 * This file is part of the sdsdkv project. See the LICENSE file at the
 * top-level directory of this distribution.
 */

#include "sdsdkv.h"

#include <cstdlib>

#include "mpi.h"

int
main(int argc, char **argv)
{
    sdsdkv_context dkvc;

    int erc = MPI_Init(&argc, &argv);
    if (erc != MPI_SUCCESS) return EXIT_FAILURE;

    int rank = 0, numpe = 0;
    erc = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (erc != MPI_SUCCESS) return EXIT_FAILURE;
    erc = MPI_Comm_size(MPI_COMM_WORLD, &numpe);
    if (erc != MPI_SUCCESS) return EXIT_FAILURE;

    sdsdkv_config dkv_config = {
        /* .init_comm   = */ MPI_COMM_WORLD,
        /* .personality = */ (rank % 2 == 0) ? SDSDKV_PERSONALITY_CLIENT :
                                               SDSDKV_PERSONALITY_SERVER
    };

    erc = sdsdkv_create(&dkvc, &dkv_config);
    if (erc != SDSDKV_SUCCESS) return EXIT_FAILURE;

    erc = sdsdkv_destroy(dkvc);
    if (erc != SDSDKV_SUCCESS) return EXIT_FAILURE;

    erc = MPI_Finalize();
    if (erc != MPI_SUCCESS) return EXIT_FAILURE;

    return EXIT_SUCCESS;
}
