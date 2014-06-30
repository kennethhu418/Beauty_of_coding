// 2.7 Greatest Common Divisor.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

class Solution
{
public:
    int gcd(int x, int y)
    {
        if (x == y)
            return x;
        
        if (x > y)
        {
            x ^= y;
            y ^= x;
            x ^= y;
        }

        if (x == 0)
            return y;

        if ((x & 1) && (y & 1))
            return gcd(x, y - x);

        if ((x & 1) == 0 && (y & 1) == 0)
            return gcd(x >> 1 , y >> 1);

        if (x & 1)
            return gcd(x, y >> 1);

        return gcd(x >> 1, y);
    }
};

int _tmain(int argc, _TCHAR* argv[])
{
	return 0;
}

