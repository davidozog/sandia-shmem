#include <stdio.h>
#include <shmem.h>

#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main( int argc, char** argv ) {

  shmem_init();

  int npes = shmem_n_pes();
  int my_pe = shmem_my_pe();

  printf("PE %d of %d says hi\n", my_pe, npes);

{
  int i = 0;
  char hostname[256];
  gethostname(hostname, sizeof(hostname));
  system("rm gdb.[0-9]*");
  char filename[64];
  sprintf(filename, "gdb.%d", getpid());
  FILE *fp = fopen(filename, "w");
  fprintf( fp, "%d", getpid() );
  fclose(fp);
  printf("PID %d on %s ready for attach\n", getpid(), hostname);

  fflush(stdout);
  while (0 == i)
      sleep(1);
}

  shmem_barrier_all();

  printf("PE %d of %d is out of barrier\n", my_pe, npes);

  shmem_finalize();

  return 0;
}

