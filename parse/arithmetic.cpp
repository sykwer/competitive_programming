// Source: https://gist.github.com/draftcode/1357281

#include <iostream>
#include <string>
#include <cctype>
using namespace std;

typedef string::const_iterator State;
class ParseError {};

/* BNF Notation
 * <expression> ::= <term> (+ | -) <term> (+ | -) ..
 * <term> ::= <factor> (* | /) <factor> (* | /) ..
 * <factor> ::= (<expression>) | <number>
 * <number> ::= ..
 */

void consume(State &begin, char expected) {
    if (*begin == expected) {
        begin++;
    } else {
        cerr << "Expected `" << expected << "` but got `" << *begin << "`" << endl;
        cerr << "Rest string is `";
        while (*begin) {
            cerr << *begin;
            begin++;
        }
        cerr << "“" << endl;

        throw ParseError();
    }
}

int expression(State &begin);
int term(State &begin);
int factor(State &begin);
int number(State &begin);

int expression(State &begin) {
    int ret = term(begin);

    while (true) {
        if (*begin == '+') {
            begin++;
            ret += term(begin);
        } else if (*begin == '-') {
            begin++;
            ret -= term(begin);
        } else {
            break;
        }
    }

    return ret;
}

int term(State &begin) {
    int ret = factor(begin);

    while (true) {
        if (*begin == '*') {
            begin++;
            ret *= factor(begin);
        } else if (*begin == '/') {
            begin++;
            ret /= factor(begin);
        } else {
            break;
        }
    }

    return ret;
}

int factor(State &begin) {
    if (*begin == '(') {
        begin++;
        int ret = expression(begin);
        begin++;
        return ret;
    } else {
        return number(begin);
    }
}

int number(State &begin) {
    int ret = 0;

    while (isdigit(*begin)) {
        ret *= 10;
        ret += *begin - '0';
        begin++;
    }

    return ret;
}

signed main() {
    int N;
    cin >> N;

    // discard newline char
    // std::istream::ignore(streamsize n = 1, int delim = EOF);
    cin.ignore();

    for (int i = 0; i < N; i++) {
        string s;
        getline(cin, s);

        State begin = s.begin();
        int ans = expression(begin);
        cout << ans << endl;
    }
}