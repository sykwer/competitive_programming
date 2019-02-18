#include <iostream>
#include <vector>

using namespace std;

// Range Sum Query
struct SegmentTree {
private:
    int n;
    vector<int> node;

public:
    SegmentTree(vector<int> v) {
        int _n = v.size();
        n = 1;
        while (n < _n) n *= 2;
        node.resize(2 * n - 1, 0);
        for (int i = 0; i < _n; i++) node[i + n - 1] = v[i];
        for (int i = n - 2; i >= 0; i--) node[i] = node[2 * i + 1] + node[2 * i + 2];
    }

    void add(int idx, int val) {
        idx += n - 1;
        node[idx] += val;
        while (idx > 0) {
            idx = (idx - 1) / 2;
            node[idx] = node[2 * idx + 1] + node[2 * idx + 2];
        }
    }

    int get_sum(int a, int b, int idx = 0, int left = 0, int right = -1) {
        if (right < 0) right = n;
        if (right <= a || b <= left) return 0;
        if (a <= left && right <= b) return node[idx];
        int mid = (left + right) / 2;
        return get_sum(a, b, 2 * idx + 1, left, mid) + get_sum(a, b, 2 * idx + 2, mid, right);
    }
};

signed main() {
    int n, q;
    cin >> n >> q;

    SegmentTree seg(vector<int>(n, 0));

    for (int i = 0; i < q; i++) {
        int c, x, y;
        cin >> c >> x >> y;
        if (c == 0) {
            seg.add(x - 1, y);
        } else {
            cout << seg.get_sum(x - 1, y) << endl;
        }
    }
}