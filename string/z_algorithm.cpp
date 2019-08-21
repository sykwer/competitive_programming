#include <iostream>
#include <vector>
#include <string>

using namespace std;

// O(|S|)
vector<int> z_algorithm(const string &S) {
    auto N = (int) S.size();

    // Z-Array
    vector<int> Z(N);
    Z[0] = N;

    /* For the already computed prefixes S[0..len-1] == S[idx..idx+len-1]
     * r = max(idx+len-1)
     * l = such i that maximizes (idx+len-1)
     */
    int l = -1;
    int r = -1;

    for (int k = 1; k < N; k++) {
        int p = k - l;

        if (k > r) {
            for (int i = k; i <= N; i++) {
                if (i == N || S[i-k] != S[i]) {
                    Z[k] = i-k;
                    l = k;
                    r = i-1;
                    break;
                }
            }
        } else if (Z[p] < r-k+1) {
            Z[k] = Z[p];
        } else { // Z[p] >= r-k+1
            for (int i = r+1; i <= N; i++) {
                if (i == N || S[i-k] != S[i]) {
                    Z[k] = i-k;
                    l = k;
                    r = i-1;
                    break;
                }
            }
        }
    }

    return move(Z);
}

// Verified at https://yukicoder.me/problems/no/430 (TLE for the several samples)
int main() {
    string str;
    int Q;
    cin >> str >> Q;

    int ret = 0;

    for (int i = 0; i < Q; i++) {
        string pattern;
        cin >> pattern;
        auto M = (int) pattern.size();

        string T;
        T += pattern;
        T += "$";
        T += str;
        vector<int> z_array = z_algorithm(T);

        int cnt = 0;
        for (int j = M + 1; j < z_array.size(); j++) {
            if (z_array[j] == M) cnt++;
        }
        ret += cnt;
    }

    cout << ret << endl;
    return 0;
}