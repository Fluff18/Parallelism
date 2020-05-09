#include<stdio.h>
#include <stdlib.h>
#include <amp.h>
using namespace concurrency;
void MultiplyWithAMP() {
int aMatrix[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
int bMatrix[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
int productMatrix[] = { 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0 };

array_view<int, 8> a(8, 8, aMatrix);

array_view<int, 8> b(8, 8, bMatrix);

array_view<int, 8> product(8, 8, productMatrix);

parallel_for_each(product.extent,
   [=] (index<8> idx) restrict(amp) {
       int row = idx[0];
       int col = idx[1];
       for (int inner = 0; inner <8; inner++) {
           product[idx] += a(row, inner)* b(inner, col);
       }
   });

product.synchronize();

for (int row = 0; row <8; row++) {
   for (int col = 0; col <8; col++) {
       //std::cout << productMatrix[row*8 + col] << "  ";
       std::cout << product(row, col) << "  ";
   }
   std::cout << "\n";
  }
}
int main() {
    MultiplyWithOutAMP();
    MultiplyWithAMP();
    getchar();
}