/* -*- C -*-
 *
 * Copyright (c) 2020 Intel Corporation. All rights reserved.
 * This software is available to you under the BSD license.
 *
 * This file is part of the Sandia OpenSHMEM software package. For license
 * information, see the LICENSE file in the top level directory of the
 * distribution.
 *
 */

#ifndef SHMEM_ACCELERATOR_H
#define SHMEM_ACCELERATOR_H

#include "shmem_internal.h"

#ifdef USE_ZE
#include <level_zero/ze_api.h>

extern ze_driver_handle_t shmem_internal_gpu_driver;
extern ze_driver_handle_t shmem_internal_fpga_driver;
extern ze_device_handle_t shmem_internal_gpu_device;
extern ze_device_handle_t shmem_internal_fpga_device;

extern ze_context_handle_t shmem_internal_ze_context;
extern ze_context_desc_t shmem_internal_ze_context_desc;
extern ze_command_queue_handle_t shmem_internal_cmd_queue;
extern ze_command_list_handle_t shmem_internal_cmd_list;

#ifdef USE_XPU_IPC
extern ze_ipc_mem_handle_t shmem_internal_mem_ipc_handle;
#endif

#define ZE_CHECK(call)                            \
  do {                                            \
    ze_result_t status = call;                    \
    if (status != ZE_RESULT_SUCCESS) {            \
      RAISE_ERROR_MSG("ZE FAIL: call = '%s' result = '%d'\n", #call, status);   \
    }                                             \
  } while (0)                                    
      

static inline 
void print_device_properties(ze_device_properties_t props) {
  DEBUG_MSG("Device: \n"
            RAISE_PREFIX
            "name : %s, type : %s\n"
            RAISE_PREFIX
            "vendorId : %d, deviceId : %d\n"
            RAISE_PREFIX
            "subdeviceId : %d, coreClockRate : %d\n",
            props.name, ((props.type == ZE_DEVICE_TYPE_GPU) ? "GPU" : "FPGA"),
            props.vendorId, props.deviceId, props.subdeviceId, props.coreClockRate);
}

#endif /* USE_ZE */

#ifdef USE_CUDA
#include <cuda_runtime.h>
#include <cuda.h>

#define CU_CHECK(call)                                                  \
    do {                                                                \
        cudaError_t status = call;                                      \
        if (status != cudaSuccess) {                                    \
            printf("CUDA FAIL: call='%s'. Reason:%s\n", #call,          \
                   cudaGetErrorString(status));                         \
            shmem_runtime_abort(1, PACKAGE_NAME " exited in error");    \
        }                                                               \
    } while (0)

#endif /* USE_CUDA */

/* Accelerator routines */

/* Initialize accelerator */
int shmem_internal_accelerator_init(int my_pe);

/* Finalize accelerator */
int shmem_internal_accelerator_fini(void);

#ifdef USE_XPU_IPC
/* Initialize IPC buffer */
int shmem_internal_xpu_ipc_init(void);
#endif

#endif /* SHMEM_ACCELERATOR_H */
