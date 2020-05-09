#include <stdio.h>
#include <omp.h>
int main(){
    static long num_steps = 10000;
    double sum = 0.0;
    #pragma omp parallel for reduction(+:sum)
    for (long count =0; count < num_steps; count += 1){
            double x = (double)(count)*(1.0/num_steps);
            sum += 4.0/(1.0+x*x);
    }
    printf("value of PI is %lf\n",sum*(1.0/num_steps));
}