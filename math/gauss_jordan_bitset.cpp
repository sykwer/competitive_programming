#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <bitset>
using namespace std;

const int MAX_ROW = 1005;
const int MAX_COL = 1005;

struct BitMatrix {
    int H, W;
    bitset<MAX_COL> val[MAX_ROW];
    BitMatrix(int n = 1, int m = 1) : H(n), W(m) {}
    inline bitset<MAX_COL> &operator[] (int i) { return val[i]; }
};

// O(HW)
int gauss_jordan(BitMatrix &A, bool extended = false) {
    int rank = 0;
    for (int col = 0; col < A.W; col++) {
        if (extended && col == A.W - 1) break;
        int pivot = -1;
        for (int row = rank; row < A.H; row++) {
            if (A[row][col]) {
                pivot = row;
                break;
            }
        }
        if (pivot == -1) continue;
        swap(A[rank], A[pivot]);
        for (int row = 0; row < A.H; row++) {
            if (row != rank && A[row][col]) A[row] ^= A[rank];
        }
        ++rank;
    }
    return rank;
}

int linear_equation(BitMatrix &A, vector<int> const &b, vector<int> &ans) {
    int n = A.H;
    int m = A.W;
    BitMatrix M(n, m+1);
    for (int i = 0; i < n; i++) {
        M[i] = A[i];
        M[i][m] = b[i];
    }
    int rank = gauss_jordan(M, true);

    ans.resize(0);
    for (int i = rank; i < n; i++) if (M[i][m]) return rank;
    ans.resize(n, 0);
    for (int i = 0; i < rank; i++) ans[i] = M[i][m];
    return rank;
}

// Aizu1308 - Awkward Lights
bool solve() {
    int M, N, D;
    cin >> M >> N >> D;
    if (!(M || N || D)) return false;
    vector<int> b(M*N);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            int a;
            cin >> a;
            b[i * M + j] = a;
        }
    }
    BitMatrix A(N*M, N*M);
    for (int r1 = 0; r1 < N; r1++) {
        for (int c1 = 0; c1 < M; c1++) {
            for (int r2 = 0; r2 < N; r2++) {
                for (int c2 = 0; c2 < M; c2++) {
                    int dis = abs(r1 - r2) + abs(c1 - c2);
                    if (dis == 0 || dis == D) A[r1 * M + c1][r2 * M + c2] = true;
                }
            }
        }
    }
    vector<int> ans;
    linear_equation(A, b, ans);
    if (ans.size() > 0) cout << 1 << endl;
    else cout << 0 << endl;
    return true;
}

int main() {
    bool flag = true;
    while (flag) flag = solve();
}
