#include <bits/stdc++.h>

using namespace std;

typedef long long int64;
typedef __int128 int128;

namespace QuadraticResidue
{
    // Compute x * y mod m
    int64 multiply(int64 x, int64 y, int64 m)
    {
        return (int128)x * y % m;
    }

    // Compute n^k mod m.
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

    // Compute n^(-1) mod m.
    int64 inverse(int64 n, int64 m)
    {
        if (m == 1)
            return 0;

        int64 x_0 = 0, x_1 = 1, origin_m = m;
        while (n >1)
        {
            int64 q = n / m, t = m;

            m = n % m;
            n = t;
            t = x_0;
            x_0 = x_1 - q * x_0;
            x_1 = t;
        }

        // Make x_1 positive
        if (x_1 < 0)
            x_1 += origin_m;

        return x_1;
    }

    // Apply Chinese Remainder Theorem.
    // Find x such that
    // x = remainders[0] mod nums[0],
    // x = remainders[1] mod nums[1],
    // ... and so on.
    int64 chinese_remainder(vector<int64> nums, vector<int64> remainders)
    {
        int64 product = 1, result = 0;
        size_t size = nums.size();
        if (remainders.size() != size)
            return -1;

        for (int64 &num : nums)
            product *= num;

        // Apply above formula
        for (size_t i = 0; i < size; i++) {
            int64 pp = product / nums[i];
            result += multiply(multiply(remainders[i], inverse(pp, nums[i]), product), pp, product);
        }

        return result % product;
    }

    // Apply Hensel's Lifting.
    // Assume that f(x) = x^2 - n.
    // If we know x_1 such that f(x_1) = 0 mod p,
    // then find x_k such that f(x_k) = 0 mod p^k for given x, n, p, k.
    // Calculate recursively like x_(k+1) = x_k - f(x_k) * f'(x_1)^-1 mod p^(k+1).
    int64 hensel_lifting(int64 x, int64 n, int64 p, int64 k)
    {
        if (k == 1)
            return x;

        // f(x) = x^2 - n, f'(x) = 2x.
        auto function = [n, p](int64 x) -> int64 { return x * x - (n % p); };
        auto derivative = [](int64 x) -> int64 { return 2 * x; };

        if (derivative(x) % p != 0)
        {
            int64 result = x;
            int64 factor = 1;
            for (int i = 0; i < k; i++)
            {
                factor *= p;
                // Find modular inverse of f'(x_1) mod p^k
                int64 inv = inverse(derivative(x), factor);
                result = (result - function(result) * inv) % factor;

                if (result < 0)
                    result += factor;
            }

            return result;
        }

        return -1;
    }

    // Apply Tonelli-Shanks Algorithm.
    // Find an integer x such that x^2 = n mod p, where p is a prime number.
    int64 tonelli_shanks(int64 n, int64 p)
    {
        if (n == 0)
            return 0;

        if (n >= p)
            n %= p;

        if (p == 2)
            return n;

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

        // 6. The answer is r, p - r.
    }
}

namespace Cornacchia
{
    using namespace QuadraticResidue;

    // Find integers x, y such that x^2 + d * y^2 = n, where 1 <= d < m.
    pair<int64, int64> cornacchia(int64 d, int64 n)
    {
        // 1. Find r_0 such that r_0^2 = -d mod n.
        int64 possible = tonelli_shanks(n - d, n);        
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