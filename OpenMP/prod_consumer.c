#include <omp.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>

#define N        1000

#define SEED       4564
#define rmult  4562
#define radd   23462
#define rmod   23415
int randy = SEED;
void frand(int length, double *a)
{
   int i;
   for (i=0;i<length;i++) {
     randy = (rmult * randy + radd) % rmod;
     *(a+i) = ((double) randy)/((double) rmod);
   }
}

double Avg_n(int length, double *a)
{
   int i;  double sum = 0.0;

   for (i=0;i<length;i++) {
     sum += *(a+i);
   }

   return sum;
}

int main()
{
  double *A, sum, runtime;
  int flag = 0;

  A = (double *)malloc(N*sizeof(double));

  runtime = omp_get_wtime();

  #pragma omp parallel
  {
    #pragma omp sections
    {
      #pragma omp section
      {
          frand(N, A);
          #pragma omp flush
          #pragma omp atomic write
          flag = 1;
          #pragma omp flush(flag)
      }
      #pragma omp section
      {
        int flgtmp;
        while (1){
          #pragma omp flush(flag)
          #pragma omp atomic read
          flgtmp = flag;
          if (flgtmp == 1) break;
        }
          #pragma omp flush
          sum = Avg_n(N, A);
      }
    }
  }

  runtime = omp_get_wtime() - runtime;

  printf(" In %f seconds, The sum is %f \n",runtime,sum);
}
