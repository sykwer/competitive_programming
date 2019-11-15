#include <cstdint>
using namespace std;

const int MOD = 1e9 + 7;
using ll = long long;

template <uint_fast64_t Modulus> class mint {
    using u64 = uint_fast64_t;
    u64 a;
public:

    constexpr mint(const u64 a = 0) noexcept : a(a % Modulus) {}

    constexpr u64& value() noexcept {
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

    constexpr mint& operator /=(const mint& rhs) noexcept {
        u64 power = Modulus - 2;
        while (power) {
            if (power & 1u) {
                *this *= rhs;
            }

            rhs *= rhs;
            power >>= 1u;
        }

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

    constexpr mint operator /(const mint &rhs) const noexcept {
        return mint(*this) /= rhs;
    }
};

signed main() {
    return 0;
}
