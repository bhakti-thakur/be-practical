#include <iostream>
#include <vector>
#include <omp.h>
#include <climits>
#include <ctime>
using namespace std;

// Sequential
void reduction_seq(vector<int>& arr) {
    int mn = INT_MAX, mx = INT_MIN, sum = 0;
    for (int x : arr) {
        if (x < mn) mn = x;
        if (x > mx) mx = x;
        sum += x;
    }
    cout << "  Min: " << mn << "\n  Max: " << mx
         << "\n  Sum: " << sum << "\n  Avg: " << (double)sum / arr.size() << "\n";
}

// Parallel
void reduction_par(vector<int>& arr) {
    int mn = INT_MAX, mx = INT_MIN, sum = 0;
    #pragma omp parallel for reduction(min:mn) reduction(max:mx) reduction(+:sum)
    for (int i = 0; i < (int)arr.size(); i++) {
        if (arr[i] < mn) mn = arr[i];
        if (arr[i] > mx) mx = arr[i];
        sum += arr[i];
    }
    cout << "  Min: " << mn << "\n  Max: " << mx
         << "\n  Sum: " << sum << "\n  Avg: " << (double)sum / arr.size() << "\n";
}

// Main 
int main() {
    int n;
    cout << "Enter number of elements: ";
    cin >> n;

    srand(time(0));
    vector<int> arr(n);
    for (int i = 0; i < n; i++)
        arr[i] = rand() % 1000000;

    double start, end;

    start = omp_get_wtime();
    reduction_seq(arr);
    end = omp_get_wtime();
    cout << "Sequential time: " << end - start << " sec\n\n";

    start = omp_get_wtime();
    reduction_par(arr);
    end = omp_get_wtime();
    cout << "Parallel time:   " << end - start << " sec\n";

    return 0;
}

// g++ -fopenmp reduction.cpp -o reduce
// ./reduce.exe