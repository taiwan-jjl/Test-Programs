/*
**Copyright (C) 2019, Jyong-Jhih Lin.
**All rights reserved.
**Licensed under the GNU Affero General Public License v3.0. See LICENSE file in the project root for full license information.
*/
/*
Test programs for MPI Point-to-Point communication.
Type: C11, MPI, standard output
compile:
mpicc(gcc) -std=c11 -Wall -Wextra -O0 <file> -lm -static
mpicc(pgcc) -c11 -Minform=inform -O0 -Minfo -ta=host/multicore/tesla,9.0,cc50 <file>
mpiicc(icc) -std=c11 -Wall -O0 <file> -lm
 */




#include <stdlib.h>
#include <stdio.h>
//#include <math.h>
//#include <limits.h>
#include <mpi.h>


int main(int argc, char* argv[])
{
int rank;
int size;
char name[MPI_MAX_PROCESSOR_NAME];
int name_len;


MPI_Init(&argc, &argv);

MPI_Comm_rank(MPI_COMM_WORLD, &rank);
MPI_Comm_size(MPI_COMM_WORLD, &size);
MPI_Get_processor_name(name, &name_len);

printf("Hello world from processor %s, rank %d out of %d processors\n", name, rank, size);
MPI_Barrier(MPI_COMM_WORLD);

int number=0;
for(int i=0; i<size; i++){
     for(int j=0; j<size; j++){
          if(rank == i && i != j){
               number = 1;
               MPI_Send(&number, 1, MPI_INT, j, 0, MPI_COMM_WORLD);
          }
          else if(rank == j && i != j){
               number = 0;
               MPI_Recv(&number, 1, MPI_INT, i, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
               printf("Process %s Rank %d received status %d from Rank %d\n", name, j, number, i);
          }
          MPI_Barrier(MPI_COMM_WORLD);
          if(rank==0){printf("i = %d, j = %d \n",i ,j);}
          MPI_Barrier(MPI_COMM_WORLD);
     }
}


MPI_Barrier(MPI_COMM_WORLD);
MPI_Finalize();
return 0;
}