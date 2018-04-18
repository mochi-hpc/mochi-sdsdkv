/*
 * Copyright (c) 2018      Los Alamos National Security, LLC
 *                         All rights reserved.
 *
 * This file is part of the sdsdkv project. See the LICENSE file at the
 * top-level directory of this distribution.
 */

/**
 * @file sdsdkv-config.h
 */

#pragma once

#include "sdsdkv.h"
#include "sdskv-common.h"

#include <cstdlib>
#include <string>

/** Internal config representation. */
class sdsdkv_iconfig {
    //
    static bool
    valid(
        const sdsdkv_config &config
    ) {
        switch (config.personality) {
            case SDSDKV_PERSONALITY_CLIENT:
            case SDSDKV_PERSONALITY_SERVER:
                break;
            default:
                return false;
        }
        //
        switch (config.hash_be) {
            case SDSDKV_HASHING_CH_PLACEMENT:
                break;
            default:
                return false;
        }
        //
        switch (config.db_type) {
            case SDSDKV_DB_MAP:
            case SDSDKV_DB_LEVELDB:
                break;
            default:
                return false;
        }
        //
        return true;
    }
public:
    // See sdsdkv.h to see what we are replicating.  ///////////////////////////
    //
    MPI_Comm init_comm;
    //
    sdsdkv_config_personality personality;
    //
    sdsdkv_config_hashing hash_be;
    //
    sdsdkv_config_db db_type;
    //
    std::string db_name;
    //
    std::string comm_protocol;
    //
    sdsdkv_iconfig(void) = default;
    //
    ~sdsdkv_iconfig(void) = default;
    //
    int
    init(
        const sdsdkv_config &config
    ) {
        if (!valid(config)) {
            return SDSDKV_ERR_INVLD_CONFIG;
        }
        //
        init_comm = config.init_comm;
        personality = config.personality;
        hash_be = config.hash_be;
        db_type = config.db_type;
        db_name = std::string(config.db_name);
        comm_protocol = std::string(config.comm_protocol);
        //
        return SDSDKV_SUCCESS;
    }
    //
    static sdskv_db_type_t
    get_real_db_type(
        sdsdkv_config_db itype
    ) {
        switch (itype) {
            case SDSDKV_DB_MAP:
                return KVDB_MAP;
            case SDSDKV_DB_LEVELDB:
                return KVDB_LEVELDB;
            default:
                return sdskv_db_type_t(-187);
        }
    }
};

/*
 * vim: ft=cpp ts=4 sts=4 sw=4 expandtab
 */
