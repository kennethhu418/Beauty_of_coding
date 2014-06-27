// 2.5 FindKLargestNumber.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <vector>
#include <algorithm>
#include <assert.h>

using namespace std;

class Solution
{
public:
    vector<int>  findKLargestQuickSortLike(vector<int> &srcArray, int K)
    {
        result.clear();
        unsigned int n = srcArray.size();
        if (n == 0)
            return result;
        if (n == 1)
        {
            result = srcArray;
            return result;
        }

        int curRangeStart = 0, curRangeEnd = n - 1, finalPos = -1;
        int pivot = 0;
        while (curRangeStart <= curRangeEnd)
        {
            pivot = srcArray[(curRangeStart + curRangeEnd) / 2];
            int start = curRangeStart, end = curRangeEnd;
            swap(srcArray[(curRangeStart + curRangeEnd) / 2], srcArray[curRangeEnd]);

            while (start < end)
            {
                while(start < end && srcArray[start] > pivot)
                    start++;
                if (start == end)
                    break;

                srcArray[end] = srcArray[start];

                while (start < end && srcArray[end] <= pivot)
                    end--;
                if (start == end)
                    break;

                srcArray[start] = srcArray[end];
            }

            srcArray[start] = pivot;

            if (start - curRangeStart + 1 == K)
            {
                finalPos = start;
                break;
            }

            if (start - curRangeStart + 1 > K)
            {
                curRangeEnd = start - 1;
                continue;
            }

            curRangeStart = start + 1;
            K -= (start - curRangeStart + 1);
        }

        assert(finalPos >= 0);
        for (unsigned int i = 0; i <= finalPos; i++)
            result.push_back(srcArray[i]);
        return result;
    }

private:
    vector<int> result;
};

int _tmain(int argc, _TCHAR* argv[])
{
	return 0;
}

