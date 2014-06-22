// 1.6 Drink Supply.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <vector>
#include <assert.h>
#include <algorithm>
#include <queue>

using namespace std;

typedef struct __DrinkRecord
{
    string name;
    unsigned int    volume;
    unsigned int    maxCount;
    unsigned int    satisefy;
}DrinkRecord;

typedef struct __ComboDrinkRecord
{
    unsigned int    volume; //0 to indicate invalid record
    unsigned int    satisefy;
    unsigned int    indexInSrcArray;
    vector<__ComboDrinkRecord>  recordArr; //can only be 0 or 2 size
}ComboDrinkRecord;

class Solution_DP
{
public:
    Solution_DP()
    {
        satisfyArr = satisfyArrangeArr = NULL;
        buffer = NULL;
    };

    unsigned int getMaxSatisfySupplyArrangement(const vector<DrinkRecord> &recordArr, int maxVolume, unsigned int *arrangeArr)
    {
        int n = recordArr.size();
        if (n == 0)
            return 0;

        AllocateMemory(n + 1, maxVolume + 1);

        //Initialize DP array
        //No need to do explicit initialization here because in
        //AllocateMemory we have already zeroed the heap
        //for (int i = 0; i <= maxVolume; i++)
        //{
        //    satisfyArr[n][i] = 0;
        //    satisfyArrangeArr[n][i] = 0;
        //}

        int nextTotalVolume = 0;
        int maxSatisfy = 0, curSatisfy = 0, maxSatisfyCnt = 0;

        //Do DP now
        for (int curDrink = n - 1; curDrink >= 0; curDrink--)
        {
            for (int curTotalVolume = 0; curTotalVolume <= maxVolume; curTotalVolume++)
            {
                maxSatisfy = 0;
                maxSatisfyCnt = 0;

                for (int curDrinkCnt = 0; curDrinkCnt <= recordArr[curDrink].maxCount; curDrinkCnt++)
                {
                    if (curDrinkCnt*recordArr[curDrink].volume > curTotalVolume)
                        break;

                    nextTotalVolume = curTotalVolume - curDrinkCnt*recordArr[curDrink].volume;
                    curSatisfy = curDrinkCnt*recordArr[curDrink].satisefy + satisfyArr[curDrink + 1][nextTotalVolume];
                    if (maxSatisfy < curSatisfy)
                    {
                        maxSatisfy = curSatisfy;
                        maxSatisfyCnt = curDrinkCnt;
                    }
                }

                satisfyArr[curDrink][curTotalVolume] = maxSatisfy;
                satisfyArrangeArr[curDrink][curTotalVolume] = maxSatisfyCnt;
            }
        }

        //save final result
        maxSatisfy = satisfyArr[0][maxVolume];
        nextTotalVolume = maxVolume;
        for (int curDrink = 0; curDrink < n; curDrink++)
        {
            arrangeArr[curDrink] = satisfyArrangeArr[curDrink][nextTotalVolume];
            nextTotalVolume -= (arrangeArr[curDrink]*recordArr[curDrink].volume);
            assert(nextTotalVolume >= 0);
        }

        FreeMemory();
        return maxSatisfy;
    }

private:
    unsigned int **satisfyArr;
    unsigned int **satisfyArrangeArr;
    char* buffer;

    void AllocateMemory(int n, int v)
    {
        int singleLineDataSize = sizeof(unsigned int)*v;
        int pointerArrSize = sizeof(unsigned int*)*n;
        buffer = new char[2 * (singleLineDataSize*n + pointerArrSize)];
        memset(buffer, 0, 2 * (singleLineDataSize*n + pointerArrSize));

        satisfyArr = (unsigned int **)(buffer + singleLineDataSize*n);
        satisfyArrangeArr = (unsigned int **)(buffer + 2 * singleLineDataSize*n + pointerArrSize);
        for (int i = 0; i < n; i++)
            satisfyArr[i] = (unsigned int*)(buffer + singleLineDataSize*i);
        for (int i = 0; i < n; i++)
            satisfyArrangeArr[i] = (unsigned int*)(buffer + singleLineDataSize*n + pointerArrSize + singleLineDataSize*i);
    }

    void FreeMemory()
    {
        delete[] buffer;
    }
};


int drinkRecordCompare(const ComboDrinkRecord &a, const ComboDrinkRecord &b){
    if (a.volume < b.volume)
        return true;
    if (a.volume > b.volume)
        return false;

    return a.satisefy > b.satisefy;
}

class Solution_Combo
{
public:
    Solution_Combo()
    {
    };

