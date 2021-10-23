#include <string>
#include <vector>

#pragma warning (disable: 4554)
#pragma warning (disable: 26451)

using namespace std;

#define MAX_SIZE 50000
#define set_bit(arr, x) (arr[x >> 6] |= 1LL << (x & 63))
#define get_bit(arr, x) ((arr[x >> 6] & 1LL << (x & 63)) != 0)

typedef unsigned long long uint64;

string str_a, str_b;
string answer;

short result_f[MAX_SIZE + 1], result_b[MAX_SIZE + 1];

uint64 subtract(uint64& x, const uint64 y)
{
    const uint64 temp = x;
    x = temp - y;
    return x > temp;
}

int count(uint64 num)
{
    uint64 v = num;
    int count = 0;
    for (; v; count++)
        v &= v - 1;

    return count;
}

// After storing str_a and str_b in global variable,
// Start with bit_hirschberg(1, str_a.length(), 1, str_b.length())
// And the result would be stored in 'answer'.
void bit_hirschberg(const int start_a, const int end_a, const int start_b, const int end_b)
{
    // 1. Initialize & Split str_a in half
    const int mid = (start_a + end_a) / 2;
    int len_a = 0, len_b = end_b - start_b + 1;
    const int block_size = (len_b >> 6) + 1;

    vector<uint64> cache_f(block_size, 0), cache_b(block_size, 0);
    vector<vector<uint64>> match(26, vector<uint64>(block_size, 0));
    vector<vector<uint64>> r_match(26, vector<uint64>(block_size, 0));

    result_f[start_b - 1] = result_b[end_b + 1] = 0;
    
    // 2. Forwarding DP
    string str_a_f = str_a.substr(start_a - 1, mid - start_a + 1);
    string str_b_f = str_b.substr(start_b - 1, len_b);
    len_a = static_cast<int>(str_a_f.size());

    for (int i = 0; i < len_b; i++)
        set_bit(match[str_b_f[i] - 'A'], i);
    
    for (int i = 0; i < len_a; i++)
    {
        uint64 shift_carry = 1, subtract_carry = 0;

        for (int k = 0; k < block_size; k++)
        {
            uint64 temp1 = match[str_a_f[i] - 'A'][k] | cache_f[k];

            const uint64 temp2 = cache_f[k] << 1 | shift_carry;
            shift_carry = cache_f[k] >> 63;

            uint64 temp3 = temp1;
            subtract_carry = subtract(temp3, subtract_carry);
            subtract_carry += subtract(temp3, temp2);

            cache_f[k] = temp1 ^ temp1 & temp3;
        }
    }

    for (int i = start_b; i <= end_b; i++)
        result_f[i] = result_f[i - 1] + get_bit(cache_f, i - start_b);

    // 3. Reverse DP
    string str_a_r = str_a.substr(mid, end_a - mid);
    string str_b_r = str_b.substr(start_b - 1, len_b);
    reverse(str_a_r.begin(), str_a_r.end());
    reverse(str_b_r.begin(), str_b_r.end());
    len_a = static_cast<int>(str_a_r.size());

    for (int i = 0; i < len_b; i++)
        set_bit(r_match[str_b_r[i] - 'A'], i);
    
    for (int i = 0; i < len_a; i++)
    {
        uint64 shift_carry = 1, subtract_carry = 0;

        for (int k = 0; k < block_size; k++)
        {
            uint64 temp1 = r_match[str_a_r[i] - 'A'][k] | cache_b[k];

            const uint64 temp2 = cache_b[k] << 1 | shift_carry;
            shift_carry = cache_b[k] >> 63;

            uint64 temp3 = temp1;
            subtract_carry = subtract(temp3, subtract_carry);
            subtract_carry += subtract(temp3, temp2);

            cache_b[k] = temp1 ^ temp1 & temp3;
        }
    }

    for (int i = end_b; i >= start_b; i--)
        result_b[i] = result_b[i + 1] + get_bit(cache_b, end_b - i);

    // 4. Find maximum index & answer
    int idx, max;

    if (result_f[end_b] > result_b[start_b])
    {
        idx = end_b;
        max = result_f[end_b];
    }
    else
    {
        idx = start_b - 1;
        max = result_b[start_b];
    }

    for (int i = start_b; i < end_b; i++)
    {
        const int cur = result_f[i] + result_b[i + 1];
        if (cur <= max)
            continue;

        max = cur;
        idx = i;
    }
    
    // 5. Check & record the answer and split into two problems.
    if (mid == start_a)
    {
        if (result_f[idx] > 0)
            answer.push_back(str_a[start_a - 1]);
    }
    else
        bit_hirschberg(start_a, mid, start_b, idx);

    if (mid < end_a)
        bit_hirschberg(mid + 1, end_a, idx + 1, end_b);
}