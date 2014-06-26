// 2.4 NumberOfOnes.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>

using namespace std;

class Solution
{
public:
    //get the count of digit k in the range [0, N]
    unsigned int getSpecificDigitCount(unsigned int N, unsigned int k)
    {
        unsigned int curPosDigit = 0, lowerPosCount = 0, higherPosCount = 0;
        unsigned int curPosBase = 1;
        unsigned int totalCount = 0, curPosCount = 0;

        while (curPosBase <= N)
        {
            curPosDigit = (N / curPosBase) % 10;
            lowerPosCount = N % curPosBase;
            higherPosCount = N / (curPosBase * 10);

            if (curPosDigit < k)
                curPosCount = higherPosCount * curPosBase;
            else if (curPosDigit == k)
                curPosCount = higherPosCount * curPosBase + lowerPosCount + 1;
            else
                curPosCount = (higherPosCount + 1) * curPosBase;

            totalCount += curPosCount;
            curPosBase *= 10;
        }

        return totalCount;
    }
};

int _tmain(int argc, _TCHAR* argv[])
{
	return 0;
}

