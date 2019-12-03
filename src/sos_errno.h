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

#ifndef SOS_ERRNO_H
#define SOS_ERRNO_H

#include <errno.h>

#ifdef __cplusplus
extern "C" {
#endif


#define SOS_SUCCESS 0

/* SOS specific return values: >= 512 */
#define SOS_ERRNO_OFFSET 512

enum {
    SOS_EOTHER            = SOS_ERRNO_OFFSET, /* Unspecified error */
    SOS_ESHMALLOC_FAIL    = 513,
    SOS_ERESOURCE_EXHAUST = 514,
    SOS_EINVALID_TEAM     = 515,
    SOS_EINVALID_ARG      = 516,
    SOS_ERRNO_MAX
};


static const char *const errstr[] = {
    [SOS_EOTHER - SOS_ERRNO_OFFSET] = "Unspecified error",
    [SOS_ESHMALLOC_FAIL - SOS_ERRNO_OFFSET] = "Internal symmetric heap allocation failed",
    [SOS_ERESOURCE_EXHAUST - SOS_ERRNO_OFFSET] = "Exhausted a critical resource",
    [SOS_EINVALID_TEAM - SOS_ERRNO_OFFSET]  = "Encountered an unexpected invalid team",
    [SOS_EINVALID_ARG - SOS_ERRNO_OFFSET] = "Encountered an invalid argument",
};

static inline
const char *sos_strerror(int errnum)
{
    if (errnum < SOS_ERRNO_OFFSET)
        return strerror(errnum);
    else if (errnum < SOS_ERRNO_MAX)
        return errstr[errnum - SOS_ERRNO_OFFSET];
    else
        return errstr[SOS_EOTHER - SOS_ERRNO_OFFSET];
}

#ifdef __cplusplus
}
#endif

#endif /* SOS_ERRNO_H */

