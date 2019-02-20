#include <iostream>
#include <stack>
#include <vector>

using namespace std;

struct TopologicalSort {
private:
    int V;
    int E;
    vector<vector<int>> table;
public:
    TopologicalSort(int v, int e) {
        V = v;
        E = e;
        table.resize(v);
    }

    void add_edge(int from, int to) {
        table[from].push_back(to);
    }

    // if the size of vector is less than V, this graph is cyclic
    vector<int> sort() {
        vector<int> sorted_nodes;
        stack<int> st;

        vector<int> indegree(V, 0);
        for (int i = 0; i < V; i++) {
            for (int to : table[i]) {
                indegree[to]++;
            }
        }

        for (int i = 0; i < V; i++) {
            if (indegree[i] == 0) st.push(i);
        }

        while (st.size() > 0) {
            int popped = st.top();
            st.pop();

            sorted_nodes.push_back(popped);

            for (int to : table[popped]) {
                indegree[to]--;
                if (indegree[to] == 0) st.push(to);
            }
        }

        return move(sorted_nodes);
    }
};

// O(V + E)
// Validated at http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_4_B
signed main() {
    int V, E;
    cin >> V >> E;

    TopologicalSort ts(V, E);

    for (int i = 0; i < E; i++) {
        int s, t;
        cin >> s >> t;
        ts.add_edge(s, t);
    }

    vector<int> sorted_nodes = ts.sort();
    for (int node: sorted_nodes) cout << node << endl;
}

