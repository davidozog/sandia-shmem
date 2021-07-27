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


#include "config.h"
#include "shmem_accelerator.h"

#if defined(USE_ZE)
ze_driver_handle_t *all_drivers;
ze_device_handle_t **all_devices;
uint32_t driver_count = 0;

ze_driver_handle_t shmem_internal_gpu_driver = NULL; 
ze_driver_handle_t shmem_internal_fpga_driver = NULL;
ze_device_handle_t shmem_internal_gpu_device = NULL; 
ze_device_handle_t shmem_internal_fpga_device = NULL;

ze_context_handle_t shmem_internal_ze_context;
ze_context_desc_t shmem_internal_ze_context_desc = {};
ze_command_queue_handle_t shmem_internal_cmd_queue = NULL;
ze_command_list_handle_t shmem_internal_cmd_list = NULL;

#ifdef USE_XPU_IPC
ze_ipc_mem_handle_t shmem_internal_mem_ipc_handle = NULL;
#endif

int shmem_internal_accelerator_init(int my_pe) {
  uint32_t device_count = 0;
  uint32_t i, j;
  int gpu_device_found = 0;
  int fpga_device_found = 0;

  ZE_CHECK(zeInit(ZE_INIT_FLAG_GPU_ONLY));

  ZE_CHECK(zeDriverGet(&driver_count, NULL));
  if (driver_count == 0) {
    RAISE_ERROR_MSG("No ZE driver found\n");
  }

  all_drivers = (ze_driver_handle_t *) malloc(driver_count * sizeof(ze_driver_handle_t));
  all_devices = (ze_device_handle_t **) malloc(driver_count * sizeof(ze_device_handle_t *));

  ZE_CHECK(zeDriverGet(&driver_count, all_drivers));

  for (i = 0; i < driver_count; i++) {
    device_count = 0;

    ZE_CHECK(zeDeviceGet(all_drivers[i], &device_count, NULL));
    if (device_count == 0) continue;

    all_devices[i] = (ze_device_handle_t *) malloc(device_count * sizeof(ze_device_handle_t));

    ZE_CHECK(zeDeviceGet(all_drivers[i], &device_count, all_devices[i]));

    for (j = 0; j < device_count; j++) {
      ze_device_properties_t device_properties;
      ZE_CHECK(zeDeviceGetProperties(all_devices[i][j], &device_properties));

      /* Storing gpu and fpga devices only for now */
      if (ZE_DEVICE_TYPE_GPU == device_properties.type && !gpu_device_found) {
        shmem_internal_gpu_driver = all_drivers[i];
        shmem_internal_gpu_device = all_devices[i][my_pe % device_count];
        gpu_device_found = 1;
      } else if (ZE_DEVICE_TYPE_FPGA == device_properties.type && !fpga_device_found) {
        shmem_internal_fpga_driver = all_drivers[i];
        shmem_internal_fpga_device = all_devices[i][my_pe % device_count];
        fpga_device_found = 1;
      }
    }
  }

  if (!gpu_device_found && !fpga_device_found) {
    RAISE_ERROR_MSG("No ZE device found\n");
  }

  if (shmem_internal_params.DEBUG) {
    ze_device_properties_t device_properties;

    if (gpu_device_found) {
      ZE_CHECK(zeDeviceGetProperties(shmem_internal_gpu_device, &device_properties));
      print_device_properties(device_properties);
    } 

    if (fpga_device_found) {
      ZE_CHECK(zeDeviceGetProperties(shmem_internal_fpga_device, &device_properties));
      print_device_properties(device_properties);
    }
  }

  /* Create the ZE context */
  shmem_internal_ze_context_desc.stype = ZE_STRUCTURE_TYPE_CONTEXT_DESC;

  ZE_CHECK(zeContextCreate(shmem_internal_gpu_driver,
                           &shmem_internal_ze_context_desc,
                           &shmem_internal_ze_context) );

  /* Create the ZE command queue */
  ze_command_queue_desc_t cmdq_desc = {
    .stype          = ZE_STRUCTURE_TYPE_COMMAND_QUEUE_DESC,
    .pNext          = NULL,
    .ordinal        = 0,
    .index          = 0,
    .flags          = 0,
    .mode           = ZE_COMMAND_QUEUE_MODE_SYNCHRONOUS,
    .priority       = ZE_COMMAND_QUEUE_PRIORITY_NORMAL,
  };
  ZE_CHECK(zeCommandQueueCreate(shmem_internal_ze_context, shmem_internal_gpu_device, &cmdq_desc,
                                &shmem_internal_cmd_queue));

  ze_command_list_desc_t cmd_list_desc = {
    .stype                          = ZE_STRUCTURE_TYPE_COMMAND_LIST_DESC,
    .pNext                          = NULL,
    .commandQueueGroupOrdinal       = 0,
    .flags                          = 0,
  };
  ZE_CHECK(zeCommandListCreate(shmem_internal_ze_context, shmem_internal_gpu_device, &cmd_list_desc,
                               &shmem_internal_cmd_list));

  return 0;    
}

int shmem_internal_accelerator_fini(void) {
  int i;

  /* Destroy of command queue and list */
  ZE_CHECK(zeCommandListDestroy(shmem_internal_cmd_list));
  ZE_CHECK(zeCommandQueueDestroy(shmem_internal_cmd_queue));

  for (i = 0; i < driver_count; i++)
    free(all_devices[i]);
  free(all_devices);
  free(all_drivers);

  return 0;
}

#ifdef USE_XPU_IPC
int shmem_internal_xpu_ipc_init(void) {
  int value = 3;
  ZE_CHECK(zeCommandListAppendMemoryFill(shmem_internal_cmd_list, shmem_internal_heap_base, 
                                         (void *) &value, sizeof(value), shmem_internal_heap_length, 
                                         NULL);
  ZE_CHECK(zeCommandListClose(shmem_internal_cmd_list));
  ZE_CHECK(zeCommandQueueExecuteCommandLists(shmem_internal_cmd_queue, 1, &shmem_internal_cmd_list, NULL));
  ZE_CHECK(zeCommandQueueSynchronize(shmem_internal_cmd_queue, INT_MAX));
  ZE_CHECK(zeCommandListReset(shmem_internal_cmd_list));

  // TODO: For now doing it only for gpu driver; handle other drivers later
  ZE_CHECK(zeDriverGetMemIpcHandle(shmem_internal_gpu_driver, shmem_internal_heap_base, 
                                   &shmem_internal_mem_ipc_handle));  
}
#endif

#elif defined(USE_CUDA)

int shmem_internal_accelerator_init(void) {
  return 0;
}

int shmem_internal_accelerator_fini(void) {
  return 0;
}

#ifdef USE_XPU_IPC
int shmem_internal_xpu_ipc_init(void) {

}
#endif

#else

int shmem_internal_accelerator_init(int my_pe) {
  RAISE_WARN_MSG("No accelerator library is found\n");
  return 0;
}

int shmem_internal_accelerator_fini(void) {
  RAISE_WARN_MSG("No accelerator library is found\n");
  return 0;
}

#ifdef USE_XPU_IPC
int shmem_internal_xpu_ipc_init(void) {
  RAISE_WARN_MSG("No accelerator library is found\n");
  return 0;
}
#endif
#endif
