/*
 * Copyright (c) 2018      Los Alamos National Security, LLC
 *                         All rights reserved.
 *
 * This file is part of the sdsdkv project. See the LICENSE file at the
 * top-level directory of this distribution.
 */

/**
 * @file sdsdkv-personality-factory.h
 */

#pragma once

#include "sdsdkv.h"

#include "sdsdkv-client.h"
#include "sdsdkv-server.h"

class personality_factory {
    //
    personality_factory(void) = delete;
    //
    ~personality_factory(void) = delete;
public:
    static personality *
    create(sdsdkv_config_personality p)
    {
        switch(p) {
            case SDSDKV_PERSONALITY_CLIENT:
                return new sdsdkv_client();
            case SDSDKV_PERSONALITY_SERVER:
                return new sdsdkv_server();
            default:
                return nullptr;
        }
    }
};

/*
 * vim: ft=cpp ts=4 sts=4 sw=4 expandtab
 */
