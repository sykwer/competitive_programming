#include <iostream>
#include <utility>
#include <queue>
using namespace std;

const int INF = 1 << 25;
typedef pair<int, int> P; // min distance, vertex index

class DirectedGraph {
    struct edge {
        int to;
        int cost;
    };

public:
    int V;
    vector<vector<edge>> G;

    DirectedGraph(int v) : G(v), V(v) {}

    void add_edge(int u, int v, int cost) {
        G[u].push_back({v, cost});
    }

    // O(E * logV)
    vector<int> dijkstra(int s) {
        vector<int> d(V, INF);
        d[s] = 0;

        priority_queue<P, vector<P>, greater<>> que;
        que.push({0, s});

        while (!que.empty()) {
            P  p = que.top(); que.pop();
            int v = p.second;
            if (d[v] < p.first) continue;

            for (edge e : G[v]) {
                if (d[e.to] > d[v] + e.cost) {
                    d[e.to] = d[v] + e.cost;
                    que.push({d[e.to], e.to});
                }
            }
        }

        return move(d);
    }
};

int main() {
    DirectedGraph ug = DirectedGraph(3);

    ug.add_edge(0, 1, 1);
    ug.add_edge(1, 0, 1);

    ug.add_edge(0, 2, 4);
    ug.add_edge(2, 0, 4);

    ug.add_edge(1, 2, 2);
    ug.add_edge(2, 1, 2);

    vector<int> d = ug.dijkstra(0);

    for (int i = 0; i < ug.V; i++) {
        cout << "cost 0->" << i << " : " << d[i] << endl;
    }

    return 0;
}