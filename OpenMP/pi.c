#include <stdio.h>
#include <omp.h>
#define MAX_THREADS 2
int main(){
    static long num_steps = 10000;
    int num_threads_allocated = 0;
    double step = 1.0/(double)num_steps;
    omp_set_num_threads(MAX_THREADS);
    double sum[MAX_THREADS];
    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        int num_threads = omp_get_num_threads();
        if (tid ==0) num_threads_allocated = omp_get_num_threads();
        long count = 0; double x;
        sum[tid] = 0.0;
        for (count = tid; count < num_steps; count += num_threads){
            x = (double)(count)*step;
            sum[tid] += 4.0/(1.0+x*x);
        }
    }
    double final_sum = 0.0;
    for (int p = 0; p < num_threads_allocated; p++)
        final_sum+=sum[p]*step;
    printf("value of PI is %lf\n",final_sum);
}