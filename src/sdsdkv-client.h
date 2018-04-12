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

#include "sdskv-client.h"

#define SDSDKV_CLIENT_VERBOSE

struct sdsdkv_client {
    static int
    open(
        sdsdkv_config &config
    ) {
#ifdef SDSDKV_CLIENT_VERBOSE
        printf("hi from client\n");
#endif
        return SDSDKV_SUCCESS;
    }
};

/*
 * vim: ft=cpp ts=4 sts=4 sw=4 expandtab
 */
