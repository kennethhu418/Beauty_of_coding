// 1.5 Machine Failure.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <assert.h>
#include <iostream>

typedef unsigned __int64   DATA_ID;
#define FULL_ID_MASK    0xFFFFFFFFFFFFFFFF

class Solution
{
public:
    //Each data is stored on two machines. one machine is broken
    DATA_ID getMissingOneIDInPairs(DATA_ID *idArray, int n)
    {
        DATA_ID missingOne = 0;
        for (int i = 0; i < n; i++)
            missingOne ^= idArray[i];
        return missingOne;
    }

    //Each data is stored on two machines. Two machines are broken and the two broken machines store different IDs
    void getMissingTwoIDsInPairs(DATA_ID *idArray, int n, DATA_ID &data1, DATA_ID &data2)
    {
        DATA_ID  xorRes = 0;
        for (int i = 0; i < n; i++)
            xorRes ^= idArray[i];

        assert(xorRes > 0);
        DATA_ID mask = ((xorRes^(xorRes-1))>>1) + 1;        

        data1 = getMissingOneWithMask(idArray, n, mask);
        data2 = xorRes^data1;
    }

    //Each data is stored on three machines. One machine is residule
    DATA_ID getMissingOneIDInTripples(DATA_ID *idArray, int n)
    {
        DATA_ID ones = 0, twos = 0;
        getSetBitCountWithMask(idArray, n, ones, twos, FULL_ID_MASK);
        assert(twos == 0);
        return ones;
    }

    //Each data is stored on three machines. Two machines are residule and they store different IDs.
    void getMissingTwoIDsInTripples(DATA_ID *idArray, int n, DATA_ID &data1, DATA_ID &data2)
    {
        DATA_ID ones = 0, twos = 0;
        getSetBitCountWithMask(idArray, n, ones, twos, FULL_ID_MASK);
        assert(ones > 0);

        DATA_ID mask = ((ones ^ (ones - 1)) >> 1) + 1;
        DATA_ID newOnes = 0, newTwos = 0;
        getSetBitCountWithMask(idArray, n, newOnes, newTwos, mask);
        assert(newTwos == 0);

        data1 = newOnes;
        data2 = (~data1)&ones&(~twos) | data1&(~ones)&twos;
    }

    //Each data is stored on three machines. Three machines are residule and they store different IDs.
    void getMissingThreeIDsInTripples(DATA_ID *idArray, int n, DATA_ID &data1, DATA_ID &data2, DATA_ID &data3)
    {
        DATA_ID ones = 0, twos = 0;
        DATA_ID newOnes = 0, newTwos = 0;
        DATA_ID mask = 0;
        getSetBitCountWithMask(idArray, n, ones, twos, FULL_ID_MASK);

        if (ones > 0) //there is '1' bit in ones
        {
            mask = ((ones ^ (ones - 1)) >> 1) + 1;
            newOnes = 0;  newTwos = 0;
            getSetBitCountWithMask(idArray, n, newOnes, newTwos, mask);
            assert(newTwos == 0);            
            data1 = newOnes;
        }
        else
        {
            assert(twos > 0);
            mask = ((twos ^ (twos - 1)) >> 1) + 1;
            newOnes = 0;  newTwos = 0;
            getSetBitCountWithUnsetMask(idArray, n, newOnes, newTwos, mask);
            assert(newTwos == 0);
            data1 = newOnes;
        }

        //decrease ones and twos according to data1
        newOnes = ones&(~twos)&(~data1) | (~ones)&twos&data1;
        twos = (~ones)&twos&(~data1) | (~ones)&(~twos)&data1;
        ones = newOnes;

        mask = ((ones ^ (ones - 1)) >> 1) + 1;
        newOnes = 0;  newTwos = 0;
        getSetBitCountWithMask(idArray, n, newOnes, newTwos, mask);
        if ((data1 & mask) != 0)
        {
            DATA_ID tempOnes;
            //decrease ones and twos according to data1
            tempOnes = newOnes&(~newTwos)&(~data1) | (~newOnes)&newTwos&data1;
            newTwos = (~newOnes)&newTwos&(~data1) | (~newOnes)&(~newTwos)&data1;
            newOnes = tempOnes;
        }

        assert(newTwos == 0);
        data2 = newOnes;
        data3 = (~data2)&ones&(~twos) | data2&(~ones)&twos;
    }

private:
    inline DATA_ID getMissingOneWithMask(DATA_ID *idArray, int n, DATA_ID mask)
    {
        DATA_ID missingOne = 0;

        for (int i = 0; i < n; i++)
        {
            if ((idArray[i] & mask) == 0 && mask != FULL_ID_MASK)
                continue;
            missingOne ^= idArray[i];
        }

        return missingOne;
    }

    inline void getSetBitCountWithMask(DATA_ID *idArray, int n, DATA_ID &ones, DATA_ID &twos, DATA_ID mask)
    {
        DATA_ID newOnes = 0;
        ones = 0; twos = 0;

        for (int i = 0; i < n; i++)
        {
            if ((idArray[i] & mask) == 0 && mask != FULL_ID_MASK)
                continue;

            newOnes = (~ones)&(~twos)&idArray[i] | ones&(~twos)&(~idArray[i]);
            twos = ones&(~twos)&idArray[i] | (~ones)&twos&(~idArray[i]);
            ones = newOnes;
        }
    }

    inline void getSetBitCountWithUnsetMask(DATA_ID *idArray, int n, DATA_ID &ones, DATA_ID &twos, DATA_ID mask)
    {
        DATA_ID newOnes = 0;
        ones = 0; twos = 0;

        for (int i = 0; i < n; i++)
        {
            if ((idArray[i] & mask) != 0)
                continue;

            newOnes = (~ones)&(~twos)&idArray[i] | ones&(~twos)&(~idArray[i]);
            twos = ones&(~twos)&idArray[i] | (~ones)&twos&(~idArray[i]);
            ones = newOnes;
        }
    }
};

int _tmain(int argc, _TCHAR* argv[])
{
    using namespace std;

    Solution so;

    /***************Test pair machines************************/
    DATA_ID A[] = { 3, 3, 2, 2, 1, 9, 10, 11, 9, 12, 11, 10, 1 };
    DATA_ID B[] = { 3, 3, 2, 2, 8, 9, 10, 11, 9, 12, 11, 10};
    DATA_ID miss1, miss2, miss3;

    cout << so.getMissingOneIDInPairs(A, sizeof(A)/sizeof(DATA_ID)) << endl;

    so.getMissingTwoIDsInPairs(B, sizeof(B) / sizeof(DATA_ID), miss1, miss2);
    cout << miss1 << " "<< miss2  << endl;

    /***************Test three machines************************/

    DATA_ID C[] = { 3, 3, 3, 4, 4, 4, 5, 5, 5, 8 };
    DATA_ID D[] = { 3, 3, 3, 4, 4, 4, 5, 5, 5, 8, 12 };
    DATA_ID E[] = { 3, 3, 3, 19, 19, 19, 5, 5, 5, 8, 12, 15 };
    cout << "---------------------------------------------------------------------" << endl << endl;

    cout << so.getMissingOneIDInTripples(C, sizeof(C) / sizeof(DATA_ID)) << endl;
    so.getMissingTwoIDsInTripples(D, sizeof(D) / sizeof(DATA_ID), miss1, miss2);
    cout << miss1 << " " << miss2 << endl;
    so.getMissingThreeIDsInTripples(E, sizeof(E) / sizeof(DATA_ID), miss1, miss2, miss3);
    cout << miss1 << " " << miss2 << " "<< miss3 << endl;

	return 0;
}

