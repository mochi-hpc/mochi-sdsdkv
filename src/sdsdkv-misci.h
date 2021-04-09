/*
 * Copyright (c) 2018      Los Alamos National Security, LLC
 *                         All rights reserved.
 *
 * This file is part of the sdsdkv project. See the LICENSE file at the
 * top-level directory of this distribution.
 */

/**
 * @file sdsdkv-misci.h
 */

#pragma once

#include "sdsdkv.h"

#include "sdskv-common.h"

/**
 * Takes an sdskv return code and returns an appropriate sdsdkv code.
 */
static inline int
sdskv2irc(int rc)
{
    switch(rc) {
        case (SDSKV_SUCCESS):         return SDSDKV_SUCCESS;
        case (SDSKV_ERR_ALLOCATION):  return SDSDKV_ERR_OOR;
        case (SDSKV_ERR_INVALID_ARG): return SDSDKV_ERR_INVLD_ARG;
//      case (SDSKV_ERR_MERCURY):     return SDSDKV_ERR_SERVICE;
        case (SDSKV_ERR_DB_CREATE):   return SDSDKV_ERR_DB_CREATE;
        case (SDSKV_ERR_DB_NAME):     return SDSDKV_ERR_DB_NAME;
        case (SDSKV_ERR_UNKNOWN_DB):  return SDSDKV_ERR_DB_UNKNOWN;
        case (SDSKV_ERR_UNKNOWN_PR):  return SDSDKV_ERR_PR_UNKNOWN;
        case (SDSKV_ERR_PUT):         return SDSDKV_ERR_PUT;
        case (SDSKV_ERR_UNKNOWN_KEY): return SDSDKV_ERR_UNKNOWN_KEY;
        case (SDSKV_ERR_SIZE):        return SDSDKV_ERR_SIZE;
        case (SDSKV_ERR_ERASE):       return SDSDKV_ERR_ERASE;

        case (SDSKV_ERR_PR_EXISTS):   return SDSDKV_ERR;
        case (SDSKV_ERR_MIGRATION):   return SDSDKV_ERR;
        case (SDSKV_OP_NOT_IMPL):     return SDSDKV_ERR;
        case (SDSKV_ERR_COMP_FUNC):   return SDSDKV_ERR;
        case (SDSKV_ERR_REMI):        return SDSDKV_ERR;
        case (SDSKV_ERR_MAX):         return SDSDKV_ERR;
    }
    return SDSDKV_ERR_INVLD_ARG;
}

/*
 * vim: ft=cpp ts=4 sts=4 sw=4 expandtab
 */
