#include <iostream>
#include <vector>
#include <omp.h>
using namespace std;

vector<vector<int>> adj;
int n;

// Sequential DFS
void dfs_seq(int node, vector<bool>& visited) {
    visited[node] = true;
    for (int v : adj[node])
        if (!visited[v])
            dfs_seq(v, visited);
}

// Parallel DFS
void dfs_par(int node, vector<bool>& visited) {
    visited[node] = true;
    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < (int)adj[node].size(); i++) {
        int v = adj[node][i];
        if (!visited[v]) {
            #pragma omp critical
            visited[v] = true;
            dfs_par(v, visited);
        }
    }
}

// Main
int main() {
    int edges, src;
    cout << "Enter vertices, edges, source: ";
    cin >> n >> edges >> src;

    adj.resize(n + 1);
    cout << "Enter edges (u v):\n";
    for (int i = 0; i < edges; i++) {
        int u, v; cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    double start, end;

    vector<bool> vis1(n + 1, false);
    start = omp_get_wtime();
    dfs_seq(src, vis1);
    end = omp_get_wtime();
    cout << "Sequential DFS time: " << end - start << " sec\n";

    vector<bool> vis2(n + 1, false);
    start = omp_get_wtime();
    #pragma omp parallel
    #pragma omp single
    dfs_par(src, vis2);
    end = omp_get_wtime();
    cout << "Parallel DFS time:   " << end - start << " sec\n";

    return 0;
}

// g++ -fopenmp dfs.cpp -o dfs
// ./dfs.exe