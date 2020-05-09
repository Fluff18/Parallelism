#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

# define np 100
# define MAXITER 100

struct func{
   double r;
   double i;
};
void trial(struct func c);
struct func c;
int out = 0;

int main(){
   int i, j;
   double area, error, eps  = 1.0e-5;

#pragma omp parallel for default(shared) private(c,j) firstprivate(eps)
   for (i=0; i<np; i++) {
     for (j=0; j<np; j++) {
       c.r = -2.0+2.5*(double)(i)/(double)(np)+eps;
       c.i = 1.125*(double)(j)/(double)(np)+eps;
       trial(c);
     }
   }

    area=2.0*2.5*1.125*(double)(np*np-out)/(double)(np*np);
   error=area/(double)np;

   printf("Mandlebrot set Area = %12.8f +/- %12.8f\n",area,error);
   printf("Correct answer 1.510659\n");

}

void trial(struct func c){

       struct func z;
       int iter;
       double temp;

       z=c;
       for (iter=0; iter<MAXITER; iter++){
         temp = (z.r*z.r)-(z.i*z.i)+c.r;
         z.i = z.r*z.i*2+c.i;
         z.r = temp;
         if ((z.r*z.r+z.i*z.i)>4.0) {
            #pragma omp atomic
            out++;
            break;
         }
       }
}