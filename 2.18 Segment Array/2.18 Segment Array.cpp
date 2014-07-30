// 2.18 Segment Array.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <vector>

using namespace std;

class Solution
{
public:
    int     getMinHalfArraySumDiff(int arr[], int n)
    {
        if (n < 2)  return 0;
        int totalSum = 0;
        for (int i = 0; i < n; i++)
            totalSum += arr[i];
        maxHalfSum = (totalSum >> 1);

        AllocateMemory(n + 1, (n >> 1) + 1, maxHalfSum + 1);

        //Initialize DP
        resultArr[n][0][0] = 0;
       
        //Run DP
        for (int curPos = n - 1; curPos >= 0; --curPos)
        {
            for (int selNum = 0; selNum <= (n >> 1) && selNum <= (n - curPos); ++selNum)
            {
                for (int s = 0; s <= maxHalfSum; ++s)
                {
                    //if do not select current number
                    resultArr[curPos][selNum][s] = resultArr[curPos + 1][selNum][s];
                    //if select current number
                    if (selNum > 0 && s >= arr[curPos] && resultArr[curPos + 1][selNum - 1][s - arr[curPos]] >= 0)
                    {
                        if (arr[curPos] + resultArr[curPos + 1][selNum - 1][s - arr[curPos]] > resultArr[curPos][selNum][s])
                        {
                            resultArr[curPos][selNum][s] = arr[curPos] + resultArr[curPos + 1][selNum - 1][s - arr[curPos]];
                            selectArr[curPos][selNum][s] = true;
                        }
                    }
                }
            }
        }

        PartitionArray(arr, n);
        int result = totalSum - (resultArr[0][n>>1][maxHalfSum] << 1);
        FreeMemory();
        return result;
    }

private:
    int             ***resultArr;
    bool         ***selectArr; //bool value
    int             mSize;
    int             nSize;
    int             kSize;
    int             maxHalfSum;
    vector<int>     segArr1;
    vector<int>     segArr2;

    void    AllocateMemory(int m, int n, int k)
    {
        mSize = m;
        nSize = n;
        kSize = k;

        resultArr = new int**[m];
        for (int i = 0; i < m; i++)
        {
            resultArr[i] = new int*[n];
            for (int j = 0; j < n; j++)
            {
                resultArr[i][j] = new int[k];
                for (int p = 0; p < k; p++)
                    resultArr[i][j][p] = -1;
            }
        }

        selectArr = new bool**[m];
        for (int i = 0; i < m; i++)
        {
            selectArr[i] = new bool*[n];
            for (int j = 0; j < n; j++)
            {
                selectArr[i][j] = new bool[k];
                memset(selectArr[i][j], 0, sizeof(bool)* k);
            }
        }

        segArr1.clear();
        segArr2.clear();
    }

    void    FreeMemory()
    {
        for (int i = 0; i < mSize; i++)
        {
            for (int j = 0; j < nSize; j++)
            {
                delete[] resultArr[i][j];
                delete[] selectArr[i][j];
            }
            delete [] resultArr[i];
            delete[] selectArr[i];
        }
        delete[] resultArr;
        delete[] selectArr;
    }

    void PartitionArray(int *dest, int n)
    {
        int curPos = 0, curSum = maxHalfSum, curNum = (n >> 1);
        while (curPos < n)
        {
            if (selectArr[curPos][curNum][curSum])
            {
                segArr1.push_back(dest[curPos]);
                --curNum;   curSum -= dest[curPos];
            }
            else
                segArr2.push_back(dest[curPos]);
            ++curPos;
        }

        curPos = 0;
        for (int i = 0; i < segArr1.size(); i++)
            dest[curPos++] = segArr1[i];
        for (int i = 0; i < segArr2.size(); i++)
            dest[curPos++] = segArr2[i];
    }
};


int _tmain(int argc, _TCHAR* argv[])
{
    int     arr[] = {3, 6, 1, 9, 4, 7, 5, 4, 10, 13};
    int     size = sizeof(arr) / sizeof(int);
    Solution    so;

    int result = so.getMinHalfArraySumDiff(arr, size);


	return 0;
}

