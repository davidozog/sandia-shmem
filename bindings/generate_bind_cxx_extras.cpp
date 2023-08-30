/* -*- C -*-
 *
 * Copyright (c) 2017 Intel Corporation. All rights reserved.
 * This software is available to you under the BSD license.
 *
 * This file is part of the Sandia OpenSHMEM software package. For license
 * information, see the LICENSE file in the top level directory of the
 * distribution.
 *
 */

#include <type_traits>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "config.h"

#if OPAL_HAVE_ATTRIBUTE_UNUSED
#define VAR_IS_UNUSED __attribute__((unused))
#else
#define VAR_IS_UNUSED
#endif

#define COMPARE_TYPES_FIRST(A,B) if (std::is_same<A, B>::value) need_assoc = false;
#define COMPARE_TYPES(A,B)       else if (std::is_same<A, B>::value) need_assoc = false;

/* Note: Mirrors SHMEM_BIND_CXX_RMA */
#define NEED_RMA_ASSOC(CTYPE)                       \
           COMPARE_TYPES_FIRST(char, CTYPE)         \
           COMPARE_TYPES(signed char, CTYPE)        \
           COMPARE_TYPES(short, CTYPE)              \
           COMPARE_TYPES(int, CTYPE)                \
           COMPARE_TYPES(long, CTYPE)               \
           COMPARE_TYPES(long long, CTYPE)          \
           COMPARE_TYPES(unsigned char, CTYPE)      \
           COMPARE_TYPES(unsigned short, CTYPE)     \
           COMPARE_TYPES(unsigned int, CTYPE)       \
           COMPARE_TYPES(unsigned long, CTYPE)      \
           COMPARE_TYPES(unsigned long long, CTYPE) \
           COMPARE_TYPES(float, CTYPE)              \
           COMPARE_TYPES(double, CTYPE)             \
           COMPARE_TYPES(long double, CTYPE)

/* Note: Mirrors SHMEM_BIND_CXX_AMO */
#define NEED_AMO_ASSOC(CTYPE)                       \
           COMPARE_TYPES_FIRST(int, CTYPE)          \
           COMPARE_TYPES(long, CTYPE)               \
           COMPARE_TYPES(long long, CTYPE)          \
           COMPARE_TYPES(unsigned int, CTYPE)       \
           COMPARE_TYPES(unsigned long, CTYPE)      \
           COMPARE_TYPES(unsigned long long, CTYPE)

/* Note: Mirrors SHMEM_BIND_CXX_EXTENDED_AMO */
#define NEED_EXTENDED_AMO_ASSOC(CTYPE)              \
           COMPARE_TYPES_FIRST(int, CTYPE)          \
           COMPARE_TYPES(long, CTYPE)               \
           COMPARE_TYPES(long long, CTYPE)          \
           COMPARE_TYPES(unsigned int, CTYPE)       \
           COMPARE_TYPES(unsigned long, CTYPE)      \
           COMPARE_TYPES(unsigned long long, CTYPE) \
           COMPARE_TYPES(float, CTYPE)              \
           COMPARE_TYPES(double, CTYPE)

/* Note: Mirrors SHMEM_BIND_CXX_BITWISE_AMO */
#define NEED_BITWISE_AMO_ASSOC(CTYPE)               \
           COMPARE_TYPES_FIRST(unsigned int, CTYPE) \
           COMPARE_TYPES(unsigned long, CTYPE)      \
           COMPARE_TYPES(unsigned long long, CTYPE)

/* Note: Mirrors SHMEM_BIND_CXX_SYNC */
#define NEED_SYNC_ASSOC(CTYPE)                      \
           COMPARE_TYPES_FIRST(int, CTYPE)          \
           COMPARE_TYPES(long, CTYPE)               \
           COMPARE_TYPES(long long, CTYPE)          \
           COMPARE_TYPES(unsigned int, CTYPE)       \
           COMPARE_TYPES(unsigned long, CTYPE)      \
           COMPARE_TYPES(unsigned long long, CTYPE)