    unsigned int getMaxSatisfySupplyArrangement(const vector<DrinkRecord> &recordArr, int maxVolume, unsigned int *arrangeCntResult)
    {
        ComboDrinkRecord curRecord;
        curBitRecordCollection = &collection1;
        nextBitRecordCollection = &collection2;
        this->drinkTypeCount = recordArr.size();

        for (int i = 0; i < drinkTypeCount; i++)
        {
            curRecord.volume = recordArr[i].volume;
            curRecord.satisefy = recordArr[i].satisefy;
            curRecord.indexInSrcArray = i;
            totalCollection.push_back(curRecord);
        }

        nextComboPos = 0;
        arrangeArrSize = 0;
        sort(totalCollection.begin(), totalCollection.end(), drinkRecordCompare);
        memset(arrangeArr, 0, 32 * sizeof(ComboDrinkRecord));

        unsigned int curBitMask = 0x1;
        unsigned int totalSatisfy = 0;
        vector<ComboDrinkRecord>    *tempCollection = NULL;

        while (curBitMask <= maxVolume)
        {
            addCurrentBitRecordsWithSort(curBitMask);

            if (curBitRecordCollection->size() == 0)
            {
                curBitMask = curBitMask << 1;
                arrangeArrSize++;
                continue;
            }

            if ((curBitMask & maxVolume) != 0)
            {
                curRecord = takeOutFirstRecord();
                arrangeArr[arrangeArrSize] = curRecord;
            }

            generateNextBitCollection();
            curBitRecordCollection->clear(); //clear the residule 1 record
            tempCollection = curBitRecordCollection;
            curBitRecordCollection = nextBitRecordCollection;
            nextBitRecordCollection = tempCollection;

            curBitMask = curBitMask << 1;
            arrangeArrSize++;
        }

        totalSatisfy = FormFinalResult(recordArr, arrangeCntResult);
        
        CleanData();//collection1, collection2, queueToTraverseTree, totalCollection to clear
        return totalSatisfy;
    }

private:
    vector<ComboDrinkRecord>    collection1;
    vector<ComboDrinkRecord>    collection2;
    vector<ComboDrinkRecord>    *curBitRecordCollection;
    vector<ComboDrinkRecord>    *nextBitRecordCollection;
    queue<ComboDrinkRecord>     queueToTraverseTree;
    ComboDrinkRecord            arrangeArr[32];
    int arrangeArrSize;
    int drinkTypeCount;
    
    vector<ComboDrinkRecord>    totalCollection;
    int nextComboPos;

    inline void addCurrentBitRecordsWithSort(unsigned int targetVolume)
    {
        if (nextComboPos >= totalCollection.size())
            return;
        if (totalCollection[nextComboPos].volume > targetVolume)
            return;

        while (nextComboPos < totalCollection.size() && totalCollection[nextComboPos].volume == targetVolume)
            curBitRecordCollection->push_back(totalCollection[nextComboPos++]);

        sort(curBitRecordCollection->begin(), curBitRecordCollection->end(), drinkRecordCompare);
    }

    inline ComboDrinkRecord takeOutFirstRecord()
    {
        ComboDrinkRecord retRecord = curBitRecordCollection->at(0);
        curBitRecordCollection->at(0).volume = curBitRecordCollection->at(0).satisefy = 0;
        return retRecord;
    }

    inline void generateNextBitCollection()
    {
        ComboDrinkRecord newRecord;
        unsigned int n = curBitRecordCollection->size();
        for (unsigned int i = 0; i < n; i += 2)
        {
            if (curBitRecordCollection->at(i).volume == 0)
                continue;
            if (i == n - 1)
                break;
            newRecord.volume = curBitRecordCollection->at(i).volume << 1;
            newRecord.satisefy = curBitRecordCollection->at(i).satisefy + curBitRecordCollection->at(i+1).satisefy;
            newRecord.indexInSrcArray = 0x7FFFFFFF;
            newRecord.recordArr.push_back(curBitRecordCollection->at(i));
            newRecord.recordArr.push_back(curBitRecordCollection->at(i+1));
            nextBitRecordCollection->push_back(newRecord);
            newRecord.recordArr.clear();
        }
    }

    unsigned int FormFinalResult(const vector<DrinkRecord> &srcRecordArr, unsigned int *arrangeCntResult)
    {
        unsigned int totalSatisfy = 0;
        ComboDrinkRecord    curRecord;

        memset(arrangeCntResult, 0, sizeof(unsigned int)*drinkTypeCount);
        for (int i = 0; i < arrangeArrSize; i++)
        {
            if (arrangeArr[i].volume == 0)
                continue;
            
            queueToTraverseTree.push(arrangeArr[i]);
            while (!queueToTraverseTree.empty())
            {
                curRecord = queueToTraverseTree.front();
                queueToTraverseTree.pop();

                if (curRecord.recordArr.size() == 0)
                {
                    totalSatisfy += srcRecordArr[curRecord.indexInSrcArray].satisefy;
                    arrangeCntResult[curRecord.indexInSrcArray]++;
                    continue;
                }

                assert(curRecord.recordArr.size() == 2);
                queueToTraverseTree.push(curRecord.recordArr[0]);
                queueToTraverseTree.push(curRecord.recordArr[1]);
            }
        }

        return totalSatisfy;
    }

    inline void CleanData()
    {
        collection1.clear();
        collection2.clear();
        totalCollection.clear();
    }
};


int _tmain(int argc, _TCHAR* argv[])
{
	return 0;
}

