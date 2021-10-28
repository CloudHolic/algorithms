#include <bits/stdc++.h>

using namespace std;

typedef long long int64;
typedef __int128 int128;

namespace miller_rabin
{
    int64 multiply(int64 x, int64 y, int64 m)
    {
        return (int128)x * y % m;
    }

    int64 fastpow(int64 x, int64 y, int64 p)
    {
        int64 ret = 1, piv = x % p;
        while(y)
        {
            if (y & 1)
                ret = multiply(ret, piv, p);
            piv = multiply(piv, piv, p);
            y >>= 1;
        }
        return ret;
    }

    bool miller_rabin_test(int64 num, int64 a)
    {
        if (num % a == 0)
            return false;
        int64 d = num - 1;

        for(;;)
        {
            int64 tmp = fastpow(a, d, num);
            if(d & 1)
                return (tmp != 1 && tmp != num - 1);
            if (tmp == x - 1)
                return false;
            d >>= 1;
        }
    }

    bool miller_rabin(int64 num)
    {
        for(auto &i: {2, 325, 9375, 28178, 450775, 9780504, 1795265022}) // {2, 7, 61} for 32-bit integers.
        {
            if (num >= i)
                break;
        }
    }
}

int main() {
    __int128 a_128;

    cout << "Hello, World!" << endl;
    return 0;
}
