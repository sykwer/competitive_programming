#include <iostream>
#include <utility>
#include <queue>
using namespace std;

const int INF = 1 << 25;
const int MAX_V = 10000;

struct edge {
    int to;
    int cost;
};

typedef pair<int, int> P; // min distance, vertex index

int V;
vector<edge> G[MAX_V];
int d[MAX_V];

// O(E * logV)
void dijkstra(int s) {
    priority_queue<P, vector<P>, greater<>> que;
    fill(d, d + V, INF);
    d[s] = 0;
    que.push(P(0, s));

    while (!que.empty()) {
        P p = que.top(); que.pop();
        int v = p.second;
        if (d[v] < p.first) continue;

        for (int i = 0; i < G[v].size(); i++) {
            edge e = G[v][i];
            if (d[e.to] > d[v] + e.cost) {
                d[e.to] = d[v] + e.cost;
                que.push(P(d[e.to], e.to));
            }
        }
    }
}

int main() {
    V = 3;
    G[0].push_back({1, 1});
    G[0].push_back({2, 4});
    G[1].push_back({0, 1});
    G[1].push_back({2, 2});
    G[2].push_back({0, 4});
    G[2].push_back({1, 2});

    dijkstra(0); // shortest paths from node-0

    for (int i = 0; i < V; i++) {
        cout << "cost 0->" << i << " : " << d[i] << endl;
    }

    return 0;
}