#include <stdio.h>
#include <string.h>

int countPalindromicSubsequence(char* s)
{
    int len = strlen(s);
    int count = 0;

    for (char ch = 'a'; ch <= 'z'; ch++)
    {
        int first = -1, last = -1;

        for (int i = 0; i < len; i++)
        {
            if (s[i] == ch)
            {
                if (first == -1)
                    first = i;

                last = i;
            }
        }

        if (first != -1 && last - first >= 2)
        {
            int seen[26] = {0};

            for (int i = first + 1; i < last; i++)
            {
                seen[s[i] - 'a'] = 1;
            }

            for (int i = 0; i < 26; i++)
            {
                if (seen[i] == 1)
                    count++;
            }
        }
    }

    return count;
}
