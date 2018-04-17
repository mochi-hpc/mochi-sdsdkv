/*
 * Copyright (c) 2018      Los Alamos National Security, LLC
 *                         All rights reserved.
 *
 * This file is part of the sdsdkv project. See the LICENSE file at the
 * top-level directory of this distribution.
 */

/**
 * @file sdsdkv-client.h
 */

#pragma once

#include "sdsdkv-config.h"
#include "sdsdkv-personality.h"

#include "sdskv-client.h"

#define SDSDKV_CLIENT_VERBOSE

struct sdsdkv_client : public sdsdkv_personality {
    int
    open(void) {
#ifdef SDSDKV_CLIENT_VERBOSE
        printf("hi from client\n");
#endif
        return SDSDKV_SUCCESS;
    }
public:
    //
    sdsdkv_client(
        const sdsdkv_config &config
    ) : sdsdkv_personality(config) { }
    //
    virtual
    ~sdsdkv_client(void) = default;
};

/*
 * vim: ft=cpp ts=4 sts=4 sw=4 expandtab
 */
