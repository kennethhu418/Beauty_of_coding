// beauty_of_coding.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <time.h>
#include <math.h>
#include <windows.h>

#define MAX_RUN_PASS_COUNT 10000

//正弦波
int _tmain(int argc, _TCHAR* argv[])
{
    const unsigned long UNIT_TIME = 1000; //1000ms as increment unit time
    const unsigned long K = UNIT_TIME / 2;

    unsigned long passedUnitCount = 0;    
    unsigned long execTime = 0;    
    clock_t curPassExecEndTime = 0;
    clock_t curTime = 0;

    //Set thread to run only on core 2 so that we can observe the CPU
    //ultilization curver more clearly
    SetThreadAffinityMask(GetCurrentThread(), 0x2);

    while (passedUnitCount < MAX_RUN_PASS_COUNT)
    {        
        curPassExecEndTime = clock();
        execTime = K*(sin((double)passedUnitCount) + 1);
        curPassExecEndTime += (execTime*CLOCKS_PER_SEC) / 1000;

        curTime = clock();
        while (curTime <= curPassExecEndTime)
            curTime = clock();

        if (UNIT_TIME > execTime)
            Sleep(UNIT_TIME - execTime);

        passedUnitCount++;
    }

    system("PAUSE");
	return 0;
}

//方波
//int _tmain(int argc, _TCHAR* argv[])
//{
//    const unsigned long UNIT_TIME = 8000; //1000ms
//
//    unsigned long passedUnitCount = 0;
//    clock_t curPassExecEndTime = 0;
//    clock_t curTime = 0;
//
//    //Set thread to run only on core 2 so that we can observe the CPU
//    //ultilization curver more clearly
//    SetThreadAffinityMask(GetCurrentThread(), 0x2);
//
//    while (passedUnitCount < MAX_RUN_PASS_COUNT)
//    {
//        if (passedUnitCount & 0x1)
//        {
//            curPassExecEndTime = clock() + (UNIT_TIME*CLOCKS_PER_SEC) / 1000;
//            curTime = clock();
//            while (curTime <= curPassExecEndTime)
//                curTime = clock();
//        }
//        else
//        {
//            Sleep(UNIT_TIME);
//        }
//
//        passedUnitCount++;
//    }
//
//    system("PAUSE");
//    return 0;
//}