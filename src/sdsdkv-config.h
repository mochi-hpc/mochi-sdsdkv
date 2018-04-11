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

static bool
config_valid(
    const sdsdkv_config &config
) {
    switch (config.personality) {
        case SDSDKV_PERSONALITY_CLIENT:
        case SDSDKV_PERSONALITY_SERVER:
            break;
        default:
            return false;
    }

    switch (config.hash_be) {
        case SDSDKV_HASHING_CH_PLACEMENT:
            break;
        default:
            return false;
    }

    return true;
}

/*
 * vim: ft=cpp ts=4 sts=4 sw=4 expandtab
 */
