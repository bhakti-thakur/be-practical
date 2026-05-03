#include <iostream>
#include <vector>
#include <ctime>
#include <omp.h>
using namespace std;

// Sequential Bubble Sort 
void bubble_sort_seq(vector<int> arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (arr[j] > arr[j + 1])
                swap(arr[j], arr[j + 1]);
}

// Parallel Bubble Sort (Odd-Even Transposition) 
void bubble_sort_par(vector<int> arr) {
    int n = arr.size();
    for (int phase = 0; phase < n; phase++) {
        int start = phase % 2; // 0 = even phase, 1 = odd phase
        #pragma omp parallel for
        for (int i = start; i < n - 1; i += 2)
            if (arr[i] > arr[i + 1])
                swap(arr[i], arr[i + 1]);
    }
}

// Main 
int main() {
    int n;
    cout << "Enter number of elements: ";
    cin >> n;

    vector<int> arr(n);
    srand(time(0));
    for (int i = 0; i < n; i++)
        arr[i] = rand() % 1000000;

    double start, end;

    start = omp_get_wtime();
    bubble_sort_seq(arr);
    end = omp_get_wtime();
    cout << "Sequential bubble sort time: " << end - start << " sec\n";

    start = omp_get_wtime();
    bubble_sort_par(arr);
    end = omp_get_wtime();
    cout << "Parallel bubble sort time:   " << end - start << " sec\n";

    return 0;
}

// g++ -fopenmp bubblesort.cpp -o bubble
// ./bubble.exe