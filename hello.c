#include <stdio.h>
#include "mpi.h"

int main(int argc,char **argv)
{
	int myid, nprocs;
	int namelen,mpiv,mpisubv;
	char processor_name[MPI_MAX_PROCESSOR_NAME];

	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&nprocs);
	MPI_Comm_rank(MPI_COMM_WORLD,&myid);
	MPI_Get_processor_name(processor_name,&namelen);

	printf("Process %d of %d on %s\n",myid, nprocs, processor_name);

	if (myid == 0){
		MPI_Get_version(&mpiv, &mpisubv);
		printf("MPI Version: %d.%d\n",mpiv,mpisubv);
	}

	MPI_Finalize();
	return 0;


}
