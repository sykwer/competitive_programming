#include <iostream>
#include <vector>
#include <functional>
using namespace std;

template <typename T, typename E> class SegTree {
    // T: Element set
    // E: Operator set

    // Lazy SegTree can be used if..
    // 1. (T, F) is monoid (Identity element is ti)
    // 2. (E, H) is monoid (Identity element is ei)
    // 3. g(f(t1, t2), e1) = f(g(t1, e1), g(t2, e1))
    //    or g(f(t1, t2), w(e1, len)) = f(g(t1, w(e1, len/2)), g(t2, w(e1, len/2)))
    // 4. g(g(t1, e1), e2) = g(t1, h(e1, e2))
    // 5. g(t1, ei) = t1
    using F = function<T(T, T)>;
    using G = function<T(T, E)>;
    using H = function<E(E, E)>;
    using W = function<E(E, int)>;

    int N;
    F f;
    G g;
    H h;
    W w;
    T ti;
    E ei;
    vector<T> data;
    vector<E> lazy;

    void eval(int k, int len) {
        if (lazy[k] == ei) return;
        if (k < N-1) {
            lazy[2*k+1] = h(lazy[2*k+1], lazy[k]);
            lazy[2*k+2] = h(lazy[2*k+2], lazy[k]);
        }
        data[k] = g(data[k], w(lazy[k], len));
        lazy[k] = ei;
    }

    void _update(int a, int b, E x, int k, int l, int r) {
        eval(k, r-l);
        if (a <= l && r <= b) {
            lazy[k] = h(lazy[k], x);
            eval(k, r-l);
        } else if (a < r && l < b) {
            _update(a, b, x, 2*k+1, l, (l+r)/2);
            _update(a, b, x, 2*k+2, (l+r)/2, r);
            data[k] = f(data[2*k+1], data[2*k+2]);
        }
    }

    T _query(int a, int b, int k, int l, int r) {
        eval(k, r-l);
        if (r <= a || b <= l) {
            return ti;
        } else if (a <= l && r <= b) {
            return data[k];
        } else {
            T left = _query(a, b, 2*k+1, l, (l+r)/2);
            T right = _query(a, b, 2*k+2, (l+r)/2, r);
            return f(left, right);
        }
    }
public:
    // function<T(T, T)> f: Function to merge two elements
    // function<T(T, E)> g: Function to apply operator to element
    // function<E(E, E)> h: Function to merge two operators
    // function<E(E, int)> w: w(e1, len) = h(e1, e1, ..., e1)
    // T ti: Identity element of T
    // E ei: Identity element of E
    SegTree(F f, G g, H h, W w, T ti, E ei) : f(f), g(g), h(h), w(w), ti(ti), ei(ei) {}

    void init(int n) {
        N = 1;
        while (N < n) N *= 2;
        data.resize(2*N-1, ti);
        lazy.resize(2*N-1, ei);
    }

    void build(const vector<T> &v) {
        int n = v.size();
        init(n);
        for (int i = 0; i < n; i++) data[i+N-1] = v[i];
        for (int i = N-2; i >= 0; i--) {
            data[i] = f(data[2*i+1], data[2*i+2]);
        }
    }

    void update(int a, int b, E x) {
        _update(a, b, x, 0, 0, N);
    }

    T query(int a, int b) {
        return _query(a, b, 0, 0, N);
    }
};

// AOJ RSQ and RAQ
int main() {
    int N, Q;
    cin >> N >> Q;

    using T = long long;
    using E = long long;
    auto f = [](T t1, T t2) -> T { return t1 + t2; };
    auto g = [](T t, E e) -> T { return t + e; };
    auto h = [](E e1, E e2) -> E { return e1 + e2; };
    auto w = [](E e1, int len) -> E { return e1 * len; };
    T ti = 0;
    E ei = 0;
    auto seg = new SegTree<T, E>(f, g, h, w, ti, ei);
    seg->init(N);

    for (int q = 0; q < Q; q++) {
        int c;
        cin >> c;
        if (c) {
            int s, t;
            cin >> s >> t;
            s--; t--;
            cout << seg->query(s, t+1) << endl;
        } else {
            int s, t, x;
            cin >> s >> t >> x;
            s--; t--;
            seg->update(s, t+1, x);
        }
    }
}