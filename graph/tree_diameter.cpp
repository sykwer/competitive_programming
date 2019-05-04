#include <iostream>
#include <vector>
using namespace std;

class UnDirectedTree {
  struct edge {
      int to;
      int cost;
  };

  vector<vector<edge>> table;
  int N;
  vector<int> distances;

  void dfs(int now, int par) {
      for (edge e : table[now]) {
          if (e.to == par) continue;
          distances[e.to] = distances[now] + e.cost;
          dfs(e.to, now);
      }
  }
public:
    UnDirectedTree (int n) {
        table.resize(n);
        distances.resize(n);
        N = n;
    }

    void add_edge(int s, int t, int cost) {
        table[s].push_back({t, cost});
        table[t].push_back({s, cost});
    }

    int diameter(void) {
        fill(distances.begin(), distances.end(), 0);
        dfs(0, -1);

        int mx_idx = -1;
        int mx = -1;
        for (int i = 0; i < N; i++) {
            if (distances[i] > mx) {
                mx_idx = i;
                mx = distances[i];
            }
        }

        fill(distances.begin(), distances.end(), 0);
        dfs(mx_idx, -1);

        int ret = -1;
        for (int d : distances) ret = max(ret, d);
        return ret;
    }
};

// validated at http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_5_A
int main(void) {
    int N;
    cin >> N;
    UnDirectedTree dt = UnDirectedTree(N);
    for (int i = 0; i < N - 1; i++) {
        int s, t, w;
        cin >> s >> t >> w;
        dt.add_edge(s, t, w);
    }
    cout << dt.diameter() << endl;
}