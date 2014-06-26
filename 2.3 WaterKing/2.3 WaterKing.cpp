// 2.3 WaterKing.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <assert.h>

using namespace std;

typedef unsigned int USER_ID;

class Solution
{
public:
    USER_ID getWaterKingWithHalf(const vector<USER_ID> &IDs)
    {
        USER_ID kingID = 0;
        unsigned int count = 0;
        
        for (unsigned int i = 0; i < IDs.size(); i++)
        {
            if (count == 0)
            {
                kingID = IDs[i];
                count++;
                continue;
            }

            if (IDs[i] == kingID)
            {
                count++;
                continue;
            }

            count--;
        }

        assert(count > 0);
        return kingID;
    }

    void getWaterKingWithQuad(const vector<USER_ID> &IDs, vector<USER_ID> &result)
    {
        memset(kingIDArr, 0, KING_NUM * sizeof(USER_ID));
        memset(countArr, 0, KING_NUM * sizeof(unsigned int));

        for (unsigned int i = 0; i < IDs.size(); i++)
        {
            if (pushKingIDIfAvailable(IDs[i]))
                continue;

            TakeOneKingIDOff();
        }

        CheckFullResultDebug();

        for (int i = 0; i < KING_NUM; i++)
            result.push_back(kingIDArr[i]);
    }

private:
    static const unsigned int KING_NUM = 3;
    USER_ID kingIDArr[KING_NUM];
    unsigned int countArr[KING_NUM];

    bool pushKingIDIfAvailable(USER_ID userId)
    {
        //check whether there is same one
        for (int i = 0; i < KING_NUM; i++)
        {
            if (countArr[i] > 0 && kingIDArr[i] == userId)
            {
                countArr[i]++;
                return true;
            }
        }

        //check whether can be inserted as kingID
        for (int i = 0; i < KING_NUM; i++)
        {
            if (countArr[i] == 0)
            {
                kingIDArr[i] = userId;
                countArr[i]++;
                return true;
            }
        }

        return false;
    }

    void TakeOneKingIDOff()
    {
        for (int i = 0; i < KING_NUM; i++)
        {
            if (countArr[i] > 0)
                countArr[i]--;
        }
    }

    void CheckFullResultDebug()
    {
        for (int i = 0; i < KING_NUM; i++)
            assert(countArr[i] > 0);
    }
};


int _tmain(int argc, _TCHAR* argv[])
{
	return 0;
}

