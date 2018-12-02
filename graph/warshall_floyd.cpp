#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;

const int INF = 1 << 25;

class DirectedGraph {
public:
    int V;
    vector<vector<int>> costs;

    DirectedGraph(int v) : V(v), costs(v, vector<int>(v, INF)) {
        for (int i = 0LL; i < V; i++) {
            costs[i][i] = 0LL;
        }
    }

    void add_edge(int u, int v, int cost) {
        costs[u][v] = cost;
    }

    void warshall_floyd() {
        for (int k = 0LL; k < V; k++) {
            for (int i = 0LL; i < V; i++) {
                for (int j = 0LL; j < V; j++) {
                    costs[i][j] = min(costs[i][j], costs[i][k] + costs[k][j]);
                }
            }
        }
    }
};

signed main() {
    int V = 3;

    DirectedGraph g = DirectedGraph(V);
    g.add_edge(0, 1, 1);
    g.add_edge(1, 2, 1);
    g.add_edge(2, 0, 4);
    g.warshall_floyd();

    vector<vector<int>> costs = g.costs;
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            cout << costs[i][j] << " ";
        }
        cout << endl;
    }
}