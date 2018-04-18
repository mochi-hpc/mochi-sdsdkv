/*
 * Copyright (c) 2018      Los Alamos National Security, LLC
 *                         All rights reserved.
 *
 * This file is part of the sdsdkv project. See the LICENSE file at the
 * top-level directory of this distribution.
 */

/**
 * @file sdsdkv-personality.h
 */

#pragma once

#include "sdsdkv.h"

#include "margo.h"

struct personality {
protected:
    //
    margo_instance_id m_mid = MARGO_INSTANCE_NULL;
public:
    //
    personality(void) = default;
    //
    virtual
    ~personality(void) = default;
    //
    virtual int
    open(void) = 0;
    //
    virtual int
    xchange_addrs(void) = 0;
};

/*
 * vim: ft=cpp ts=4 sts=4 sw=4 expandtab
 */