/* Note: Mirrors SHMEM_BIND_CXX_COLL_AND_OR_XOR */
#define NEED_COLL_AND_OR_XOR_ASSOC(CTYPE)           \
           COMPARE_TYPES_FIRST(unsigned char, CTYPE)\
           COMPARE_TYPES(unsigned short, CTYPE)     \
           COMPARE_TYPES(unsigned int, CTYPE)       \
           COMPARE_TYPES(unsigned long, CTYPE)      \
           COMPARE_TYPES(unsigned long long, CTYPE)

/* Note: Mirrors SHMEM_BIND_CXX_COLL_MIN_MAX */
#define NEED_COLL_MIN_MAX_ASSOC(CTYPE)              \
           COMPARE_TYPES_FIRST(char, CTYPE)         \
           COMPARE_TYPES(signed char, CTYPE)        \
           COMPARE_TYPES(short, CTYPE)              \
           COMPARE_TYPES(int, CTYPE)                \
           COMPARE_TYPES(long, CTYPE)               \
           COMPARE_TYPES(long long, CTYPE)          \
           COMPARE_TYPES(unsigned char, CTYPE)      \
           COMPARE_TYPES(unsigned short, CTYPE)     \
           COMPARE_TYPES(unsigned int, CTYPE)       \
           COMPARE_TYPES(unsigned long, CTYPE)      \
           COMPARE_TYPES(unsigned long long, CTYPE) \
           COMPARE_TYPES(float, CTYPE)              \
           COMPARE_TYPES(double, CTYPE)             \
           COMPARE_TYPES(long double, CTYPE)

/* Note: Mirrors SHMEM_BIND_CXX_COLL_SUM_PROD */
#define NEED_COLL_SUM_PROD_ASSOC(CTYPE)             \
           COMPARE_TYPES_FIRST(char, CTYPE)         \
           COMPARE_TYPES(signed char, CTYPE)        \
           COMPARE_TYPES(short, CTYPE)              \
           COMPARE_TYPES(int, CTYPE)                \
           COMPARE_TYPES(long, CTYPE)               \
           COMPARE_TYPES(long long, CTYPE)          \
           COMPARE_TYPES(unsigned char, CTYPE)      \
           COMPARE_TYPES(unsigned short, CTYPE)     \
           COMPARE_TYPES(unsigned int, CTYPE)       \
           COMPARE_TYPES(unsigned long, CTYPE)      \
           COMPARE_TYPES(unsigned long long, CTYPE) \
           COMPARE_TYPES(float, CTYPE)              \
           COMPARE_TYPES(double, CTYPE)             \
           COMPARE_TYPES(long double, CTYPE)

