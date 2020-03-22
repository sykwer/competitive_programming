#include <iostream>
#include <vector>
#include <functional>
using ll = long long;
using namespace std;

template <class T> class SegTree {
    int N;
    vector<T> data;
    T ide; // Identity element (Initial value)
    function<T(T, T)> operation; // Function for range query
    function<T(T, T)> update; // Function for point update

    T _query(int a, int b, int k, int l, int r) {
        if (r <= a || b <= l) return ide;
        if (a <= l && r <= b) {
            return data[k];
        } else {
            T left = _query(a, b, 2 * k + 1, l, (l + r) / 2);
            T right = _query(a, b, 2 * k + 2, (l + r) / 2, r);
            return operation(left, right);
        }
    }

public:
    // int n:                       Size of array
    // T ide:                       Identity element
    // function<T(T, T)> operation: Function to merge elements
    // function<T(T, T)> update:    (old data[i], some value x) -> new data[i]
    SegTree(size_t n, T ide, function<T(T, T)> operation,
            function<T(T, T)> update) : ide(ide), operation(operation), update(update) {
        N = 1;
        while (N < n) N *= 2;
        data.resize(2 * N - 1, ide);
    }

    void change(int i, T x) {
        i += N - 1;
        data[i] = update(data[i], x);
        while (i > 0) {
            i = (i - 1) / 2;
            data[i] = operation(data[i * 2 + 1], data[i * 2 + 2]);
        }
    }

    T query(int a, int b) {
        return _query(a, b, 0, 0, N);
    }

    T operator[](int i) {
        return data[i + N - 1];
    }
};

// AOJ Range Minimum Query
int main() {
    int N, Q;
    cin >> N >> Q;

    ll ide = (1LL << 31) - 1;
    auto operation = [](ll left, ll right) -> ll { return min(left, right); };
    auto change = [](ll old, ll x) -> ll { return x; };
    auto seg = new SegTree<ll>(N, ide, operation, change);

    for (int q = 0; q < Q; q++) {
        int c, x, y;
        cin >> c >> x >> y;
        if (!c) seg->change(x, y);
        else cout << seg->query(x, y+1) << endl;
    }
}