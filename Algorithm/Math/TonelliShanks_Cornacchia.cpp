#include <bits/stdc++.h>

using namespace std;

typedef long long int64;
typedef __int128 int128;

namespace QuadraticResidue
{
    int64 multiply(int64 x, int64 y, int64 m)
    {
        return (int128)x * y % m;
    }

    int64 fast_pow(int64 n, int64 k, int64 m)
    {
        int64 result = 1, next_exp = n % m;
        while(k)
        {
            if (k & 1)
                result = multiply(result, next_exp, m);
            next_exp = multiply(next_exp, next_exp, m);
            k >>= 1;
        }
        return result;
    }

    // Find an integer x such that x^2 = n mod p.
    int64 tonelli_shanks(int64 n, int64 p)
    {
        if (n == 0)
            return 0;

        // 1. Check if n is quadratic residue in mod p.
        if (fast_pow(n, (p - 1) / 2, p) != 1)
            return -1;

        // 2. Find odd number q and non-negative integer s such that p - 1 = q * 2^s
        int64 q = p -1, s = 0;
        while(!(q & 1))
        {
            s++;
            q >>= 1;
        }

        // 3. Find quadratic non-residue integer z in mod p.
        int64 z;
        for(int64 i = 2; i < p; i++)
        {
            if (fast_pow(i, (p - 1) / 2, p) != 1)
            {
                z = i;
                break;
            }
        }

        // 4. Let m = s mod p, c = z^q mod p, t = n^q mod p, and r = n^((q + 1) / 2) mod p
        int64 m = s % p;
        int64 c = fast_pow(z, q, p);
        int64 t = fast_pow(n, q, p);
        int64 r = fast_pow(n, (q + 1) / 2, p);

        // 5. Repeat the following.
        // (1) if t = 0 -> return 0
        // (2) if t = 1 -> return r
        // (3) find the first integer i such that 0 < i < m and t^(2^i) = 1 mod p.
        // (4) let b = c^(2^(m - i - 1)) mod p
        // (5) m = i mod p, c = b^2 mod p, t = t * b^2 mod p, r = r * b mod p
        for( ; ; )
        {
            if (t == 0)
                return 0;
            if (t == 1)
                return r;

            int64 i;
            int64 temp = multiply(t, t, p);
            for(int64 j = 1; j < m; j++)
            {
                if (temp == 1)
                {
                    i = j;
                    break;
                }
                temp = multiply(temp, temp, p);
            }

            int64 b = fast_pow(c, fast_pow(2, m - i - 1, p - 1), p);

            m = i % p;
            c = multiply(b, b, p);
            t = multiply(t, c, p);
            r = multiply(r, b, p);
        }

        // 6. The answer is r, -r.
    }
}

namespace Cornacchia
{
    using namespace QuadraticResidue;

    // Find integers x, y such that x^2 + d * y^2 = n, where 1 <= d < m.
    pair<int64, int64> cornacchia(int64 d, int64 n)
    {
        // 1. Find r_0 such that r_0^2 = -d mod n.
        int64 sol = tonelli_shanks(n - d, n);
        vector<int64> possible;
        if(sol > 0)
            possible = {sol, n - sol};

        for(auto &r: possible)
        {
            if (r > n / 2)
                r = n - r;

            // 2. Let r_1 = n mod r_0
            int64 prev_r = r, prev_prev_r;
            r = n % r;

            // 3. For k > 1, let r_k = r_(k-2) mod r_(k-1)
            // Repeat this until r_k < sqrt n.
            auto root = static_cast<int64>(sqrt(n));
            while(r > root)
            {
                prev_prev_r = prev_r;
                prev_r = r;
                r = prev_prev_r % prev_r;
            }

            // 4. Check if s = sqrt((n - r_k^2) / d) is an integer.
            // If so, then x = r_k, and y = s. Otherwise, try with another r_0.
            int64 temp = n - r * r;
            if (temp % d != 0)
                continue;
            temp /= d;

            root = static_cast<int64>(sqrt(temp));
            if(root * root != temp)
                continue;

            return make_pair(r, root);
        }

        // No answer.
        return make_pair(-1, -1);
    }
}

int main()
{
    int64 n, p;
    cin >> n >> p;
    int64 result = TonelliShanks::tonelli_shanks(n, p);
    cout << result;

    return 0;
}