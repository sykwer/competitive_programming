#include <iostream>
using namespace std;

// Returns gcd(a, b). Set (x, y) where ax + by = gcd(a, b)
long long ext_gcd(long long a, long long b, long long &x, long long &y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    long long d = ext_gcd(b, a % b, y, x);
    y -= a / b * x;
    return d;
}

int main() {
    long long x, y;
    ext_gcd(111, 30, x, y);
    cout << x << ", " << y << endl;
}