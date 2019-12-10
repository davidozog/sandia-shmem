/* -*- C -*-
 *
 * Copyright (c) 2019 Intel Corporation. All rights reserved.
 * This software is available to you under the BSD license.
 *
 * This file is part of the Sandia OpenSHMEM software package. For license
 * information, see the LICENSE file in the top level directory of the
 * distribution.
 *
 */

#ifndef SHMEM_ERRNO_H
#define SHMEM_ERRNO_H

#ifdef __cplusplus
extern "C" {
#endif


#define SHMEM_INTERNAL_SUCCESS 0

/* SOS specific return values: <= -256 */
#define SHMEM_ERRNO_OFFSET -256

#define SHMEM_ERRNO_INDEX(x) (-((x) - SHMEM_ERRNO_OFFSET))

enum {
    SHMEM_INTERNAL_EOTHER            = SHMEM_ERRNO_OFFSET, /* Unspecified error */
    SHMEM_INTERNAL_ESHMALLOC_FAIL    = -257,
    SHMEM_INTERNAL_ERESOURCE_EXHAUST = -258,
    SHMEM_INTERNAL_EINVALID_TEAM     = -259,
    SHMEM_INTERNAL_EINVALID_ARG      = -260,
    SHMEM_ERRNO_MIN                  = -261
};


static const char *const errstr[] = {
    [SHMEM_ERRNO_INDEX(SHMEM_INTERNAL_EOTHER)]            = "Unspecified error",
    [SHMEM_ERRNO_INDEX(SHMEM_INTERNAL_ESHMALLOC_FAIL)]    = "Internal symmetric heap allocation failed",
    [SHMEM_ERRNO_INDEX(SHMEM_INTERNAL_ERESOURCE_EXHAUST)] = "Exhausted a critical resource",
    [SHMEM_ERRNO_INDEX(SHMEM_INTERNAL_EINVALID_TEAM)]     = "Encountered an unexpected invalid team",
    [SHMEM_ERRNO_INDEX(SHMEM_INTERNAL_EINVALID_ARG)]      = "Encountered an invalid argument",
};

static inline
const char *shmem_internal_strerror(int errnum)
{
    if (errnum <= SHMEM_ERRNO_OFFSET && errnum > SHMEM_ERRNO_MIN)
        return errstr[SHMEM_ERRNO_INDEX(errnum)];
    else
        return errstr[SHMEM_ERRNO_INDEX(SHMEM_INTERNAL_EOTHER)];
}

#ifdef __cplusplus
}
#endif

#endif /* SHMEM_ERRNO_H */

