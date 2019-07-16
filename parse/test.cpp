#include <iostream>
using namespace std;

int main() {
    cout << "enter your name" << endl;

    char first, last;
    first = cin.get();
    cin.ignore(256, ' ');
    last = cin.get();

    cout << "initial is " << first << last << endl;

    return 0;
}