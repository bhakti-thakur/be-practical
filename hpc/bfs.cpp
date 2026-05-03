#include <iostream>
#include <vector>
#include <omp.h>
using namespace std;

vector<vector<int>> adj;
int n; // num vertices

// Sequential BFS 
void bfs_seq(int src) {
    vector<bool> visited(n + 1, false);
    vector<int> frontier = {src};
    visited[src] = true;
    while (!frontier.empty()) {
        vector<int> next;
        for (int u : frontier)
            for (int v : adj[u])
                if (!visited[v]) { visited[v] = true; next.push_back(v); }
        frontier = next;
    }
}

// Parallel BFS
void bfs_par(int src) {
    vector<bool> visited(n + 1, false);
    vector<int> frontier = {src};
    visited[src] = true;
    while (!frontier.empty()) {
        vector<int> next;
        #pragma omp parallel
        {
            vector<int> local;
            #pragma omp for schedule(dynamic)
            for (int i = 0; i < (int)frontier.size(); i++)
                for (int v : adj[frontier[i]])
                    if (!visited[v]) { visited[v] = true; local.push_back(v); }
            #pragma omp critical
            next.insert(next.end(), local.begin(), local.end());
        }
        frontier = next;
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

    start = omp_get_wtime();
    bfs_seq(src);
    end = omp_get_wtime();
    cout << "Sequential BFS time: " << end - start << " sec\n";

    start = omp_get_wtime();
    bfs_par(src);
    end = omp_get_wtime();
    cout << "Parallel BFS time:   " << end - start << " sec\n";

    return 0;
}

// g++ -fopenmp bfs.cpp -o bfs
// ./bfs.exe