/*
**Copyright (C) 2017, Jyong-Jhih Lin.
**All rights reserved.
**Licensed under the GNU Affero General Public License v3.0. See LICENSE file in the project root for full license information.
*/
/*
Test programs for various systems.
Type: C11, MPI, random number, standard output
compile:
mpicc(gcc) -std=c11 -Wall -Wextra -O0 <file> -lm -static
mpicc(pgcc) -c11 -Minform=inform -O0 -Minfo -ta=host/multicore/tesla,9.0,cc50 <file>
 */




#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>
#include <mpi.h>


int main(int argc, char* argv[])
{
int rank;
int size;
char name[MPI_MAX_PROCESSOR_NAME];
int name_len;
int reduce_send = 1;
int reduce_receive = 0;


MPI_Init(&argc, &argv);

MPI_Comm_rank(MPI_COMM_WORLD, &rank);
MPI_Comm_size(MPI_COMM_WORLD, &size);
MPI_Get_processor_name(name, &name_len);

printf("Hello world from processor %s, rank %d out of %d processors\n", name, rank, size);


MPI_Reduce(&reduce_send, &reduce_receive, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
if(rank==0){   
     printf("MPI Reduce to count processor number = %i\n", reduce_receive);
}
/*
*
*
*
*/
srand(time(NULL) + rank);
double px = 0;
double py = 0;
int i = 0;
int drop = INT_MAX / size /100;
int local_inside = 0;
int global_inside = 0;

for(i=0;i<drop;i++){
     px = (double)(rand()) / (double)(RAND_MAX);
     py = (double)(rand()) / (double)(RAND_MAX);
     if( ( pow(px,2) + pow(py,2) ) <= (double)(1) ){
          local_inside++;
     }
}

MPI_Barrier(MPI_COMM_WORLD);
MPI_Reduce(&local_inside, &global_inside, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
if(rank==0){   
     printf("RAND_MAX = %u\n", RAND_MAX);
     printf("MPI PI = %.15lf\n", (double)(global_inside*4) / (double)(drop*size));
     printf("MPI global_inside*4 = %i\n", (global_inside*4));
     printf("MPI drop*size = %i\n", (drop*size));
}

MPI_Finalize();
return 0;
}