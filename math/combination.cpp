#include <vector>
#include <iostream>

using namespace std;

const long long MOD = 1e9 + 7;
#define int long long

// pre: O(N^2), call: O(1) ----------------------------------------------------
vector<vector<int>> set_pascal(int N) {
    vector<vector<int>> c(N + 1, vector<int>(N + 1, 0));

    for (int i = 0; i <= N; i++) {
        c[i][0] = 1;

        for (int j = 1; j <= i; j++) {
            c[i][j] = c[i-1][j] + c[i-1][j-1];
        }
    }

    return move(c);
}

vector<vector<int>> set_pascal(int N, int mod) {
    vector<vector<int>> c(N + 1, vector<int>(N + 1, 0));

    for (int i = 0; i <= N; i++) {
        c[i][0] = 1;

        for (int j = 1; j <= i; j++) {
            c[i][j] = c[i-1][j] + c[i-1][j-1];
            if (c[i][j] >= mod) c[i][j] -= mod;
        }
    }

    return move(c);
}

// pre: O(n), call: O(1) ----------------------------------------------------
class Combination {
    int powmod(int a, int p) {
        int ans = 1;
        int mul = a;

        for (; p > 0; p >>= 1, mul = (mul * mul) % mod) {
            if ((p & 1) == 1) ans = (ans * mul) % mod;
        }

        return ans;
    }
public:
    int N;
    int mod;
    vector<int> fact;
    vector<int> revFact;

    Combination(int n, int m) : N(n), mod(m), fact(n+1), revFact(n+1) {
        fact[0] = 1;
        for (int i = 1; i <= N; i++) {
            fact[i] = fact[i-1] * i;
            fact[i] %= mod;
        }

        // Fermat's little theorem
        revFact[N] = powmod(fact[N], mod - 2);

        for (int i = N-1; i >= 0; i--) {
            revFact[i] = revFact[i+1] * (i+1);
            revFact[i] %= mod;
        }
    }

    int get(int a, int b) {
        return (((fact[a] * revFact[b]) % mod) * revFact[a-b]) % mod;
    }
};

// O(b + log(mod)) ----------------------------------------------------
int powmod(int a, int p, int mod) {
    int ans = 1;
    int mul = a;

    for (; p > 0; p >>= 1, mul = (mul * mul) % mod) {
        if ((p & 1) == 1) ans = (ans * mul) % mod;
    }

    return ans;
}

int comb(int a, int b, int mod) {
    int ans = 1;
    if (b > a / 2) return comb(a, a - b, mod);
    int div = 1;

    for (int i = 0; i < b; i++) {
        ans *= a - i;
        ans %= mod;

        div *= i + 1;
        div %= mod;
    }

    // Fermat's little theorem
    ans *= powmod(div, mod - 2, mod);

    return ans % mod;
}

signed main() {
}