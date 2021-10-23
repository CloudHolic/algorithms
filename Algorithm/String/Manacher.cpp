#include <iostream>
#include <vector>

using namespace std;

// Assume that sequence contains only positive integers.
vector<int> manacher(vector<int> origin)
{
    int radius = -1, center = -1;
    vector<int> seq, palindrome;
    const int n = origin.size(), len = 2 * n + 1;

    for (int i = 0; i < n; i++)
    {
        seq.push_back(-1);
        seq.push_back(origin[i]);
    }
    seq.push_back(-1);

    for (int i = 0; i < len; i++)
    {
        if (radius >= i)
            palindrome[i] = min(radius - i, palindrome[center * 2 - i]);
        else
            palindrome[i] = 0;

        while (i + palindrome[i] + 1 < len && i - palindrome[i] - 1 >= 0 && seq[i + palindrome[i] + 1] == seq[i - palindrome[i] - 1])
            palindrome[i]++;

        if (i + palindrome[i] > radius)
        {
            radius = i + palindrome[i];
            center = i;
        }
    }

    return palindrome;
}

// 'palindrome' should be palindrome array which can be calculated by the function 'manacher'.
bool check_palindrome(vector<int> palindrome, int start, int end)
{
    const int trans_s = 2 * start - 1;
    const int trans_e = 2 * end - 1;
    const int trans_center = (trans_s + trans_e) / 2;

    return trans_center - trans_s <= palindrome[trans_center];
}