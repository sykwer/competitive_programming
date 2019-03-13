#include <iostream>
#include <vector>

using namespace std;

const int INF = 1 << 30;

// O(FE) where F = Amount of Maximum Flow
struct FordFulKerson {
    struct edge {
        int to;
        int cap;
        int rev;
    };

    int V;
    vector<vector<edge>> table;
    vector<bool> visited;

    int dfs(int s, int t, int mn) {
        if (s == t) return mn;
        visited[s] = true;

        for (int i = 0; i < table[s].size(); i++) {
            edge &e = table[s][i];
            if (visited[e.to] || e.cap == 0) continue;
            int diff = dfs(e.to, t, min(mn, e.cap));
            if (diff > 0) {
                e.cap -= diff;
                table[e.to][e.rev].cap += diff;
                return diff;
            }
        }

        return 0;
    }

public:
    FordFulKerson(int v) {
        V = v;
        table.resize(V);
        visited.resize(V);
    }

    void add_edge(int from, int to, int cap) {
        table[from].emplace_back((edge) {to, cap, (int) table[to].size()});
        table[to].emplace_back((edge) {from, 0, (int) table[from].size() - 1});
    }

    int calc(int from, int to) {
        int flow = 0;

        while (true) {
            fill(visited.begin(), visited.end(), false);
            int f = dfs(from, to, INF);
            if (f == 0) break;
            flow += f;
        }

        return flow;
    }
};

// Verified in http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_6_A
signed main() {
    int V, E;
    cin >> V >> E;

    FordFulKerson ff = FordFulKerson(V);
    for (int i = 0; i < E; i++) {
        int s, t, cap;
        cin >> s >> t >> cap;
        ff.add_edge(s, t, cap);
    }

    cout << ff.calc(0, V - 1) << endl;
}