// 1.4 Buy Books.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include <unordered_map>
#include <algorithm>
#include <assert.h>
#include <iostream>
#include <time.h>
#include <windows.h>

using namespace std;

#define MAX_BOOK_KINDS  20

typedef struct __SingleBookPermutation
{
    int      bookCount[MAX_BOOK_KINDS];
    int      bookKinds;
}SingleBookPermutation;

typedef struct __SingleBookPermutationHash
{
    size_t operator()(const SingleBookPermutation &item) const
    {
        unsigned int hash = 0x4E67C6A7;
        for (int i = 0; i < item.bookKinds; i++)
        {
            hash ^= ((hash << 5) + item.bookCount[i] + (hash >> 2));
        }

        return (hash & 0x7FFFFFFF);
    }
}SingleBookPermutationHash;

typedef struct __SingleBookPermutationCompare
{
    bool operator()(const SingleBookPermutation &a, const SingleBookPermutation &b) const
    {
        if (a.bookKinds != b.bookKinds)
            return false;

        for (int i = 0; i < a.bookKinds; i++)
        {
            if (a.bookCount[i] != b.bookCount[i])
                return false;
        }

        return true;
    }
}SingleBookPermutationCompare;


class Solution
{
public:
    Solution(int kindsOfBooks, double bookPrice, const double *discountsArray)
    {
        this->mBookKinds = kindsOfBooks;
        this->bookPrice = bookPrice;
        this->discountsArray = discountsArray;
        this->orgBookCountArray = NULL;

        if (kindsOfBooks > 0)
            this->orgBookCountArray = new int[kindsOfBooks];
    }

    ~Solution()
    {
        if (orgBookCountArray)
            delete[] orgBookCountArray;
    }

    double getMinimumPrices(int* inputBookCountArr)
    {
        if (inputBookCountArr == NULL)
            return 0;

        if (mBookKinds == 1)
        {
            return inputBookCountArr[0] * bookPrice*discountsArray[0];
        }

        bookCountArray = inputBookCountArr;

        sort(bookCountArray, bookCountArray + mBookKinds);
        memcpy(this->orgBookCountArray, bookCountArray, sizeof(int)*mBookKinds);

        //Initialize totalPriceArray
        SingleBookPermutation sp;
        memset(sp.bookCount, 0, sizeof(int)*mBookKinds);
        sp.bookKinds = mBookKinds;
        minTotalPriceArray[sp] = 0;

        for (int i = 1; i <= bookCountArray[mBookKinds - 1]; i++)
        {
            sp.bookCount[mBookKinds - 1] = i;
            minTotalPriceArray[sp] = i*bookPrice*discountsArray[0];
        }

        generateTotalPriceArray(0, 0);

        memcpy(sp.bookCount, orgBookCountArray, sizeof(int)*mBookKinds);
        assert(minTotalPriceArray.find(sp) != minTotalPriceArray.end());
        double result = minTotalPriceArray[sp];
        minTotalPriceArray.clear();
        return result;
    }

private:
    int     mBookKinds;
    double bookPrice;
    const double* discountsArray;
    int*   bookCountArray;
    int*   orgBookCountArray;
    //array used to save results during the dynamic programming. This array is actually a map, which simulates a
    //mBookKinds dimentional array.
    unordered_map<SingleBookPermutation, double, SingleBookPermutationHash, SingleBookPermutationCompare>    minTotalPriceArray;

    //this recursive function simulates the mBookKinds level "for" loop
    void generateTotalPriceArray(int curBookKind, int startCount)
    {
        if (curBookKind == mBookKinds - 1 && bookCountArray[curBookKind - 1] == 0)
            return; //already calculated

        if (curBookKind < mBookKinds - 1)
        {
            for (int curPosBookCount = startCount; curPosBookCount <= orgBookCountArray[curBookKind]; curPosBookCount++)
            {
                bookCountArray[curBookKind] = curPosBookCount;
                generateTotalPriceArray(curBookKind + 1, curPosBookCount);
            }
            return;
        }

        for (int curPosBookCount = startCount; curPosBookCount <= orgBookCountArray[curBookKind]; curPosBookCount++)
        {
            bookCountArray[curBookKind] = curPosBookCount;
            getCurPermutationResult();
        }    
    }

