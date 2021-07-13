/* -*- C -*-
 *
 * Copyright 2011 Sandia Corporation. Under the terms of Contract
 * DE-AC04-94AL85000 with Sandia Corporation, the U.S.  Government
 * retains certain rights in this software.
 *
 * Copyright (c) 2017 Intel Corporation. All rights reserved.
 * This software is available to you under the BSD license.
 *
 * This file is part of the Sandia OpenSHMEM software package. For license
 * information, see the LICENSE file in the top level directory of the
 * distribution.
 *
 */

#include "config.h"

#define SHMEM_INTERNAL_INCLUDE
#include "shmem.h"
#include "shmemx.h"
#include "shmem_internal.h"
#include "shmem_accelerator.h"

#ifdef ENABLE_PROFILING
#include "pshmem.h"

#pragma weak shmemx_xpu_init = pshmemx_xpu_init
#define shmemx_xpu_init pshmemx_xpu_init

#pragma weak shmemx_xpu_finalize = pshmemx_xpu_finalize
#define shmemx_xpu_finalize pshmemx_xpu_finalize

#endif /* ENABLE_PROFILING */


int SHMEM_FUNCTION_ATTRIBUTES
shmemx_xpu_init(void **driver, void **device, 
                void **cmd_queue, void **cmd_list)
{
    SHMEM_ERR_CHECK_INITIALIZED();

    int ret = 0;
    if (!shmem_internal_initialized) {
        ret = shmem_internal_accelerator_init(shmem_internal_my_pe);
    }

#ifdef USE_ZE
    *driver = (void *) &shmem_internal_gpu_driver;
    *device = (void *) &shmem_internal_gpu_device;
    *cmd_queue = (void *) &shmem_internal_cmd_queue;
    *cmd_list = (void *) &shmem_internal_cmd_list;
#endif

    return ret;
}


int SHMEM_FUNCTION_ATTRIBUTES
shmemx_xpu_finalize()
{
    SHMEM_ERR_CHECK_INITIALIZED();

    int ret = 0;
    if (shmem_internal_initialized && 
        !shmem_internal_finalized) {
        ret = shmem_internal_accelerator_fini();
    }
    return ret;
}
