#include <string>

using namespace std;

#define MAX_SIZE 50000
#define set_bit(arr, x) (arr[x >> 6] |= 1LL << (x & 63))

typedef unsigned long long uint64;

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

int bit_lcs_len(string& str_a, string& str_b)
{
    const int len_a = static_cast<int>(str_a.size()), len_b = static_cast<int>(str_b.size());
    const int block_size = (len_b >> 6) + 1;
    uint64 cache[(MAX_SIZE >> 6) + 1], match[26][(MAX_SIZE >> 6) + 1];

    for (int j = 0; j < len_b; j++)
        set_bit(match[str_b[j] - 'A'], j);

    for (int i = 0; i < len_a; i++)
    {
        uint64 shift_carry = 1, subtract_carry = 0;

        for (int k = 0; k < block_size; k++)
        {
            uint64 temp1 = match[str_a[i] - 'A'][k] | cache[k];

            const uint64 temp2 = cache[k] << 1 | shift_carry;
            shift_carry = cache[k] >> 63;

            uint64 temp3 = temp1;
            subtract_carry = subtract(temp3, subtract_carry);
            subtract_carry += subtract(temp3, temp2);

            cache[k] = temp1 ^ temp1 & temp3;
        }
    }

    int answer = 0;
    for (int i = 0; i < block_size; i++)
        answer += count(cache[i]);

    return answer;
}