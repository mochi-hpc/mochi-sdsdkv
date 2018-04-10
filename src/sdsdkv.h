/*
 * Copyright (c) 2018      Los Alamos National Security, LLC
 *                         All rights reserved.
 *
 * This file is part of the sdsdkv project. See the LICENSE file at the
 * top-level directory of this distribution.
 */

/**
 * @file sdsdkv.h
 */

#pragma once

// For exported MPI types.
#include "mpi.h"

#ifdef __cplusplus
extern "C" {
#endif

/** Convenience definition (in case anyone needs this). */
#define SDSDKV 1

/** Opaque sdsdkv context. */
struct sdsdkv;
/** Convenience typedef. */
typedef struct sdsdkv sdsdkv_t;
/** External context type. */
typedef sdsdkv_t* sdsdkv_context;

enum {
    /** Success. */
    SDSDKV_SUCCESS = 0,
    /** Success, but already done. */
    SDSDKV_SUCCESS_ALREADY_DONE,
    /** General error---fatal. */
    SDSDKV_ERR,
    /** System error---fatal. */
    SDSDKV_ERR_SYS,
    /** Out of resources error---fatal. */
    SDSDKV_ERR_OOR,
    /** Invalid argument provided to library---usually fatal. */
    SDSDKV_ERR_INVLD_ARG,
    /** Library call before sdsdkv_init was called---improper use of library. */
    SDSDKV_ERR_CALL_BEFORE_INIT,
    /** MPI error---fatal. */
    SDSDKV_ERR_MPI,
    /** Action not supported---usually not fatal. */
    SDSDKV_ERR_NOT_SUPPORTED,
    /** The thing that you were looking for wasn't found---not fatal. */
    SDSDKV_ERR_NOT_FOUND
};

/**
 *
 */
int
sdsdkv_create(
    sdsdkv_context *c,
    MPI_Comm c_comm
);

/**
 *
 */
int
sdsdkv_destroy(
    sdsdkv_context c
);

#ifdef __cplusplus
}
#endif
