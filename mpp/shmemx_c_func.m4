dnl Copyright (c) 2016 Intel Corporation. All rights reserved.
dnl This software is available to you under the BSD license.
dnl
dnl This file is part of the Sandia OpenSHMEM software package. For license
dnl information, see the LICENSE file in the top level directory of the
dnl distribution.
dnl

#ifndef SHMEM_FUNCTION_ATTRIBUTES
#  if OPAL_C_HAVE_VISIBILITY == 1
#     define SHMEM_FUNCTION_ATTRIBUTES __attribute__((visibility("default")))
#  else
#     define SHMEM_FUNCTION_ATTRIBUTES
#  endif
#endif

SHMEM_FUNCTION_ATTRIBUTES void SHPRE()shmemx_init_thread(int tl_requested, int *tl_provided);
SHMEM_FUNCTION_ATTRIBUTES double SHPRE()shmemx_wtime(void);
SHMEM_FUNCTION_ATTRIBUTES char* SHPRE()shmemx_nodename(void);

SHMEM_FUNCTION_ATTRIBUTES void SHPRE()shmemx_getmem_ct(shmemx_ct_t ct, void *target, const void *source, size_t len, int pe);
SHMEM_FUNCTION_ATTRIBUTES void SHPRE()shmemx_putmem_ct(shmemx_ct_t ct, void *target, const void *source, size_t len, int pe);
SHMEM_FUNCTION_ATTRIBUTES void SHPRE()shmemx_ct_create(shmemx_ct_t *ct);
SHMEM_FUNCTION_ATTRIBUTES void SHPRE()shmemx_ct_free(shmemx_ct_t *ct);
SHMEM_FUNCTION_ATTRIBUTES long SHPRE()shmemx_ct_get(shmemx_ct_t ct);
SHMEM_FUNCTION_ATTRIBUTES void SHPRE()shmemx_ct_set(shmemx_ct_t ct, long value);
SHMEM_FUNCTION_ATTRIBUTES void SHPRE()shmemx_ct_wait(shmemx_ct_t ct, long wait_for);

SHMEM_FUNCTION_ATTRIBUTES void SHPRE()shmemx_pcontrol(int level, ...);
