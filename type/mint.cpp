#include <cstdint>
using namespace std;

const int MOD = 1e9 + 7;
using ll = long long;

template <long long Modulus> class mint {
    long long a;
public:

    constexpr mint(const long long a = 0) noexcept : a((a % Modulus + Modulus) % Modulus) {}

    constexpr long long& value() noexcept {
        return a;
    }

    constexpr mint& operator +=(const mint& rhs) noexcept {
        a += rhs.a;
        if (a >= Modulus) a -= Modulus;
        return *this;
    }

    constexpr mint& operator -=(const mint& rhs) noexcept {
        a += Modulus - rhs.a;
        if (a >= Modulus) a -= Modulus;
        return *this;
    }

    constexpr mint& operator *=(const mint& rhs) noexcept {
        a = a * rhs.a % Modulus;
        return *this;
    }

    constexpr mint operator +(const mint rhs) const noexcept {
        return mint(*this) += rhs;
    }

    constexpr mint operator -(const mint rhs) const noexcept {
        return mint(*this) -= rhs;
    }

    constexpr mint operator *(const mint &rhs) const noexcept {
        return mint(*this) *= rhs;
    }

    constexpr mint pow(long long t) const noexcept {
        if (!t) return 1;
        mint<Modulus> ret = pow(t >> 1u);
        ret *= ret;
        if (t & 1) ret *= *this;
        return ret;
    }

    constexpr mint inv() const noexcept {
        return pow(Modulus - 2);
    }

    constexpr mint operator /=(const mint rhs) {
        return (*this) *= rhs.inv();
    }

    constexpr mint operator /(const mint &rhs) const noexcept {
        return mint(*this) /= rhs;
    }
};

signed main() {
    return 0;
}
