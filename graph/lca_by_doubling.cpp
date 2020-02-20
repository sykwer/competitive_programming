#include <iostream>
#include <vector>

using namespace std;

class LcaByDoubling {
    int N;
    int logN;
    vector<vector<int>> table;
    vector<int> depths;

    // dp[i][j] = 2**j th parent node from i th node
    vector<vector<int>> dp;

    void calc_depth(int node, int par, int depth) {
        depths[node] = depth;
        dp[node][0] = par;
        for (int next : table[node]) {
            if (next == par) continue;
            calc_depth(next, node, depth + 1);
        }
    }

    void calc_dp() {
        for (int k = 1; k < logN; k++) {
            for (int i = 0; i < N; i++) {
                if (dp[i][k-1] == -1) dp[i][k] = -1;
                else dp[i][k] = dp[dp[i][k-1]][k-1];
            }
        }
    }

public:
    LcaByDoubling(const vector<vector<int>> &table, int root) : table(table) {
        N = table.size();
        logN = (sizeof(int) * 8) - __builtin_clz(N);
        depths.resize(N);
        dp.resize(N);
        for (int i = 0; i < N; i++) dp[i].resize(logN, -1);
        calc_depth(root, -1, 0);
        calc_dp();
    }

    // O(logN)
    int lca(int u, int v) {
        // Assume depths[u] <= depths[v]
        if (depths[u] > depths[v]) swap(u, v);

        // Equalize depth
        int diff = depths[v] - depths[u];
        for (int k = 0; k < logN; k++) {
            if ((diff >> k) & 1) v = dp[v][k];
        }

        if (u == v) return u;

        // Popup both nodes just below the lca
        // Wrong: for (int k = 0; k < logN; k++) { ... }
        for (int k = logN - 1; k >= 0; k--) {
            if (dp[u][k] != dp[v][k]) {
                u = dp[u][k];
                v = dp[v][k];
            }
        }

        return dp[u][0];
    }
};

// Verified at http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_5_C
int main() {
    int N;
    cin >> N;
    vector<bool> is_root(N, true);
    vector<vector<int>> table(N);

    for (int i = 0; i < N; i++) {
        int k;
        cin >> k;
        for (int j = 0; j < k; j++) {
            int c;
            cin >> c;
            is_root[c] = false;
            table[i].push_back(c);
            table[c].push_back(i);
        }
    }

    int root = -1;
    for (int i = 0; i < N; i++) if (is_root[i]) root = i;

    auto solver = new LcaByDoubling(table, root);

    int Q;
    cin >> Q;
    for (int q = 0; q < Q; q++) {
        int u, v;
        cin >> u >> v;
        cout << solver->lca(u, v) << endl;
    }

    return 0;
}