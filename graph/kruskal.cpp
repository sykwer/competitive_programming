#include <iostream>
#include <vector>
#include <numeric>
using namespace std;

class UnionFind {
public:
    vector<int> par, rank;
    int cnt;

    UnionFind(int n) : cnt(n) {
        par = vector<int>(n);
        iota(par.begin(), par.end(), 0);
        rank = vector<int>(n, 0);
    }

    int root(int x) {
        if (par[x] == x) {
            return x;
        } else {
            return par[x] = root(par[x]);
        }
    }

    void unite(int x, int y) {
        x = root(x);
        y = root(y);
        if (x == y) return;
        if (rank[x] < rank[y]) {
            par[x] = y;
        } else {
            par[y] = x;
            if (rank[x] == rank[y]) rank[x]++;
        }
        cnt--;
    }

    bool same(int x, int y) {
        return root(x) == root(y);
    }
};

struct edge {
    int u;
    int v;
    int cost;
};

bool comp_edge(const edge& e1, const edge& e2) {
    return e1.cost < e2.cost;
}

int kruskal(int V, vector<edge>& es) {
    sort(es.begin(), es.end(), comp_edge);
    UnionFind uf(V);
    int ret = 0;

    for (edge e : es) {
        if (uf.same(e.u, e.v)) continue;
        uf.unite(e.u, e.v);
        ret += e.cost;
    }

    return ret;
}

int main() {
    int V, E;
    cin >> V >> E;

    vector<edge> es(E);

    for (int i = 0; i < E; i++) {
        cin >> es[i].u >> es[i].v >> es[i].cost;
    }

    cout << kruskal(V, es) << endl;
}