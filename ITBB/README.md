
g++ -std=c++11 -o vector vectoraddition.cpp    \
   –I ${TBB_ROOT}/include                  \
   -L ${TBB_ROOT}/lib/intel64/gcc4.7 –ltbb
