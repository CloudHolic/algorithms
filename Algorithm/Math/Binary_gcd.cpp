#include <bits/stdc++.h>

using namespace std;

typedef long long int64;
typedef __int128 int128;

int64 binary_gcd(int64 x, int64 y)
{
    if (x == 0)
        return y;
    if (y == 0)
        return x;

    int shift = __builtin_ctz(x | y);
    x >>= __builtin_ctz(x);

    do {
        y >>= __builtin_ctz(y);
        if (x > y)
            swap(x, y);
        y -= x;
    } while (y != 0);

    return x << shift;
}