#include <iostream>
#include <algorithm>
using namespace std;

const int INF = 1 << 25;

const int MAX_E = 10000;
const int MAX_V = 1000;
int V;
int E;

struct edge {
    int from;
    int to;
    int cost;
};

edge es[MAX_E];
int d[MAX_V]; // shortest path

// O(V * E)
void bellman_ford(int s) {
    fill(d, d + V, INF);
    d[s] = 0;

    while (true) {
        bool update = false;

        for (int i = 0; i < E; i++) {
            edge e = es[i];
            if (d[e.from] != INF && d[e.to] > d[e.from] + e.cost) {
                d[e.to] = d[e.from] + e.cost;
                update = true;
            }
        }

        if (!update) break;
    }
}

// if true, there is negative loop
bool find_negative_loop() {
    memset(d, 0, sizeof(d));

    for (int i = 0; i < V; i++) {
        edge e = es[i];
        if (d[e.to] > d[e.from] + e.cost) {
            d[e.to] = d[e.from] + e.cost;

            if (i == V - 1) return true;
        }
    }

    return false;
}

int main() {
    V = 3;
    E = 3;
    es[0] = {0, 1, 1};
    es[1] = {0, 2, 4};
    es[2] = {1, 2, 2};

    bellman_ford(0); // shortest paths from node-0

    for (int i = 0; i < V; i++) {
        cout << "cost 0->" << i << " : " << d[i] << endl;
    }

    return 0;
}