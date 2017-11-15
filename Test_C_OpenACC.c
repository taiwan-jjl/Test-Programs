/*
**Copyright (C) 2017, Jyong-Jhih Lin.
**All rights reserved.
**Licensed under the GNU Affero General Public License v3.0. See LICENSE file in the project root for full license information.
*/
/*
Test programs for various systems.
Type: C11, OpenACC, standard output, time
compile:
gcc -std=c11 -Wall -Wextra -O0 <file> -lm -static
pgcc -c11 -Minform=inform -O0 -Minfo -ta=host/multicore/tesla,9.0,cc50 <file>
 */


/*
size_t is the unsigned integer type of the result of sizeof , alignof (since C11) and offsetof.

time_t
This is an arithmetic type returned by the functions timer() and mktime() (usually defined as long).

TIME_UTC is int
#define TIME_UTC implementation-defined (since C11)
*/




#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main(void)
{
size_t i=0;
size_t const end_count=512*1024*1024/8;
double time_difference=0.0;
double time_difference2=0.0;
struct timespec start, end;	//C11 only, new method to measure time.
double a[end_count];
double b[end_count];
double c[end_count];

timespec_get(&start, 1/*TIME_UTC*/);
for(i=0;i<end_count;i++){
	a[i] = 1.0*2.0 + 3.0;
}
for(i=0;i<end_count;i++){
	b[i] = 4.0*5.0 + 6.0;
}
for(i=0;i<end_count;i++){
	c[i] = 7.0*a[i] + b[i];
}
timespec_get(&end, 1/*TIME_UTC*/);

//type "time_t" is defined by the OS, not in C standard.
//Therefore, the size of time_t decides how long the calendar time can be.
time_difference = (double)(end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec) / 1.0e9;

timespec_get(&start, 1/*TIME_UTC*/);
#pragma acc data create(a[0:end_count],b[0:end_count],c[0:end_count])
{
#pragma acc kernels
{
for(i=0;i<end_count;i++){
     a[i] = 1.0*2.0 + 3.0;
}
for(i=0;i<end_count;i++){
     b[i] = 4.0*5.0 + 6.0;
}
for(i=0;i<end_count;i++){
     c[i] = 7.0*a[i] + b[i];
}
}
}
timespec_get(&end, 1/*TIME_UTC*/);

time_difference2 = (double)(end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec) / 1.0e9;

printf("Type: C11, OpenACC, standard output, time\n");
printf("The time difference is %.9f\n", time_difference);
printf("The time difference2 is %.9f\n", time_difference2);
printf("The speed-up ratio is %.9f\n", time_difference/time_difference2);

return 0;
}

/*
TIMESPEC_GET(3)            Library Functions Manual            TIMESPEC_GET(3)

NAME
     timespec_get - get current calendar time

LIBRARY
     Standard C Library (libc, -lc)

SYNOPSIS
     #include <time.h>

     #define TIME_UTC 1

     int
     timespec_get(struct timespec *ts, int base);

DESCRIPTION
     The timespec_get function sets the interval pointed to by ts to hold the
     current calendar time based on the specified time base in base.

     Currently the only supported valid base is TIME_UTC.  It returns time
     elapsed since epoch.

RETURN VALUES
     The timespec_get function returns the passed value of base if successful,
     otherwise 0 on failure.

SEE ALSO
     clock_gettime(2)

STANDARDS
     The timespec_get function conforms to ISO/IEC 9899:2011 ("ISO C11").

HISTORY
     This interface first appeared in NetBSD 8.

AUTHORS
     Kamil Rytarowski <kamil@NetBSD.org>

NetBSD 8.99                     October 4, 2016                    NetBSD 8.99

https://www.daemon-systems.org/man/timespec_get.3.html
*/