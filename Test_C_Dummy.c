/*
**Copyright (C) 2019, Jyong-Jhih Lin.
**All rights reserved.
**Licensed under the GNU Affero General Public License v3.0. See LICENSE file in the project root for full license information.
*/
/*
Test programs for CPU dummy occupation, single core version.
Type: C11, random
compile:
gcc(gcc) -std=c11 -Wall -Wextra -O0 <file> -static
pgcc(pgcc) -c11 -Minform=inform -O0 -Minfo -ta=host/multicore/tesla,9.0,cc50 <file>
icc(icc) -std=c11 -Wall -O0 <file> -static
 */


#define _POSIX_C_SOURCE 199309L //for nanosleep()

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#include <threads.h> //C11, need newer libc
#include <unistd.h> //POSIX.1-2001, POSIX.1-2008.


int main(int argc, char* argv[])
{
srand(time(NULL)); // use current time as seed for random generator
//printf("Random value on [0,%d]\n", RAND_MAX);
struct timespec ts;
ts.tv_sec = 0;
ts.tv_nsec = 1000L;
 
while( 1 ){
    double d_rand = (double) rand();
    double d_max = (double) RAND_MAX;
    double frac = d_rand/d_max;
    double d_rand2 = (double) rand();
    double frac2 = d_rand2/d_max;
    
    for(int i=0;i<10000;i++){
    if( frac > 0.8){
        //thrd_sleep(&(struct timespec){.tv_sec=1}, NULL); // sleep 1 sec
        nanosleep(&ts, NULL);
    }else{
        for(int i=0;i<9000;i++){rand();}
    }
    }
    for(int i=0;i<1000;i++){
    if( frac > frac2 ){
        //thrd_sleep(&(struct timespec){.tv_sec=1}, NULL); // sleep 1 sec
        nanosleep(&ts, NULL);
    }else{
        for(int i=0;i<9000;i++){rand();}
    }
    }
    
}


return 0;
}


/*
SYNOPSIS         top
       #include <time.h>

       int nanosleep(const struct timespec *req, struct timespec *rem);

   Feature Test Macro Requirements for glibc (see feature_test_macros(7)):

       nanosleep(): _POSIX_C_SOURCE >= 199309L
*/