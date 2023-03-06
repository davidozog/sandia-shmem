#include <stdio.h>
#include <shmem.h>
#include <shmemx.h>
#include <stdlib.h>

#include <CL/sycl.hpp>
#include"level_zero/ze_api.h"

int main() {

    sycl::queue Q;
    std::cout <<"Device : " << Q.get_device().get_info<sycl::info::device::name>() << std::endl;

    size_t external_heap_length = 64 * 1024;
    size_t *external_heap = sycl::malloc_device<size_t>(external_heap_length, Q);

    int *src = (int *) malloc(1 * sizeof(int));
    int *errors = (int *) sycl::malloc_host<int>(1, Q);

    *errors = 0;

    shmemx_heap_preinit(external_heap, external_heap_length, 0);
    shmem_init();

    int me = shmem_my_pe();
    int npes = shmem_n_pes();

    auto e1 = Q.submit([&](sycl::handler &h) {
        h.single_task([=]() {
            external_heap[0] = 55;
        });
    });
    e1.wait_and_throw();

    src[0] = me + 5;

    shmem_int_put((int *) external_heap, src, 1, (me + 1) % npes);

    shmem_barrier_all();

    auto e2 = Q.submit([&](sycl::handler &h) {
        h.single_task([=]() {
            *errors = external_heap[0];
        });
    });
    e2.wait_and_throw();

    std::cout << "[PE " << me << "] Data in device heap " << *errors << ", expected = " << (me - 1 + npes) % npes + 5 << std::endl;

    sycl::free(errors, Q);
    free(src);
    sycl::free(external_heap, Q);
    shmem_finalize(); 

    return 0;
}
