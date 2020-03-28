#include <cstdint>
using namespace std;

const int MOD = 1e9 + 7;
using ll = long long;

template <long long Modulus> class ModInt {
    long long a;
public:

    constexpr ModInt(const long long a = 0) noexcept : a((a % Modulus + Modulus) % Modulus) {}

    constexpr long long& value() noexcept {
        return a;
    }

    constexpr ModInt& operator +=(const ModInt& rhs) noexcept {
        a += rhs.a;
        if (a >= Modulus) a -= Modulus;
        return *this;
    }

    constexpr ModInt& operator -=(const ModInt& rhs) noexcept {
        a += Modulus - rhs.a;
        if (a >= Modulus) a -= Modulus;
        return *this;
    }

    constexpr ModInt& operator *=(const ModInt& rhs) noexcept {
        a = a * rhs.a % Modulus;
        return *this;
    }

    constexpr ModInt operator +(const ModInt rhs) const noexcept {
        return ModInt(*this) += rhs;
    }

    constexpr ModInt operator -(const ModInt rhs) const noexcept {
        return ModInt(*this) -= rhs;
    }

    constexpr ModInt operator *(const ModInt &rhs) const noexcept {
        return ModInt(*this) *= rhs;
    }

    constexpr ModInt pow(long long t) const noexcept {
        if (!t) return 1;
        ModInt<Modulus> ret = pow(t >> 1u);
        ret *= ret;
        if (t & 1) ret *= *this;
        return ret;
    }

    constexpr ModInt inv() const noexcept {
        return pow(Modulus - 2);
    }

    constexpr ModInt operator /=(const ModInt rhs) {
        return (*this) *= rhs.inv();
    }

    constexpr ModInt operator /(const ModInt &rhs) const noexcept {
        return ModInt(*this) /= rhs;
    }
};

signed main() {
    return 0;
}
