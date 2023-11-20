#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

int thread_count;
const int N = 1e5 + 10;

void Odd_even(int a[], int n) {
   int phase, i, tmp;

   for (phase = 0; phase < n; phase++) {
      if (phase % 2 == 0)
#        pragma omp parallel for num_threads(thread_count) \
            default(none) shared(a, n) private(i, tmp)
         for (i = 1; i < n; i += 2) {
            if (a[i-1] > a[i]) {
               tmp = a[i-1];
               a[i-1] = a[i];
               a[i] = tmp;
            }
         }
      else
#        pragma omp parallel for num_threads(thread_count) \
            default(none) shared(a, n) private(i, tmp)
         for (i = 1; i < n-1; i += 2) {
            if (a[i] > a[i+1]) {
               tmp = a[i+1];
               a[i+1] = a[i];
               a[i] = tmp;
            }
         }
   }
}

void work(int n_thread, int sz){
   int  n;
   int* a;
   double start, finish;

   thread_count = n_thread;
   n = sz;
   a = (int *)malloc(n * sizeof(int));
   srand(1);
   for (int i = 0; i < n; i++) a[i] = rand() % 100;

   start = omp_get_wtime();
   Odd_even(a, n);
   finish = omp_get_wtime();
   
   printf("%d %e\n", n, finish - start);
}

int main() {
   for(int i = 1000; i < N; i += 5000) work(2, i);
   return 0;
}