#include <iostream>
using namespace std;

// O(log max(a, b))
int gcd(int a, int b) {
    if (b == 0) return a;
    return gcd(b, a % b);
}

int main() {
    cout << gcd(630, 300) << endl;  // 30
}