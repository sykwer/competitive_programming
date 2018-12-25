#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

const int INF = 1 << 25;

class DirectedGraph {
    struct edge {
        int from;
        int to;
        int cost;
    };

    int edge_num = 0;

public:
    int V;
    int E;
    vector<edge> edges;

    DirectedGraph(int v, int e) : V(v), edges(e), E(e) {};

    void add_edge(int from, int to, int cost) {
        edges[edge_num++] = {from, to, cost};
    }

    // O(E * V)
    vector<int> bellman_ford(int s) {
        vector<int> d(V, INF);
        d[s] = 0;

        while (true) {
            bool update = false;
            for (int i = 0; i < E; i++) {
                edge e = edges[i];
                if (d[e.from] != INF && d[e.to] > d[e.from] + e.cost) {
                    d[e.to] = d[e.from] + e.cost;
                    update = true;
                }
            }

            if (!update) break;
        }

        return move(d);
    }

    bool find_negative_loop() {
        vector<int> d(V, 0);

        for (int i = 0; i < V; i++) {
            for (int j = 0; j < E; j++) {
                edge e = edges[j];
                if (d[e.to] > d[e.from] + e.cost) {
                    d[e.to] = d[e.from] + e.cost;
                    if (i == V - 1) return true;
                }
            }
        }

        return false;
    }
};

int main() {
    DirectedGraph dg = DirectedGraph(3, 3);
    dg.add_edge(0, 1, 1);
    dg.add_edge(0, 2, 4);
    dg.add_edge(1, 2, 2);
    vector<int> d = dg.bellman_ford(0);

    for (int i = 0; i < dg.V; i++) {
        cout << "cost 0->" << i << " : " << d[i] << endl;
    }

    return 0;
}