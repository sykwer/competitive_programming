#include <vector>
#include <iostream>

using namespace std;

/* dp[mask][k]: 頂点群maskに対して頂点kを終点とするハミルトンパスの総数 */
// 初期値: dp[0010][1] = 1 のように1nodeに対するハミルトンパスをの総数を初期値とする
// 求める値: sum(dp[all visited][k])

class DirectedGraph {
    vector<vector<int>> table;
    int N;

public:
    DirectedGraph(int n) {
        N = n;
        table.resize(N);
    }

    // If the graph is undirected, you have to call add_edge(s, t); add_edge(t, s);
    void add_edge(int s, int t) {
        table[s].push_back(t);
    }

    // O(N^2 * 2^N)
    int count_hamiltonian_path() {
        vector<vector<int>> dp(1 << N, vector<int>(N, 0));
        for (int i = 0; i < N; i++) dp[1 << i][i] = 1;

        for (int mask = 0; mask < (1 << N); mask++) {
            for (int i = 0; i < N; i++) {
                if (!(mask & (1 << i))) continue;
                for (int next : table[i]) {
                    if (mask & (1 << next)) continue;
                    dp[mask | (1 << next)][next] += dp[mask][i];
                }
            }
        }

        int ans = 0;
        for (int i = 0; i < N; i++) ans += dp[(1 << N) - 1][i];
        return ans;
    }
};

signed main() {
    int N = 3;
    DirectedGraph *dg = new DirectedGraph(N);
    for (int i = 0; i < N; i++) for (int j = i + 1; j < N; j++) {
        dg->add_edge(i, j);
        dg->add_edge(j, i);
    }
    cout << dg->count_hamiltonian_path() << endl;
}