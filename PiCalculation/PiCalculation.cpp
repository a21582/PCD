//Include
#include <omp.h>
#include <iostream>
#include <iomanip>
#include <chrono>

//Using
using namespace std;
using namespace std::chrono;
static long long num_steps = 10000000000L;
                              

#define NUM_THREADS 256
//Optimized pi calculation in multy threads
double piMultiThreadNonOptimized()
{
	int nthreads; 
	double sum[NUM_THREADS];
	double sumF = 0.0L;
	double result;

#pragma omp parallel num_threads(NUM_THREADS)
	{
		long long i;
		int id = omp_get_thread_num();
		for (i = id, sum[id] = 0.0; i < num_steps; i = i + NUM_THREADS) {
			sum[id] += (i % 2 == 0 ? 1.0L : -1.0L) / (2L * i + 1L);
		}
	}

	for (int i = 0; i < NUM_THREADS; i++) {
		sumF += sum[i];
	}
	result = 4.0L * sumF;
	return result;
}

//Sequential pi calculation
double piSequency()
{
	double result;
	double factor = 1.0L;
	double sum = 0.0L;
	for (long long i = 0; i < num_steps; i++){
		factor = (i % 2 == 0 ? 1.0L : -1.0L);
		sum += factor / (2L * i + 1L);
	}
	result = 4.0L * sum;
	return result;
}

//pi calculation in multy threads
double piMultiThread()
{
	double result;
	double factor = 1.0L;
	double sum = 0.0L;
	steady_clock::time_point beginMultiThread = steady_clock::now();
#pragma omp parallel for reduction(+:sum) private(factor) num_threads(NUM_THREADS)
	for (long long i = 0; i < num_steps; i++) {
		factor = (i % 2 == 0 ? 1.0L : -1.0L);
		sum += factor / (2L * i + 1L);
	}
	steady_clock::time_point endMultiThread = steady_clock::now();
	cout << "Elapse Thread Time:" << duration_cast<milliseconds>(endMultiThread - beginMultiThread).count() << "[ms]" << endl;
	result = 4.0L * sum;
	return result;
}

int main()
{
	steady_clock::time_point begin = steady_clock::now();

	cout << "Start"  << endl;
	steady_clock::time_point beginFunction = steady_clock::now();
	
	double pi = piMultiThread();
	//double pi = piMultiThreadNonOptimized();
	//double pi = piSequency();

	cout << "Pi value:" << setprecision(16) << pi <<  endl;
	cout << "Interactions:" << num_steps << endl;
	steady_clock::time_point endFunction = steady_clock::now();
	cout << "Elapse Function Time:" << duration_cast<milliseconds>(endFunction - beginFunction).count() << "[ms]" << endl;

	steady_clock::time_point end = steady_clock::now();
	cout << "Elapse all Time:" << duration_cast<milliseconds>(end - begin).count() << "[ms]" << endl;
}