    inline void getCurPermutationResult() //when calling this function, bookCountArray is sorted by book count already
    {
        int startBookKind = -1;
        for (int i = 0; i < mBookKinds; i++)
        {
            if (bookCountArray[i] > 0)
            {
                startBookKind = i;
                break;
            }
        }

        if (startBookKind == -1)
            return;

        SingleBookPermutation sp;
        sp.bookKinds = mBookKinds;
        double minPrices = 999999999, curMinPrices;

        for (int i = startBookKind; i < mBookKinds; i++)
        {
            memcpy(sp.bookCount, bookCountArray, sizeof(int)*mBookKinds);

            for (int j = i; j < mBookKinds; j++)
                sp.bookCount[j]--; //take one book off
            sort(sp.bookCount, sp.bookCount + mBookKinds);

            assert(minTotalPriceArray.find(sp) != minTotalPriceArray.end());
            curMinPrices = (mBookKinds - i)*bookPrice*discountsArray[mBookKinds - i - 1] + minTotalPriceArray[sp];

            if (curMinPrices < minPrices)
                minPrices = curMinPrices;
        }

        memcpy(sp.bookCount, bookCountArray, sizeof(int)*mBookKinds);
        minTotalPriceArray[sp] = minPrices;
    }
};

class Solution_Brute_Force
{
public:
    Solution_Brute_Force(int bookKinds, double bookPrice, const double* discountArr)
    {
        this->mBookKinds = bookKinds;
        this->mBookPrice = bookPrice;
        this->mDiscountArr = discountArr;

        calculateIndexCombineArray();
    }

    ~Solution_Brute_Force()
    {
        for (int i = 0; i <= mBookKinds; i++)
            delete[] indexCombineArr[i];
        delete[] indexCombineArr;
    }

    double getMinimumPrices(int* inputBookCountArr)
    {
        if (inputBookCountArr == NULL)
            return 0;

        if (mBookKinds == 1)
            return inputBookCountArr[0] * mBookPrice*mDiscountArr[0];

        SingleBookPermutation sp;
        sp.bookKinds = mBookKinds;
        memcpy(sp.bookCount, inputBookCountArr, sizeof(int)*mBookKinds);
        sort(sp.bookCount, sp.bookCount + sp.bookKinds);

        //Initialize minTotalPriceArray
        SingleBookPermutation spForInit;
        spForInit.bookKinds = mBookKinds;
        memset(spForInit.bookCount, 0 , sizeof(int)*mBookKinds);
        minTotalPriceArray[spForInit] = 0;
        for (int i = 1; i <= sp.bookCount[mBookKinds - 1]; i++)
        {
            spForInit.bookCount[mBookKinds - 1] = i;
            minTotalPriceArray[spForInit] = i*mBookPrice*mDiscountArr[0];
        }

        double result = getMinimumPrices(sp);
        minTotalPriceArray.clear();
        return result;
    }

private:
    int mBookKinds;
    double mBookPrice;
    const double* mDiscountArr;
    vector<vector<int>> **indexCombineArr;
    //map used to save results we already calculated, which avoid us doing repeat calculations. Please note that in this brute force
    //approach, this map is not used as a mBookKinds dimentional array.
    unordered_map<SingleBookPermutation, double, SingleBookPermutationHash, SingleBookPermutationCompare>    minTotalPriceArray;

    double getMinimumPrices(SingleBookPermutation sp)
    {
        sort(sp.bookCount, sp.bookCount + sp.bookKinds);
        if (minTotalPriceArray.find(sp) != minTotalPriceArray.end())
            return minTotalPriceArray[sp];

        int startBookKind = -1;
        for (int i = 0; i < mBookKinds; i++)
        {
            if (sp.bookCount[i] > 0)
            {
                startBookKind = i;
                break;
            }
        }

        if (startBookKind == -1)
            return 0;

        int maxCombineCalcBookCount = mBookKinds - startBookKind;
        vector<vector<int>> bookCombinations;
        int combSize;
        double curPrice = 0, minPrices = 999999999;

        for (int curCombineBookCount = 1; curCombineBookCount <= maxCombineCalcBookCount; curCombineBookCount++)
        {
            const vector<vector<int>> &bookCombinations = this->indexCombineArr[startBookKind][curCombineBookCount];
            combSize = bookCombinations.size();
            for (int i = 0; i < combSize; i++)
            {
                //take one book off from the combination
                for (int j = 0; j < curCombineBookCount; j++)
                    sp.bookCount[bookCombinations[i][j]]--;

                curPrice = curCombineBookCount*mBookPrice*mDiscountArr[curCombineBookCount - 1] + getMinimumPrices(sp);
                if (curPrice < minPrices)
                    minPrices = curPrice;

                //resume the original array.
                for (int j = 0; j < curCombineBookCount; j++)
                    sp.bookCount[bookCombinations[i][j]]++;
            }
        }

        minTotalPriceArray[sp] = minPrices;
        return minPrices;
    }

