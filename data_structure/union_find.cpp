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

signed main() {
    UnionFind uf(5);
    uf.unite(0, 1);
    uf.unite(1, 2);

    cout << uf.cnt << endl; // 3
    cout << uf.same(0, 2) << endl; // true

    return 0;
}