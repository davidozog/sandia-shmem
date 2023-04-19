#include <shmem.h>
#include <stdio.h>

#define NELEMS 10

int main(int argc, char* argv[])
{
    shmem_init();

    int my_pe = shmem_my_pe();
    int n_pes = shmem_n_pes();

    /* Allocate source and target data on the symmetric heap */
    int* source_data = shmem_malloc(sizeof(int) * NELEMS);
    int* target_data = shmem_malloc(sizeof(int) * NELEMS);

    /* Initialize source data */
    for (int i = 0; i < NELEMS; i++) {
        source_data[i] = my_pe * NELEMS + i;
    }

    /* All PEs put their source data to their "neighbor" PE. */
    /* The PE with the largest ID sends its data to process 0 */
    shmem_put(target_data, source_data, NELEMS, (my_pe + 1) % n_pes);

    /* Assure all the communication has completed */
    shmem_barrier_all();

    /* Check the result is correct! */
    int errors = 0;
    for (int i = 0; i < NELEMS; i++) {
        int source_pe = my_pe ? ((my_pe-1) % n_pes) : n_pes-1;
        if (target_data[i] != source_pe * NELEMS + i)
                errors++;
    }
    if (errors == 0)
        printf("No errors on PE %d.\n", my_pe);
    else
        printf("Encountered %d errors on PE %d.\n", errors, my_pe);

    /* Free the allocated data from the symmetric heap */
    shmem_free(source_data);
    shmem_free(target_data);

    shmem_finalize();
}