    inline void calculateIndexCombineArray()
    {
        indexCombineArr = new vector<vector<int>>*[mBookKinds + 1]; //combResArr[i][k]: starting from pos i with k num selected
        for (int i = 0; i <= mBookKinds; i++)
            indexCombineArr[i] = new vector<vector<int>>[mBookKinds + 1];

        vector<int> emptySingleRes;
        for (int i = 0; i <= mBookKinds; i++)
            indexCombineArr[i][0].push_back(emptySingleRes);

        for (int i = mBookKinds - 1; i >= 0; i--)
        {
            for (int selNum = 1; selNum <= mBookKinds - i; selNum++)
            {
                //not include current number
                indexCombineArr[i][selNum] = indexCombineArr[i + 1][selNum];
                //include current number
                combineResult(indexCombineArr[i][selNum], i, indexCombineArr[i + 1][selNum - 1]);
            }
        }
    }

    inline void combineResult(vector<vector<int>> &dest, int curNum, const vector<vector<int>> &src)
    {
        int n = src.size();
        vector<int> singleRes;
        if (n == 0)
        {
            singleRes.push_back(curNum);
            dest.push_back(singleRes);
            return;
        }

        for (int i = 0; i < n; i++)
        {
            singleRes.push_back(curNum);
            for (int j = 0; j < src[i].size(); j++)
                singleRes.push_back(src[i][j]);
            dest.push_back(singleRes);
            singleRes.clear();
        }
    }
};

inline void generateRandomArray(int* dest, int n, int max_val)
{
    max_val++;
    for (int i = 0; i < n; i++)
        dest[i] = rand() % max_val;
}

inline void OutputArray(int dest[], int n)
{
    for (int i = 0; i < n; i++)
        cout << dest[i] << " ";
}

int _tmain(int argc, _TCHAR* argv[])
{
    const int BOOK_KINDS = 7; //seven different books
    const double BOOK_PRICE = 8.5; //each book's price
    const double DISCOUNT_ARRAY[] = {1, 0.96, 0.9, 0.82, 0.73, 0.68, 0.60}; //discount when buying several different book together
    const double ERROR_LIMIT = 0.00001;
    const int MAX_BOOK_COUNT = 15; //Maximum count of each book you can buy
    const int EXPERIMENT_TIMES = 100;

    int bookCountArr[BOOK_KINDS] = {7, 3, 8, 3, 1, 10, 9};
    char inputFlag[100];
    int curExperimentTime = 0;
    srand(time(NULL));

    SetThreadAffinityMask(GetCurrentThread(),  0x1); //affine this program to core 1 to fully leverage L1 cache ability

    /*******************************************************
                                Output Parameters first
    *******************************************************/
    cout << "There are " << BOOK_KINDS << " different kinds of book." << endl;
    cout << "Each book's price is " << BOOK_PRICE << endl;
    for (int i = 0; i < BOOK_KINDS; i++)
        cout << "If you buy "<< i + 1 << " book(s) together, you get a discount of "<<100 - DISCOUNT_ARRAY[i]*100<<"%."<< endl;

    Solution so(BOOK_KINDS, BOOK_PRICE, DISCOUNT_ARRAY);
    Solution_Brute_Force so_brute(BOOK_KINDS, BOOK_PRICE, DISCOUNT_ARRAY);
    double brute_result, our_result;

    while (curExperimentTime < EXPERIMENT_TIMES)
    {
        cout << "Book count array is: ";
        OutputArray(bookCountArr, BOOK_KINDS);
        cout << endl;

        brute_result = so_brute.getMinimumPrices(bookCountArr);
        our_result = so.getMinimumPrices(bookCountArr);
        cout << "Standard Answer: " << brute_result << endl;
        cout << "Our Answer: " <<  our_result << endl;
        if (brute_result - our_result < -ERROR_LIMIT || brute_result - our_result > ERROR_LIMIT)
            MessageBoxA(NULL, "Our answer is different with standard anwer!", "ERROR", MB_OK | MB_ICONWARNING);

        cout << "----------------------------------------------------------------------------------------------" << endl << endl;
        curExperimentTime++;
        generateRandomArray(bookCountArr, BOOK_KINDS, MAX_BOOK_COUNT);
    }

	return 0;
}

