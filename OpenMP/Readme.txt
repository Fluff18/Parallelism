Include the header file:#include <omp.h>
Specify the parallel region
Set the number of threads:
Compile:
gcc -o hello -fopenmp hello.c
Execute:
./hello