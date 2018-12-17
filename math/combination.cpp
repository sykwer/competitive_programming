#include <vector>
#include <iostream>

using namespace std;

// O(N^2)
vector<vector<int>> set_pascal(int N) {
    vector<vector<int>> c(N, vector<int>(N, 0));

    for (int i = 0; i < N; i++) {
        c[i][0] = 1;

        for (int j = 1; j <= i; j++) {
            c[i][j] = c[i-1][j] + c[i-1][j-1];
        }
    }

    return move(c);
}

vector<vector<int>> set_pascal(int N, int mod) {
    vector<vector<int>> c(N, vector<int>(N, 0));

    for (int i = 0; i < N; i++) {
        c[i][0] = 1;

        for (int j = 1; j <= i; j++) {
            c[i][j] = c[i-1][j] + c[i-1][j-1];
            if (c[i][j] >= mod) c[i][j] -= mod;
        }
    }

    return move(c);
}


signed main() {
}