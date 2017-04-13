#include <stdio.h>
#include <shmem.h>
#include <ctype.h>
#include <string.h>

#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main( int argc, char** argv ) {

  shmem_init();

  int npes = shmem_n_pes();
  int my_pe = shmem_my_pe();

  printf("PE %d of %d says hi\n", my_pe, npes);

  int i = 0;

  char hostname[256];
  gethostname(hostname, sizeof(hostname));

  if (my_pe == 0) system("rm gdb.[0-9]*");

  //shmem_barrier_all();
  sleep(1);

  char filename[64];
  sprintf(filename, "gdb.%d", getpid());
  FILE *fp = fopen(filename, "w");
  fprintf( fp, "%d", getpid() );
  fclose(fp);

  printf("PID %d on %s ready for attach\n", getpid(), hostname);

  while (0 == i)
      sleep(1);

  shmem_barrier_all();

  printf("PE %d of %d entering finalize\n", my_pe, npes);

  shmem_finalize();

  printf("PE %d of %d is out of finalize\n", my_pe, npes);

  return 0;
}

