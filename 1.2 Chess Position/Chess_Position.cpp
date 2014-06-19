// 1.2 Chess Position.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <vector>
#include <iostream>

using namespace std;


typedef struct __Position
{
    unsigned int a;
    unsigned int b;
}Postition;

class Solution
{
public:
    vector<Postition> getValidPositions()
    {
        vector<Postition> positions;
        Postition curRes;

        unsigned char posBuffer = 0x11;
        while ((posBuffer & 0xF) <= 9)
        {
            while ((posBuffer >> 4) <= 9)
            {
                if ((posBuffer & 0xF) % 3 != (posBuffer >> 4) % 3)
                {
                    curRes.a = posBuffer & 0xF;
                    curRes.b = posBuffer >> 4;
                    positions.push_back(curRes);
                }

                posBuffer += 0x10;
            }

            posBuffer &= 0xF;
            posBuffer |= 0x10;
            posBuffer += 1;
        }   

        return positions;
    }
};

int _tmain(int argc, _TCHAR* argv[])
{
    Solution so;
    vector<Postition> result = so.getValidPositions();
    int n = result.size();
    cout << "Size = " << n << endl << endl;

    for (int i = 0; i < n; i++)
    {
        cout << result[i].a << " " << result[i].b << endl;
    }

    system("PAUSE");
	return 0;
}

