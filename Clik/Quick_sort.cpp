
// PARALLEL CODE FOR VECTOR ADDITION IS NOT DONE
// COMPARSION STUDY WAS DONE AT https://www.cilkplus.org/tutorial-array-notation 

#include <cilk.h>
#include <cilkview.h>
#include <algorithm>
#include <functional>
using namespace std;
void quick_sort(int * beg, int * end)
{
	if (beg < end)
	{
		end = end--;
		int * pivot = partition(beg, end, bind2nd(less<int>(), *end));
		swap(*end, *pivot);
		cilk_spawn quick_sort(begin, pivot);
		pivot++;
		end++;
		quick_sort(pivot, end);
		cilk_sync; 
	}
}
int cilk_main()
{
	int n;
	cout<<"Enter total number of elements that are to be sorted:";
	cin>>n;
	int* a = new int[n];
	cilk_for (int x = 0; x< n; x++)
	{
		a[x] = x;
	}
	random_shuffle(a, a + n);
	cout << "Sorting"<< n << "elements" << endl;
	cilk::cilkview cilkview;
	cilkview.start();
	quick_sort(a, a + n);
	cilkview.stop();
	cilkview.dump("Qucik_Sort_Results", false);
	cout << "Quciksort on" << n <<"items succeeded." << endl;
	cout << "Total Time taken by Quick Sort:” << cilkview.accumulated_milliseconds()/1000.f <<"Seconds"<< endl;
	delete[]a;
	return 0;
}