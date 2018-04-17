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
    /** General error. */
    SDSDKV_ERR,
    /** System error. */
    SDSDKV_ERR_SYS,
    /** Service error. */
    SDSDKV_ERR_SERVICE,
    /** Out of resources error. */
    SDSDKV_ERR_OOR,
    /** Invalid argument provided to library. */
    SDSDKV_ERR_INVLD_ARG,
    /** Invalid configuration provided to library. */
    SDSDKV_ERR_INVLD_CONFIG,
    /** Library call before sdsdkv_create was called. */
    SDSDKV_ERR_CALL_BEFORE_INIT,
    /** MPI error. */
    SDSDKV_ERR_MPI,
    /** Action not supported. */
    SDSDKV_ERR_NOT_SUPPORTED,
    /** The thing that you were looking for wasn't found. */
    SDSDKV_ERR_NOT_FOUND
};

////////////////////////////////////////////////////////////////////////////////
// NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE //
////////////////////////////////////////////////////////////////////////////////
// If you update sdsdkv_config_*, please update sdsdkv-config.h.

/** Defines types of supported process personalities. */
enum sdsdkv_config_personality {
    /** Client personality. */
    SDSDKV_PERSONALITY_CLIENT = 0,
    /** Server personality. */
    SDSDKV_PERSONALITY_SERVER
};

/** Defines types of supported key hashing algorithms. */
enum sdsdkv_config_hashing {
    /** ch-placement. */
    SDSDKV_HASHING_CH_PLACEMENT = 0
};

/** Defines types of supported data stores. */
enum sdsdkv_config_db {
    /** std::map */
    SDSDKV_DB_MAP = 0,
    /** levedb */
    SDSDKV_DB_LEVELDB
};

// TODO(skg) Allow for two modes of operation:
// - Total lash-up
// - Connect

/** Configuration struct. */
typedef struct sdsdkv_config {
    /** Communicator over which infrastructure will be created. */
    MPI_Comm init_comm;
    /** Process personality. */
    sdsdkv_config_personality personality;
    /** Hashing back-end. */
    sdsdkv_config_hashing hash_be;
    /** Database type. */
    sdsdkv_config_db db_type;
    /** Database name. */
    char *db_name;
    /** Communication protocol. */
    char *comm_protocol;
} sdsdkv_config;

/**
 *
 */
int
sdsdkv_create(
    sdsdkv_context *c,
    sdsdkv_config *config
);

/**
 *
 */
int
sdsdkv_open(
    sdsdkv_context c
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

/*
 * vim: ft=cpp ts=4 sts=4 sw=4 expandtab
 */
