#include <iostream>

long long binaryGCD(long long a, long long b, long long& x, long long& y) {
    if (a == 0) {
        x = 0;
        y = 1;
        return b;
    }

    long long x1, y1;
    long long d = binaryGCD(b % a, a, x1, y1);

    x = y1 - (b / a) * x1;
    y = x1;

    return d;
}

int main() {
    long long a, b;
    std::cin >> a >> b;

    long long x, y;
    long long d = binaryGCD(a, b, x, y);

    std::cout << d << " " << x << " " << y << std::endl;

    return 0;
}