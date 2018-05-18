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
#include <string>
#include <cinttypes>

#include "mpi.h"

static const int nkeys = 21;

#define ABORT(id, rc)                                                          \
do {                                                                           \
    abort_job(__LINE__, id, rc);                                               \
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

static void
do_puts(
    sdsdkv_context dkvc,
    int rank
) {
    for (int i = 0; i < nkeys; ++i) {
        uint64_t key = i + rank;
        uint64_t value = key + 1;
        int erc = sdsdkv_put(
                      dkvc,
                      (const void *)&key,
                      sizeof(key),
                      (const void *)&value,
                      sizeof(value)
                  );
        if (erc != SDSDKV_SUCCESS) ABORT(rank, erc);
    }
}

static void
do_gets(
    sdsdkv_context dkvc,
    int rank
) {
    for (int i = 0; i < nkeys; ++i) {
        uint64_t key = i + rank;
        uint64_t value = -1;
        uint64_t value_size = sizeof(value);
        int erc = sdsdkv_get(
                      dkvc,
                      (const void *)&key,
                      sizeof(key),
                      &value,
                      &value_size
                  );
        if (erc != SDSDKV_SUCCESS) ABORT(rank, erc);
        if (value != key + 1) ABORT(rank, -1);
        if (sizeof(value) != value_size) ABORT(rank, -2);
        printf(
            "rank=%d (key=%" PRIu64 ", val=%" PRIu64 ")\n",
            rank, key, value
        );
    }
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

    char *db_prefix = getenv("PWD");
    if (!db_prefix) db_prefix = (char *)"/tmp";
    std::string db_name = std::string(db_prefix) + "/TEST-DB";

    sdsdkv_config_personality personality = (
        (rank % 2 == 0) ? SDSDKV_PERSONALITY_SERVER : SDSDKV_PERSONALITY_CLIENT
    );

    MPI_Comm pcomm;
    erc = MPI_Comm_split(MPI_COMM_WORLD, personality, rank, &pcomm);
    if (erc != MPI_SUCCESS) ABORT(rank, erc);

    sdsdkv_config dkv_config = {
        /* .init_comm = */
        MPI_COMM_WORLD,
        /* .personality = */
        personality,
        /* .hash_be = */
        SDSDKV_HASHING_CH_PLACEMENT,
        /* .db_type = */
        SDSDKV_DB_LEVELDB,
        /* .cmp_fn= */
        SDSDKV_COMPARE_DEFAULT,
        /* .group_name = */
        (char *)"groupname",
        /* .db_path = */
        (char *)db_name.c_str(),
        /* .comm_protocol */
        (char *)"ofi+tcp",
    };
    //
    erc = sdsdkv_create(&dkvc, &dkv_config);
    if (erc != SDSDKV_SUCCESS) ABORT(rank, erc);
    //
    erc = sdsdkv_open(dkvc);
    if (erc != SDSDKV_SUCCESS) ABORT(rank, erc);
    // Server instances wait here until shut down by a client during
    // sdsdkv_destroy.
    if (personality == SDSDKV_PERSONALITY_CLIENT) {
        do_puts(dkvc, rank);
        MPI_Barrier(pcomm);
        do_gets(dkvc, rank);
        MPI_Barrier(pcomm);
    }
    //
    erc = sdsdkv_destroy(dkvc);
    if (erc != SDSDKV_SUCCESS) ABORT(rank, erc);
    //
    erc = MPI_Comm_free(&pcomm);
    if (erc != SDSDKV_SUCCESS) ABORT(rank, erc);
    //
    erc = MPI_Finalize();
    if (erc != SDSDKV_SUCCESS) ABORT(rank, erc);
    //
    return EXIT_SUCCESS;
}
