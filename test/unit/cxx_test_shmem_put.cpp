/*
 *  This test program is derived from a unit test created by Nick Park.
 *  The original unit test is a work of the U.S. Government and is not subject
 *  to copyright protection in the United States.  Foreign copyrights may
 *  apply.
 *
 *  Copyright (c) 2017 Intel Corporation. All rights reserved.
 *  This software is available to you under the BSD license below:
 *
 *      Redistribution and use in source and binary forms, with or
 *      without modification, are permitted provided that the following
 *      conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <shmem.h>

enum op { PUT = 0, IPUT, PUT_NBI };

#define TEST_SHMEM_PUT(OP, USE_CTX, USE_TEMPLATE, TYPE, TYPENAME)             \
  do {                                                          \
    static TYPE remote[10];                                     \
    const int mype = shmem_my_pe();                             \
    const int npes = shmem_n_pes();                             \
    TYPE local[10];                                             \
    for (int i = 0; i < 10; i++)                                \
      local[i] = (TYPE)mype;                                    \
    switch (OP) {                                               \
      case PUT:                                                 \
        if (USE_CTX)                                            \
          shmem_ctx_##TYPENAME##_put(SHMEM_CTX_DEFAULT, remote, \
                                 local, 10, (mype + 1) % npes); \
        else if (USE_TEMPLATE) {                                  \
            printf("using a template func...\n"); \
          shmem_put_template<TYPE>(remote, local, 10,                          \
                                       (mype + 1) % npes);      \
        } \
        else                                                    \
          shmem_##TYPENAME##_put(remote, local, 10,             \
                                       (mype + 1) % npes);      \
        break;                                                  \
      case IPUT:                                                \
        if (USE_CTX)                                            \
          shmem_ctx_##TYPENAME##_iput(SHMEM_CTX_DEFAULT, remote,\
                           local, 1, 1, 10, (mype + 1) % npes); \
        else                                                    \
          shmem_##TYPENAME##_iput(remote, local, 1, 1, 10,      \
                                            (mype + 1) % npes); \
        break;                                                  \
      case PUT_NBI:                                             \
        if (USE_CTX)                                            \
          shmem_ctx_##TYPENAME##_put_nbi(SHMEM_CTX_DEFAULT,     \
                         remote, local, 10, (mype + 1) % npes); \
        else                                                    \
          shmem_##TYPENAME##_put_nbi(remote, local, 10,         \
                                            (mype + 1) % npes); \
        shmem_quiet();                                          \
        break;                                                  \
      default:                                                  \
        printf("Invalid operation (%d)\n", OP);                 \
        shmem_global_exit(1);                                   \
    }                                                           \
    shmem_barrier_all();                                        \
    for (int i = 0; i < 10; i++)                                \
      if (remote[i] != (TYPE)((mype + npes - 1) % npes)) {      \
        printf("PE %i received incorrect value with "           \
               "TEST_SHMEM_PUT(%s, %d, %s)\n", mype,            \
               #OP, (int)(USE_CTX), #TYPE);                     \
        rc = EXIT_FAILURE;                                      \
      }                                                         \
  } while (false)

int main(int argc, char* argv[]) {
  shmem_init();

  int rc = EXIT_SUCCESS;
  TEST_SHMEM_PUT(PUT, 0, 1, float, float);
  TEST_SHMEM_PUT(PUT, 0, 1, double, double);
  TEST_SHMEM_PUT(PUT, 0, 1, long double, longdouble);
  TEST_SHMEM_PUT(PUT, 0, 1, char, char);
  TEST_SHMEM_PUT(PUT, 0, 1, signed char, schar);
  TEST_SHMEM_PUT(PUT, 0, 1, short, short);
  TEST_SHMEM_PUT(PUT, 0, 1, int, int);
  TEST_SHMEM_PUT(PUT, 0, 1, long, long);
  TEST_SHMEM_PUT(PUT, 0, 1, long long, longlong);
  TEST_SHMEM_PUT(PUT, 0, 1, unsigned char, uchar);
  TEST_SHMEM_PUT(PUT, 0, 1, unsigned short, ushort);
  TEST_SHMEM_PUT(PUT, 0, 1, unsigned int, uint);
  TEST_SHMEM_PUT(PUT, 0, 1, unsigned long, ulong);
  TEST_SHMEM_PUT(PUT, 0, 1, unsigned long long, ulonglong);
  TEST_SHMEM_PUT(PUT, 0, 1, int8_t, int8);
  TEST_SHMEM_PUT(PUT, 0, 1, int16_t, int16);
  TEST_SHMEM_PUT(PUT, 0, 1, int32_t, int32);
  TEST_SHMEM_PUT(PUT, 0, 1, int64_t, int64);
  TEST_SHMEM_PUT(PUT, 0, 1, uint8_t, uint8);
  TEST_SHMEM_PUT(PUT, 0, 1, uint16_t, uint16);
  TEST_SHMEM_PUT(PUT, 0, 1, uint32_t, uint32);
  TEST_SHMEM_PUT(PUT, 0, 1, uint64_t, uint64);
  TEST_SHMEM_PUT(PUT, 0, 1, size_t, size);
  TEST_SHMEM_PUT(PUT, 0, 1, ptrdiff_t, ptrdiff);

  TEST_SHMEM_PUT(PUT, 1, 0, float, float);
  TEST_SHMEM_PUT(PUT, 1, 0, double, double);
  TEST_SHMEM_PUT(PUT, 1, 0, long double, longdouble);
  TEST_SHMEM_PUT(PUT, 1, 0, char, char);
  TEST_SHMEM_PUT(PUT, 1, 0, signed char, schar);
  TEST_SHMEM_PUT(PUT, 1, 0, short, short);
  TEST_SHMEM_PUT(PUT, 1, 0, int, int);
  TEST_SHMEM_PUT(PUT, 1, 0, long, long);
  TEST_SHMEM_PUT(PUT, 1, 0, long long, longlong);
  TEST_SHMEM_PUT(PUT, 1, 0, unsigned char, uchar);
  TEST_SHMEM_PUT(PUT, 1, 0, unsigned short, ushort);
  TEST_SHMEM_PUT(PUT, 1, 0, unsigned int, uint);
  TEST_SHMEM_PUT(PUT, 1, 0, unsigned long, ulong);
  TEST_SHMEM_PUT(PUT, 1, 0, unsigned long long, ulonglong);
  TEST_SHMEM_PUT(PUT, 1, 0, int8_t, int8);
  TEST_SHMEM_PUT(PUT, 1, 0, int16_t, int16);
  TEST_SHMEM_PUT(PUT, 1, 0, int32_t, int32);
  TEST_SHMEM_PUT(PUT, 1, 0, int64_t, int64);
  TEST_SHMEM_PUT(PUT, 1, 0, uint8_t, uint8);
  TEST_SHMEM_PUT(PUT, 1, 0, uint16_t, uint16);
  TEST_SHMEM_PUT(PUT, 1, 0, uint32_t, uint32);
  TEST_SHMEM_PUT(PUT, 1, 0, uint64_t, uint64);
  TEST_SHMEM_PUT(PUT, 1, 0, size_t, size);
  TEST_SHMEM_PUT(PUT, 1, 0, ptrdiff_t, ptrdiff);

  TEST_SHMEM_PUT(IPUT, 0, 0, float, float);
  TEST_SHMEM_PUT(IPUT, 0, 0, double, double);
  TEST_SHMEM_PUT(IPUT, 0, 0, long double, longdouble);
  TEST_SHMEM_PUT(IPUT, 0, 0, char, char);
  TEST_SHMEM_PUT(IPUT, 0, 0, signed char, schar);
  TEST_SHMEM_PUT(IPUT, 0, 0, short, short);
  TEST_SHMEM_PUT(IPUT, 0, 0, int, int);
  TEST_SHMEM_PUT(IPUT, 0, 0, long, long);
  TEST_SHMEM_PUT(IPUT, 0, 0, long long, longlong);
  TEST_SHMEM_PUT(IPUT, 0, 0, unsigned char, uchar);
  TEST_SHMEM_PUT(IPUT, 0, 0, unsigned short, ushort);
  TEST_SHMEM_PUT(IPUT, 0, 0, unsigned int, uint);
  TEST_SHMEM_PUT(IPUT, 0, 0, unsigned long, ulong);
  TEST_SHMEM_PUT(IPUT, 0, 0, unsigned long long, ulonglong);
  TEST_SHMEM_PUT(IPUT, 0, 0, int8_t, int8);
  TEST_SHMEM_PUT(IPUT, 0, 0, int16_t, int16);
  TEST_SHMEM_PUT(IPUT, 0, 0, int32_t, int32);
  TEST_SHMEM_PUT(IPUT, 0, 0, int64_t, int64);
  TEST_SHMEM_PUT(IPUT, 0, 0, uint8_t, uint8);
  TEST_SHMEM_PUT(IPUT, 0, 0, uint16_t, uint16);
  TEST_SHMEM_PUT(IPUT, 0, 0, uint32_t, uint32);
  TEST_SHMEM_PUT(IPUT, 0, 0, uint64_t, uint64);
  TEST_SHMEM_PUT(IPUT, 0, 0, size_t, size);
  TEST_SHMEM_PUT(IPUT, 0, 0, ptrdiff_t, ptrdiff);

  TEST_SHMEM_PUT(IPUT, 1, 0, float, float);
  TEST_SHMEM_PUT(IPUT, 1, 0, double, double);
  TEST_SHMEM_PUT(IPUT, 1, 0, long double, longdouble);
  TEST_SHMEM_PUT(IPUT, 1, 0, char, char);
  TEST_SHMEM_PUT(IPUT, 1, 0, signed char, schar);
  TEST_SHMEM_PUT(IPUT, 1, 0, short, short);
  TEST_SHMEM_PUT(IPUT, 1, 0, int, int);
  TEST_SHMEM_PUT(IPUT, 1, 0, long, long);
  TEST_SHMEM_PUT(IPUT, 1, 0, long long, longlong);
  TEST_SHMEM_PUT(IPUT, 1, 0, unsigned char, uchar);
  TEST_SHMEM_PUT(IPUT, 1, 0, unsigned short, ushort);
  TEST_SHMEM_PUT(IPUT, 1, 0, unsigned int, uint);
  TEST_SHMEM_PUT(IPUT, 1, 0, unsigned long, ulong);
  TEST_SHMEM_PUT(IPUT, 1, 0, unsigned long long, ulonglong);
  TEST_SHMEM_PUT(IPUT, 1, 0, int8_t, int8);
  TEST_SHMEM_PUT(IPUT, 1, 0, int16_t, int16);
  TEST_SHMEM_PUT(IPUT, 1, 0, int32_t, int32);
  TEST_SHMEM_PUT(IPUT, 1, 0, int64_t, int64);
  TEST_SHMEM_PUT(IPUT, 1, 0, uint8_t, uint8);
  TEST_SHMEM_PUT(IPUT, 1, 0, uint16_t, uint16);
  TEST_SHMEM_PUT(IPUT, 1, 0, uint32_t, uint32);
  TEST_SHMEM_PUT(IPUT, 1, 0, uint64_t, uint64);
  TEST_SHMEM_PUT(IPUT, 1, 0, size_t, size);
  TEST_SHMEM_PUT(IPUT, 1, 0, ptrdiff_t, ptrdiff);

  TEST_SHMEM_PUT(PUT_NBI, 0, 0, float, float);
  TEST_SHMEM_PUT(PUT_NBI, 0, 0, double, double);
  TEST_SHMEM_PUT(PUT_NBI, 0, 0, long double, longdouble);
  TEST_SHMEM_PUT(PUT_NBI, 0, 0, char, char);
  TEST_SHMEM_PUT(PUT_NBI, 0, 0, signed char, schar);
  TEST_SHMEM_PUT(PUT_NBI, 0, 0, short, short);
  TEST_SHMEM_PUT(PUT_NBI, 0, 0, int, int);
  TEST_SHMEM_PUT(PUT_NBI, 0, 0, long, long);
  TEST_SHMEM_PUT(PUT_NBI, 0, 0, long long, longlong);
  TEST_SHMEM_PUT(PUT_NBI, 0, 0, unsigned char, uchar);
  TEST_SHMEM_PUT(PUT_NBI, 0, 0, unsigned short, ushort);
  TEST_SHMEM_PUT(PUT_NBI, 0, 0, unsigned int, uint);
  TEST_SHMEM_PUT(PUT_NBI, 0, 0, unsigned long, ulong);
  TEST_SHMEM_PUT(PUT_NBI, 0, 0, unsigned long long, ulonglong);
  TEST_SHMEM_PUT(PUT_NBI, 0, 0, int8_t, int8);
  TEST_SHMEM_PUT(PUT_NBI, 0, 0, int16_t, int16);
  TEST_SHMEM_PUT(PUT_NBI, 0, 0, int32_t, int32);
  TEST_SHMEM_PUT(PUT_NBI, 0, 0, int64_t, int64);
  TEST_SHMEM_PUT(PUT_NBI, 0, 0, uint8_t, uint8);
  TEST_SHMEM_PUT(PUT_NBI, 0, 0, uint16_t, uint16);
  TEST_SHMEM_PUT(PUT_NBI, 0, 0, uint32_t, uint32);
  TEST_SHMEM_PUT(PUT_NBI, 0, 0, uint64_t, uint64);
  TEST_SHMEM_PUT(PUT_NBI, 0, 0, size_t, size);
  TEST_SHMEM_PUT(PUT_NBI, 0, 0, ptrdiff_t, ptrdiff);

  TEST_SHMEM_PUT(PUT_NBI, 1, 0, float, float);
  TEST_SHMEM_PUT(PUT_NBI, 1, 0, double, double);
  TEST_SHMEM_PUT(PUT_NBI, 1, 0, long double, longdouble);
  TEST_SHMEM_PUT(PUT_NBI, 1, 0, char, char);
  TEST_SHMEM_PUT(PUT_NBI, 1, 0, signed char, schar);
  TEST_SHMEM_PUT(PUT_NBI, 1, 0, short, short);
  TEST_SHMEM_PUT(PUT_NBI, 1, 0, int, int);
  TEST_SHMEM_PUT(PUT_NBI, 1, 0, long, long);
  TEST_SHMEM_PUT(PUT_NBI, 1, 0, long long, longlong);
  TEST_SHMEM_PUT(PUT_NBI, 1, 0, unsigned char, uchar);
  TEST_SHMEM_PUT(PUT_NBI, 1, 0, unsigned short, ushort);
  TEST_SHMEM_PUT(PUT_NBI, 1, 0, unsigned int, uint);
  TEST_SHMEM_PUT(PUT_NBI, 1, 0, unsigned long, ulong);
  TEST_SHMEM_PUT(PUT_NBI, 1, 0, unsigned long long, ulonglong);
  TEST_SHMEM_PUT(PUT_NBI, 1, 0, int8_t, int8);
  TEST_SHMEM_PUT(PUT_NBI, 1, 0, int16_t, int16);
  TEST_SHMEM_PUT(PUT_NBI, 1, 0, int32_t, int32);
  TEST_SHMEM_PUT(PUT_NBI, 1, 0, int64_t, int64);
  TEST_SHMEM_PUT(PUT_NBI, 1, 0, uint8_t, uint8);
  TEST_SHMEM_PUT(PUT_NBI, 1, 0, uint16_t, uint16);
  TEST_SHMEM_PUT(PUT_NBI, 1, 0, uint32_t, uint32);
  TEST_SHMEM_PUT(PUT_NBI, 1, 0, uint64_t, uint64);
  TEST_SHMEM_PUT(PUT_NBI, 1, 0, size_t, size);
  TEST_SHMEM_PUT(PUT_NBI, 1, 0, ptrdiff_t, ptrdiff);

  shmem_finalize();
  return rc;
}
