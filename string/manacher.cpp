#include <vector>
#include <string>
#include <iostream>
#include <math.h>
using namespace std;

// O(N)
vector<int> manacher(string &str) {
    vector<char> newstr(2 * str.size() - 1);
    for (int i = 0; i < newstr.size(); i++) {
        if (i % 2 == 0) {
            newstr[i] = str[i / 2];
        } else {
            newstr[i] = '.'; // means spaces between characters
        }
    }

    vector<int> cnt(newstr.size());

    int i = 0;
    int j = 0;

    while (i < newstr.size()) {
        while (i - j >= 0 && i + j < newstr.size() && newstr[i - j] == newstr[i + j]) j++;
        cnt[i] = j;

        int k = 1;
        while (i - k >= 0 && i + k < newstr.size() && k + cnt[i - k] < j) {
            cnt[i + k] = cnt[i - k];
            k++;
        }

        i += k;
        j -= k;
    }

    for (int i = 0; i < cnt.size(); i++) {
        if (i % 2 == 1) {
            cnt[i] = 2 * ceil(1.0 * (cnt[i] - 1) / 2);
        } else {
            int side = ceil(1.0 * cnt[i] / 2) - 1;
            cnt[i] = side * 2 + 1;
        }
    }

    return move(cnt);
}

signed main() {
    string str1 = "aabb";
    vector<int> cnt1 = manacher(str1);
    for (int i = 0; i < 2 * str1.size() - 1; i++) cout << cnt1[i];
    cout << endl; // 1210121 (各文字/文字間を中心とした最長の回文の直径)
}