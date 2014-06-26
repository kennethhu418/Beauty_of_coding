// 2.1 Count1s.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <time.h>
#include <iostream>

using namespace std;

class Solution
{
public:
    unsigned int getOneCount(int n)
    {
        if (n == 0) return 0;

        unsigned int count = 0;

        while (n)
        {
            count++;
            n &= (n - 1);
        }

        return count;
    }
};

class Solution_Standard
{
public:
    unsigned int getOneCount(int inputN)
    {
        unsigned int n = inputN;
        unsigned int count = 0;

        while (n)
        {
            if (n & 0x1)
                count++;
            n = n >> 1;
        }

        return count;
    }
};

int _tmain(int argc, _TCHAR* argv[])
{
    const int TRY_COUNT = 1000;
    const int MAX_NUM = 10000;
    int num, curTryCount = 0;
    Solution so;
    Solution_Standard so_standard;

    srand(time(NULL));

    while (curTryCount < TRY_COUNT)
    {
        num = rand() % (MAX_NUM + 1);
        if (so.getOneCount(num) != so_standard.getOneCount(num))
        {
            cout << "Inconsistent with num " << num << endl;
        }

        curTryCount++;
    }

    system("PAUSE");
	return 0;
}

