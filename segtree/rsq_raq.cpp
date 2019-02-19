#include <iostream>
#include <vector>

using namespace std;

struct LasySegmentTree {
private:
    int n;
    vector<int> node;
    vector<int> lasy;
public:
    LasySegmentTree(vector<int> v) {
        int _n = (int) v.size();
        n = 1;
        while (n < _n) n *= 2;
        node.resize(2 * n - 1);
        lasy.resize(2 * n - 1, 0);
        for (int i = 0; i < _n; i++) node[i + n - 1] = v[i];
        for (int i = n - 2; i >= 0; i--) node[i] = node[2 * i + 1] + node[2 * i + 2];
    }

    void eval(int idx, int left, int right) {
        if (lasy[idx] == 0) return;

        node[idx] += lasy[idx];

        if (right - left > 1) {
            lasy[2 * idx + 1] += lasy[idx] / 2;
            lasy[2 * idx + 2] += lasy[idx] / 2;
        }

        lasy[idx] = 0;
    }

    void update(int a, int b, int val, int idx = 0, int left = 0, int right = -1) {
        if (right < 0) right = n;

        eval(idx, left, right);

        if (b <= left || right <= a) return;

        if (a <= left && right <= b) {
            lasy[idx] += (right - left) * val;
            eval(idx, left, right);
        } else {
            update(a, b, val, 2 * idx + 1, left, (left + right) / 2);
            update(a, b, val, 2 * idx + 2, (left + right) / 2, right);
            node[idx] = node[2 * idx + 1] + node[2 * idx + 2];
        }
    }

    int getsum(int a, int b, int idx = 0, int left = 0, int right = -1) {
        if (right < 0) right = n;

        if (b <= left || right <= a) return 0;

        eval(idx, left, right);
        if (a <= left && right <= b) return node[idx];

        return getsum(a, b, 2 * idx + 1, left, (left + right) / 2) +
               getsum(a, b, 2 * idx + 2, (left + right) / 2, right);
    }
};

// Range Sum Query and Range Add Query
// Validated at http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_2_G
signed main() {
    int n, q;
    cin >> n >> q;

    LasySegmentTree seg(vector<int>(n, 0));

    for (int i = 0; i < q; i++) {
        int c;
        cin >> c;

        if (c == 0) {
            int s, t, x;
            cin >> s >> t >> x;
            seg.update(s - 1, t, x);
        } else {
            int s, t;
            cin >> s >> t;
            cout << seg.getsum(s - 1, t) << endl;
        }
    }
}