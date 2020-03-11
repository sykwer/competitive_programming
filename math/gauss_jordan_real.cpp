#include <vector>
#include <iostream>
#include <cmath>
using namespace std;

const double EPS = 1e-10;
typedef vector<vector<double>> Matrix;

// Returns matrix's rank
int gauss_jordan(Matrix &A, bool extended = false) {
    int n = A.size();
    int m = A[0].size();

    int rank = 0;
    for (int col = 0; col < m; col++) {
        if (extended && col == m-1) break;
        int pivot = -1;
        double mx = EPS;
        for (int row = rank; row < n; row++) {
            if (abs(A[row][col]) > mx) {
                mx = abs(A[row][col]);
                pivot = row;
            }
        }
        if (pivot == -1) continue;
        swap(A[rank], A[pivot]);

        double div = A[rank][col];
        for (int col2 = 0; col2 < m; col2++) A[rank][col2] /= div;
        for (int row = 0; row < n; row++) {
            if (row != rank && abs(A[row][col]) > EPS) {
                double fac = A[row][col];
                for (int col2 = 0; col2 < m; col2++) {
                    A[row][col2] -= A[rank][col2] * fac;
                }
            }
        }
        ++rank;
    }

    return rank;
}

// Ax = b
vector<double> linear_equation(Matrix A, vector<double> b) {
    int n = A.size();
    int m = A[0].size();
    Matrix M(n, vector<double>(m+1));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            M[i][j] = A[i][j];
        }
        M[i][m] = b[i];
    }
    int rank = gauss_jordan(M, true);



    vector<double> ans;
    for (int row = rank; row < m; row++) {
        if (M[row][m] > EPS) return ans;
    }
    ans.resize(n);
    for (int i = 0; i < n; i++) ans[i] = M[i][m];
    return ans;
}

int main() {
    Matrix A = {
            {1, -2, 3},
            {4, -5, 6},
            {7, -8, 10},
    };
    vector<double> b = {6, 12, 21};
    vector<double> x = linear_equation(A, b);

    for (int i = 0; i < x.size(); i++) {
        cout << x[i];
        if (i != x.size()-1) cout << " ";
    }
    cout << endl;
}