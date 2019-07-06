#include <iostream>
#include <map>
#include <vector>

using namespace std;

vector<int> compress(vector<int> &A) {
    auto N = (int) A.size();
    map<int, int> m;
    for (int a : A) m[a] = 0;

    int idx = 0;
    for (auto it = m.begin(); it != m.end(); it++) {
        it->second = idx;
        idx++;
    }

    vector<int> B(N);
    for (int i = 0; i < N; i++) B[i] = m[A[i]];

    return move(B);
}

signed main() {
    vector<int> vec = {3, 3, 1, 6, 1};
    vector<int> compressed = compress(vec);

    for (int elem : compressed) cout << elem << " "; // 1 1 0 2 0
    cout << endl;
}