#define GEN_ASSOC(TYPENAME, CTYPE, TYPE_CLASS)                  \
  do {                                                          \
    bool need_assoc = true;                                     \
    NEED_##TYPE_CLASS##_ASSOC(CTYPE)                            \
    if (need_assoc)                                             \
      printf("$1(%s, %s)$2\n", #TYPENAME, #CTYPE);              \
  }                                                             \
  while (0)

#define GEN_AMO_ASSOC(TYPENAME, CTYPE, SHMTYPE, TYPE_CLASS)     \
  do {                                                          \
    bool need_assoc = true;                                     \
    NEED_##TYPE_CLASS##_ASSOC(CTYPE)                            \
    if (need_assoc)                                             \
      printf("$1(%s, %s, %s)$2\n", #TYPENAME, #CTYPE, #SHMTYPE);\
  }                                                             \
  while (0)

int main(int argc, char **argv)
{
    char timestr[100] = "unknown time";
    time_t curtime = time(NULL);
    struct tm *t = localtime(&curtime);

    if (t != NULL) {
        strftime(timestr, sizeof(timestr), "%m-%d-%Y %H:%M", t);
    } else {
        strcpy(timestr, "unknown time");
    }

    printf("dnl Generated by %s, %s\n", argv[0], timestr);
    printf("define(`SHMEM_BIND_CXX_RMA_EXTRAS',\n`");
    GEN_ASSOC(int8,       int8_t, RMA);
    GEN_ASSOC(int16,     int16_t, RMA);
    GEN_ASSOC(int32,     int32_t, RMA);
    GEN_ASSOC(int64,     int64_t, RMA);
    GEN_ASSOC(uint8,     uint8_t, RMA);
    GEN_ASSOC(uint16,   uint16_t, RMA);
    GEN_ASSOC(uint32,   uint32_t, RMA);
    GEN_ASSOC(uint64,   uint64_t, RMA);
    GEN_ASSOC(size,       size_t, RMA);
    GEN_ASSOC(ptrdiff, ptrdiff_t, RMA);
    printf("')dnl\n");

    printf("define(`SHMEM_BIND_CXX_AMO_EXTRAS',\n`");
    GEN_AMO_ASSOC(int32,     int32_t,     SHM_INTERNAL_INT32, AMO);
    GEN_AMO_ASSOC(int64,     int64_t,     SHM_INTERNAL_INT64, AMO);
    GEN_AMO_ASSOC(uint32,   uint32_t,    SHM_INTERNAL_UINT32, AMO);
    GEN_AMO_ASSOC(uint64,   uint64_t,    SHM_INTERNAL_UINT64, AMO);
    GEN_AMO_ASSOC(size,       size_t,    SHM_INTERNAL_SIZE_T, AMO);
    GEN_AMO_ASSOC(ptrdiff, ptrdiff_t, SHM_INTERNAL_PTRDIFF_T, AMO);
    printf("')dnl\n");

    printf("define(`SHMEM_BIND_CXX_EXTENDED_AMO_EXTRAS',\n`");
    GEN_AMO_ASSOC(int32,     int32_t,     SHM_INTERNAL_INT32, EXTENDED_AMO);
    GEN_AMO_ASSOC(int64,     int64_t,     SHM_INTERNAL_INT64, EXTENDED_AMO);
    GEN_AMO_ASSOC(uint32,   uint32_t,    SHM_INTERNAL_UINT32, EXTENDED_AMO);
    GEN_AMO_ASSOC(uint64,   uint64_t,    SHM_INTERNAL_UINT64, EXTENDED_AMO);
    GEN_AMO_ASSOC(size,       size_t,    SHM_INTERNAL_SIZE_T, EXTENDED_AMO);
    GEN_AMO_ASSOC(ptrdiff, ptrdiff_t, SHM_INTERNAL_PTRDIFF_T, EXTENDED_AMO);
    printf("')dnl\n");

    printf("define(`SHMEM_BIND_CXX_BITWISE_AMO_EXTRAS',\n`");
    GEN_AMO_ASSOC(int32,     int32_t,     SHM_INTERNAL_INT32, BITWISE_AMO);
    GEN_AMO_ASSOC(int64,     int64_t,     SHM_INTERNAL_INT64, BITWISE_AMO);
    GEN_AMO_ASSOC(uint32,   uint32_t,    SHM_INTERNAL_UINT32, BITWISE_AMO);
    GEN_AMO_ASSOC(uint64,   uint64_t,    SHM_INTERNAL_UINT64, BITWISE_AMO);
    printf("')dnl\n");

    printf("define(`SHMEM_BIND_CXX_SYNC_EXTRAS',\n`");
    GEN_ASSOC(int32,     int32_t, SYNC);
    GEN_ASSOC(int64,     int64_t, SYNC);
    GEN_ASSOC(uint32,   uint32_t, SYNC);
    GEN_ASSOC(uint64,   uint64_t, SYNC);
    GEN_ASSOC(size,       size_t, SYNC);
    GEN_ASSOC(ptrdiff, ptrdiff_t, SYNC);
    printf("')dnl\n");

    printf("define(`SHMEM_BIND_CXX_COLL_AND_OR_XOR_EXTRAS',\n`");
    GEN_AMO_ASSOC(int8,       int8_t,   SHM_INTERNAL_INT8,   COLL_AND_OR_XOR);
    GEN_AMO_ASSOC(int16,     int16_t,  SHM_INTERNAL_INT16,   COLL_AND_OR_XOR);
    GEN_AMO_ASSOC(int32,     int32_t,  SHM_INTERNAL_INT32,   COLL_AND_OR_XOR);
    GEN_AMO_ASSOC(int64,     int64_t,  SHM_INTERNAL_INT64,   COLL_AND_OR_XOR);
    GEN_AMO_ASSOC(uint8,     uint8_t,  SHM_INTERNAL_UINT8,   COLL_AND_OR_XOR);
    GEN_AMO_ASSOC(uint16,   uint16_t, SHM_INTERNAL_UINT16,   COLL_AND_OR_XOR);
    GEN_AMO_ASSOC(uint32,   uint32_t, SHM_INTERNAL_UINT32,   COLL_AND_OR_XOR);
    GEN_AMO_ASSOC(uint64,   uint64_t, SHM_INTERNAL_UINT64,   COLL_AND_OR_XOR);
    GEN_AMO_ASSOC(size,       size_t, SHM_INTERNAL_SIZE_T,   COLL_AND_OR_XOR);
    printf("')dnl\n");

    printf("define(`SHMEM_BIND_CXX_COLL_MIN_MAX_EXTRAS',\n`");
    GEN_AMO_ASSOC(ptrdiff, ptrdiff_t, SHM_INTERNAL_PTRDIFF_T,   COLL_MIN_MAX);
    GEN_AMO_ASSOC(int8,       int8_t,      SHM_INTERNAL_INT8,   COLL_MIN_MAX);
    GEN_AMO_ASSOC(int16,     int16_t,     SHM_INTERNAL_INT16,   COLL_MIN_MAX);
    GEN_AMO_ASSOC(int32,     int32_t,     SHM_INTERNAL_INT32,   COLL_MIN_MAX);
    GEN_AMO_ASSOC(int64,     int64_t,     SHM_INTERNAL_INT64,   COLL_MIN_MAX);
    GEN_AMO_ASSOC(uint8,     uint8_t,     SHM_INTERNAL_UINT8,   COLL_MIN_MAX);
    GEN_AMO_ASSOC(uint16,   uint16_t,    SHM_INTERNAL_UINT16,   COLL_MIN_MAX);
    GEN_AMO_ASSOC(uint32,   uint32_t,    SHM_INTERNAL_UINT32,   COLL_MIN_MAX);
    GEN_AMO_ASSOC(uint64,   uint64_t,    SHM_INTERNAL_UINT64,   COLL_MIN_MAX);
    GEN_AMO_ASSOC(size,       size_t,    SHM_INTERNAL_SIZE_T,   COLL_MIN_MAX);
    printf("')dnl\n");

    printf("define(`SHMEM_BIND_CXX_COLL_SUM_PROD_EXTRAS',\n`");
    GEN_AMO_ASSOC(ptrdiff, ptrdiff_t, SHM_INTERNAL_PTRDIFF_T,   COLL_SUM_PROD);
    GEN_AMO_ASSOC(int8,       int8_t,      SHM_INTERNAL_INT8,   COLL_SUM_PROD);
    GEN_AMO_ASSOC(int16,     int16_t,     SHM_INTERNAL_INT16,   COLL_SUM_PROD);
    GEN_AMO_ASSOC(int32,     int32_t,     SHM_INTERNAL_INT32,   COLL_SUM_PROD);
    GEN_AMO_ASSOC(int64,     int64_t,     SHM_INTERNAL_INT64,   COLL_SUM_PROD);
    GEN_AMO_ASSOC(uint8,     uint8_t,     SHM_INTERNAL_UINT8,   COLL_SUM_PROD);
    GEN_AMO_ASSOC(uint16,   uint16_t,    SHM_INTERNAL_UINT16,   COLL_SUM_PROD);
    GEN_AMO_ASSOC(uint32,   uint32_t,    SHM_INTERNAL_UINT32,   COLL_SUM_PROD);
    GEN_AMO_ASSOC(uint64,   uint64_t,    SHM_INTERNAL_UINT64,   COLL_SUM_PROD);
    GEN_AMO_ASSOC(size,       size_t,    SHM_INTERNAL_SIZE_T,   COLL_SUM_PROD);
    printf("')dnl\n");

    return 0;
}
