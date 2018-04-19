/*
 * Copyright (c) 2018      Los Alamos National Security, LLC
 *                         All rights reserved.
 *
 * This file is part of the sdsdkv project. See the LICENSE file at the
 * top-level directory of this distribution.
 */

#include "sdsdkv.h"

#include <cassert>
#include <cstdlib>
#include <unistd.h>

#include "mpi.h"

#define ABORT(id, rc) \
do { \
    abort_job(__LINE__, id, rc); \
} while (0)

static void
abort_job(
    int line,
    int who = -1,
    int why = -1
) {
    fprintf(
        stderr,
        "ERROR DETECTED (who=%d, line=%d, rc=%d)\n",
        who, line, why
    );
    MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
}

int
main(int argc, char **argv)
{
    sdsdkv_context dkvc;
    int rank = 0, numpe = 0;

    int erc = MPI_Init(&argc, &argv);
    if (erc != MPI_SUCCESS) ABORT(rank, erc);

    erc = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (erc != MPI_SUCCESS) ABORT(rank, erc);
    erc = MPI_Comm_size(MPI_COMM_WORLD, &numpe);
    if (erc != MPI_SUCCESS) ABORT(rank, erc);
    assert(numpe == 4 && "numpe must equal 4");

    sdsdkv_config dkv_config = {
        /* .init_comm = */
        MPI_COMM_WORLD,
        /* .personality = */
        (rank % 2 == 0) ? SDSDKV_PERSONALITY_CLIENT : SDSDKV_PERSONALITY_SERVER,
        /* .hash_be = */
        SDSDKV_HASHING_CH_PLACEMENT,
        /* .db_type = */
        SDSDKV_DB_MAP,
        /* .group_name = */
        (char *)"test-group",
        /* .db_name = */
        (char *)"db/test",
        /* .comm_protocol */
        (char *)"tcp",
    };
    //
    erc = sdsdkv_create(&dkvc, &dkv_config);
    if (erc != SDSDKV_SUCCESS) ABORT(rank, erc);
    //
    erc = sdsdkv_open(dkvc);
    if (erc != SDSDKV_SUCCESS) ABORT(rank, erc);
    //
    if (dkv_config.personality == SDSDKV_PERSONALITY_CLIENT) {
        for (int i = 0; i < 2; ++i) {
            uint64_t key = i + rank;
            int value = key + 1;
            erc = sdsdkv_put(
                      dkvc,
                      (const void *)&key,
                      sizeof(int),
                      (const void *)&value,
                      sizeof(int)
                  );
            if (erc != SDSDKV_SUCCESS) ABORT(rank, erc);
        }
    }
    sleep(2);
    if (rank == 0) {
        int ranks[2] = {0, 2};
        for (int r = 0; r < 2; ++r) {
            //
            for (int i = 0; i < 2; ++i) {
                uint64_t key = i + ranks[r];
                int value = -1;
                uint64_t value_size = sizeof(int);
                erc = sdsdkv_get(
                          dkvc,
                          (const void *)&key,
                          sizeof(int),
                          &value,
                          &value_size
                      );
                if (erc != SDSDKV_SUCCESS) ABORT(rank, erc);
                printf("rank=%d (key=%lu, val=%d)\n", ranks[r], key, value);
            }
        }
    }
    sleep(5);
    //
    erc = sdsdkv_destroy(dkvc);
    if (erc != SDSDKV_SUCCESS) ABORT(rank, erc);
    //
    erc = MPI_Finalize();
    if (erc != SDSDKV_SUCCESS) ABORT(rank, erc);
    //
    return EXIT_SUCCESS;
}
