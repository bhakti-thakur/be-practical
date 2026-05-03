#include <iostream>
#include <vector>
#include <omp.h>
#include <cstdlib>
#include <ctime>
using namespace std;

// Merge Helper 
void merge(vector<int>& arr, int l, int m, int r) {
    vector<int> L(arr.begin() + l, arr.begin() + m + 1);
    vector<int> R(arr.begin() + m + 1, arr.begin() + r + 1);
    int i = 0, j = 0, k = l;
    while (i < L.size() && j < R.size())
        arr[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];
    while (i < L.size()) arr[k++] = L[i++];
    while (j < R.size()) arr[k++] = R[j++];
}

// Sequential Merge Sort
void merge_sort_seq(vector<int>& arr, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        merge_sort_seq(arr, l, m);
        merge_sort_seq(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

// Parallel Merge Sort
void merge_sort_par(vector<int>& arr, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        #pragma omp task
        merge_sort_par(arr, l, m);
        #pragma omp task
        merge_sort_par(arr, m + 1, r);
        #pragma omp taskwait
        merge(arr, l, m, r);
    }
}

void parallel_merge_sort(vector<int>& arr) {
    #pragma omp parallel
    #pragma omp single
    merge_sort_par(arr, 0, arr.size() - 1);
}

// Main
int main() {
    int n;
    cout << "Enter number of elements: ";
    cin >> n;

    srand(time(0));
    vector<int> arr1(n), arr2(n);
    for (int i = 0; i < n; i++)
        arr1[i] = arr2[i] = rand() % 1000000;

    double start, end;

    start = omp_get_wtime();
    merge_sort_seq(arr1, 0, n - 1);
    end = omp_get_wtime();
    cout << "Sequential merge sort time: " << end - start << " sec\n";

    start = omp_get_wtime();
    parallel_merge_sort(arr2);
    end = omp_get_wtime();
    cout << "Parallel merge sort time:   " << end - start << " sec\n";

    return 0;
}

// g++ -fopenmp mergesort.cpp -o merge
// ./merge.exe