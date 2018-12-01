#include <vector>
#include <map>
#include <iostream>
#include <iterator>
#include <math.h>
using namespace std;

bool is_prime(int n) {
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) return false;
    }

    return n != 1;
}

vector<int> divisor(int n) {
    vector<int> ret;

    for (int i = 1; i * i <= n; i++) {
        if (n % i == 0) {
            ret.push_back(i);
            if (i != n / i) ret.push_back(n / i);
        }
    }

    return move(ret);
}

map<int, int> prime_factor(int n) {
    map<int, int> ret;
    for (int i = 2; i * i <= n; i++) {
        while (n % i == 0) {
            ret[i]++;
            n /= i;
        }
    }

    if (n != 1) ret[n] = 1;
    return move(ret);
};

// O(n * loglogn)
vector<int> sieve(int n) {
    vector<int> primes;

    vector<bool> prime_table(n+1, true);
    prime_table[0] = prime_table[1] = false;

    for (int i = 2; i <= n; i++) {
        if (prime_table[i]) {
            primes.push_back(i);
            for (int j = 2 * i; j <= n; j += i) prime_table[j] = false;
        }
    }

    return move(primes);
}

// [a, b)
vector<int> segment_sieve(int a, int b) {
    vector<int> primes;

    vector<bool> prime_table(b - a, true); // prime_table[i - a] = true <=> i is prime number
    vector<bool> sub_prime_table((int) ceil(sqrt(b)), true);

    for (int i = 2; i * i < b; i++) {
        if (sub_prime_table[i]) {
            for (int j = 2 * i; j * j < b; j += 1) sub_prime_table[j] = false;
            for (int j = max(2, (a + i - 1) / i) * i; j < b; j += i) {
                prime_table[j - a] = false;
            }
        }
    }

    for (int i = 0; i < b - a; i++) {
        if (prime_table[i]) primes.push_back(i + a);
    }

    return move(primes);
}

signed main() {
    map<int, int> :: iterator it;
    map<int, int> m = prime_factor(24);
    for (it = m.begin(); it != m.end(); it++) {
        cout << it->first << " : " << it->second << endl;
    }
}