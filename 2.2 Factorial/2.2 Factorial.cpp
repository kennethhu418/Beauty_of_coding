// 2.2 Factorial.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <time.h>
#include <iostream>

using namespace std;

class Solution
{
public:
    unsigned int getZeroNum(unsigned int N)
    {
        if (N < 5)
            return 0;

        return getComponentNum(N, 5);    
    }

    unsigned int get1Pos(unsigned int N)
    {
        return getComponentNum(N, 2);
    }

private:
    unsigned int getComponentNum(unsigned int N, unsigned int component)
    {
        if (N < component)
            return 0;

        unsigned int count = 0;
        unsigned int base = component;
        while (base <= N)
        {
            count += N / base;
            base *= component;
        }

        return count;
    }
};


class Solution_Standard
{
public:
    unsigned int getZeroNum(unsigned int N)
    {
        unsigned _int64 factResult = getFactorial(N);
        if (factResult < 10)
            return 0;

        unsigned int count = 0;
        while (factResult)
        {
            if (factResult % 10 == 0)
            {
                count++;
                factResult /= 10;
                continue;
            }
            break;
        }

        return count;
    }

    unsigned int get1Pos(unsigned int N)
    {
        unsigned _int64 factResult = getFactorial(N);
        if (factResult < 2)
            return 0;

        unsigned int count = 0;
        while (factResult)
        {
            if ((factResult & 1) == 0)
            {
                count++;
                factResult >>= 1;
                continue;
            }
            break;
        }

        return count;
    }

    unsigned _int64 getFactorial(unsigned int N)
    {
        unsigned _int64 result = 1;
        while (N > 1)
        {
            result *= N;
            N--;
        }
        return result;
    }
};


int _tmain(int argc, _TCHAR* argv[])
{
    const unsigned int TRY_COUNT = 200;
    const unsigned int MAX_NUM = 20;
    unsigned int num, curTryCount = 0;
    unsigned int my_result, standard_result;
    unsigned _int64  fact_result;
    Solution so;
    Solution_Standard so_standard;

    srand(time(0));

    while (curTryCount < TRY_COUNT)
    {
        num = rand() % MAX_NUM + 1;

        my_result = so.get1Pos(num);
        standard_result = so_standard.get1Pos(num);
        fact_result = so_standard.getFactorial(num);

        if (my_result != standard_result)
        {
            cout << "Inconsistent 1 result with num " << num << endl;
            cout << "Fact is " << fact_result << endl;
            cout << my_result << " vs " << standard_result << endl;
            cout << "--------------------------------------------------------------------" << endl << endl;
        }
        
        my_result = so.getZeroNum(num);
        standard_result = so_standard.getZeroNum(num);
        if (my_result != standard_result)
        {
            cout << "Inconsistent 0 result with num " << num << endl;
            cout << "Fact is " << fact_result << endl;
            cout << my_result << " vs " << standard_result << endl;
            cout << "--------------------------------------------------------------------" << endl << endl;
        }

        
        curTryCount++;
    }

    system("PAUSE");
	return 0;
}

