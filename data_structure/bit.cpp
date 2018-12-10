#include <iostream>
#include <vector>

using namespace std;

// TODO: Wrap to class

const int MAX_N = 10000;

int bit[MAX_N + 1], n;

int sum(int i) {
    int s = 0;
    while (i > 0) {
        s += bit[i];
        i -= i & -i;
    }
    return s;
}

void add_to(int i, int x) {
    while (i <= n) {
        bit[i] += x;
        i += i & -i;
    }
}

// inversion
signed main() {
    n = 4;
    int a[4] = {3, 1, 4, 2};

    int ret = 0;
    for (int j = 0; j < n; j++) {
        ret += j - sum(a[j]);
        add_to(a[j], 1);
    }

    cout << ret << endl; // 3

    return 0;
}