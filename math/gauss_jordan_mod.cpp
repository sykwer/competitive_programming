#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

template<int mod> int gauss_jordan(vector<vector<long long>> &A, bool extended = false) {
    auto powmod = [&](long long a, long long p) {
        long long ret = 1;
        long long mul = a;
        while (p > 0) {
            if (p & 1) (ret *= mul) %= mod;
            mul = (mul * mul) % mod;
            p >>= 1;
        }
        return ret;
    };

    int n = A.size();
    int m = A[0].size();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            A[i][j] = (A[i][j] % mod + mod) % mod;
        }
    }
    int rank = 0;
    for (int col = 0; col < m; col++) {
        if (extended && col == m-1) break;
        int pivot = -1;
        for (int row = rank; row < n; row++) {
            if (A[row][col] != 0) {
                pivot = row;
                break;
            }
        }
        if (pivot == -1) continue;
        swap(A[rank], A[pivot]);

        long long inv = powmod(A[rank][col], mod-2);
        for (int col2 = 0; col2 < m; col2++) A[rank][col2] = A[rank][col2] * inv % mod;

        for (int row = 0; row < n; row++) {
            if (row != rank && A[row][col]) {
                int fac = A[row][col];
                for (int col2 = 0; col2 < n; col2++) {
                    A[row][col2] -= A[rank][col2] * fac % mod;
                    if (A[row][col2] < 0) A[row][col2] += mod;
                }
            }
        }
        ++rank;
    }
    return rank;
}

// Ax = b (mod). Returns rank of matrix A if the equation is valid.
template<int mod> int linear_equation(vector<vector<long long>> const &A, vector<long long> const &b, vector<long long> &ans) {
    int n = A.size();
    int m = A[0].size();
    vector<vector<long long>> M(n, vector<long long>(m+1));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            M[i][j] = A[i][j];
        }
        M[i][m] = b[i];
    }
    int rank = gauss_jordan<mod>(M, true);
    ans.resize(0);
    for (int i = rank; i < n; i++) if (M[i][m]) return rank;
    ans.resize(n);
    for (int i = 0; i < rank; i++) ans[i] = M[i][m];
    return rank;
}

// TCO 2013 Round 2A Med TheMagicMatrix
class TheMagicMatrix {
        const int MOD = 1234567891;
public:
    int find(int n, vector<int> rows, vector<int> cols, vector<int> values) {
        int m = rows.size();
        vector<bool> row_flags(n, false);
        vector<bool> col_flags(n, false);
        for (int i = 0; i < m; i++) {
            row_flags[rows[i]] = true;
            col_flags[cols[i]] = true;
        }
        bool blank_row = false;
        bool blank_col = false;
        for (int i = 0; i < n; i++) if (!row_flags[i]) blank_row = true;
        for (int i = 0; i < n; i++) if (!col_flags[i]) blank_col = true;

        auto powmod = [&](int a, int p) -> int {
            long long ret = 1;
            long long mul = a;
            while (p > 0) {
                if (p & 1) (ret *= mul) %= MOD;
                mul = (mul * mul) % MOD;
                p >>= 1;
            }
            return (int)ret;
        };

        if (blank_row && blank_col) return powmod(10, (n-1)*(n-1)-m+1);

        long long ret = 0;
        vector<vector<long long>> A(2 * n + m, vector<long long>(n * n));
        for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) A[i][i*n+j] = 1;
        for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) A[i+n][i+j*n] = 1;
        for (int i = 0; i < m; i++) A[2*n+i][rows[i] * n + cols[i]] = 1;
        for (int X = 0; X < 10; X++) {
            vector<long long> b(2*n+m, X);
            for (int i = 0; i < m; i++) b[2*n+i] = values[i];
            int mul = 1;

            vector<long long> ans5;
            int rank5 = linear_equation<5>(A, b, ans5);
            if (ans5.size() == 0) continue;
            (mul *= powmod(MOD, n * n - rank5)) %= MOD;

            vector<long long> ans2;
            int rank2 = linear_equation<2>(A, b, ans2);
            if (ans2.size() == 0) continue;
            (mul *= powmod(MOD, n * n - rank2)) %= MOD;

            (ret += mul) %= MOD;

        }
        return (int)ret;
    }
};

int main() {
    auto solver = new TheMagicMatrix();
    int n, m;
    cin >> n >> m;
    vector<int> rows(m), cols(m) , vals(m);
    for (int i = 0; i < m; i++) cin >> rows[i];
    for (int i = 0; i < m; i++) cin >> cols[i];
    for (int i = 0; i < m; i++) cin >> vals[i];
    cout << solver->find(n, rows, cols, vals) << endl;
}
