#include <iostream>
#include <vector>
#include <climits>
using namespace std;

const int INF = INT_MAX;

// Range Minimum Query
struct RMQ {
    int n;
    vector<int> dat;

    RMQ() {}

    RMQ(int n_) {
        init(n_);
    }

    RMQ(int n_, int* a) {
        init(n_);
        build(n_, a);
    }

    void init(int n_) {
        n = 1;
        while (n < n_) n *= 2;
        dat.clear();
        dat.resize(2 * n - 1, INF);
    }

    void build(int n_, int* a) {
        for (int i = 0; i < n_; i++) dat[i + n - 1] = a[i];
        for (int i = n - 2; i >= 0; i--) dat[i] = min(dat[i * 2 + 1], dat[i * 2 + 2]);
    }

    void update(int k, int a) {
        k += n - 1;
        dat[k] = a;

        while (k > 0) {
            k = (k - 1) / 2;
            dat[k] = min(dat[k * 2 + 1], dat[k * 2 + 2]);
        }
    }

    int query(int a, int b, int k, int l, int r) {
        if (r <= a || b <= l) return INF;
        if (a <= l && r <= b) return dat[k];

        int vl = query(a, b, k * 2 + 1, l, (l + r) / 2);
        int vr = query(a, b, k * 2 + 2, (l + r) / 2, r);
        return min(vl, vr);
    }

    int query(int a, int b) {
        return query(a, b, 0, 0, n);
    }
};

signed main() {
    int n, q;
    cin >> n >> q;

    int initial[n];
    for (int i = 0; i < n; i++) initial[i] = INF;

    RMQ rmq = RMQ(n, initial);

    const int UPDATE = 0;
    const int FIND = 1;

    for (int i = 0; i < q; i++) {
        int c, x, y;
        cin >> c >> x >> y;

        if (c == UPDATE) {
            rmq.update(x, y);
        } else if (c == FIND) {
            cout << rmq.query(x, y + 1) << endl;
        }
    }

    return 0;
}