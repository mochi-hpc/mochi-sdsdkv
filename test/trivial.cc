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

    erc = sdsdkv_create(&dkvc, MPI_COMM_WORLD);
    if (erc != SDSDKV_SUCCESS) return EXIT_FAILURE;

    erc = sdsdkv_destroy(dkvc);
    if (erc != SDSDKV_SUCCESS) return EXIT_FAILURE;

    erc = MPI_Finalize();
    if (erc != MPI_SUCCESS) return EXIT_FAILURE;

    return EXIT_SUCCESS;
}
