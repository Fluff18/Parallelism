#include <stdio.h>
#include <omp.h>
#define MAX_THREADS 8
int main(){
    static long num_steps = 10000;
    int num_threads_allocated = 0;
    double step = 1.0/(double)num_steps;
    omp_set_num_threads(MAX_THREADS);
    double pi = 0.0;
    #pragma omp parallel
    {
        int thread_id = omp_get_thread_num();
        int num_threads = omp_get_num_threads();
        if (thread_id ==0) num_threads_allocated = omp_get_num_threads();
        long count = 0; double x;
        double sum = 0.0;
        for (count = thread_id; count < num_steps; count += num_threads){
            x = (double)(count)*step;

            sum += (4.0/(1.0+x*x)) ;
        }
        #pragma omp atomic
        pi+=sum*step;
    }
    printf("value of PI is %lf\n",pi);
}