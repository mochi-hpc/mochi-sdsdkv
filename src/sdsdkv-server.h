/*
 * Copyright (c) 2018      Los Alamos National Security, LLC
 *                         All rights reserved.
 *
 * This file is part of the sdsdkv project. See the LICENSE file at the
 * top-level directory of this distribution.
 */

/**
 * @file sdsdkv-server.h
 */

#pragma once

#include "sdsdkv.h"

#define SDSDKV_SERVER_VERBOSE

struct sdsdkv_server {
    static int
    open(
        sdsdkv_context c
    ) {
#ifdef SDSDKV_SERVER_VERBOSE
        printf("hi from server\n");
#endif
        return SDSDKV_SUCCESS;
    }
};

/*
 * vim: ft=cpp ts=4 sts=4 sw=4 expandtab
 */
