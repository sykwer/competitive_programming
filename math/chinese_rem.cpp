#include <iostream>
#include <utility>
#include <vector>
using namespace std;

pair<long long, long long> chinese_rem(const vector<long long> &b, const vector<long long> &m) {
    auto ext_gcd = [](auto f, long long a, long long b, long long &p, long long &q) -> long long {
        if (b == 0) {
            p = 1;
            q = 0;
            return a;
        }
        long long d = f(f, b, a%b, q, p);
        q -= a/b * p;
        return d;
    };

    // Returns (0, -1) if there is no answer, otherwise (r, m) x ≡ r (mod.m).
    auto chinese_rem = [&](long long b1, long long m1, long long b2, long long m2) -> pair<long long, long long> {
        long long p, q;
        long long d = ext_gcd(ext_gcd, m1, m2, p, q);
        if ((b2 - b1) % d != 0) return make_pair(0, -1);
        long long lcm = m1 * (m2 / d);
        long long r = b1 + ((b2 - b1) / d) * m1 * p;
        r = (r % lcm + lcm) % lcm;
        return make_pair(r, lcm);
    };

    long long x = 0, mod = 1;
    for (int i = 0; i < (int)b.size(); i++) {
        pair<int, int> p = chinese_rem(x, mod, b[i], m[i]);
        x = p.first;
        mod = p.second;
    }
    return make_pair(x, mod);
}

int main() {
    vector<long long> b = {2, 3, 4};
    vector<long long> m = {3, 5, 7};
    auto p = chinese_rem(b, m);
    cout << p.first << " " << p.second << endl;
}