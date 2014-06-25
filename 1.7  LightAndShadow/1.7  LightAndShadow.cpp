// 1.7  LightAndShadow.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <vector>
#include <algorithm>

using namespace std;

#define SAME_THRESHOLD 0.00000001

class Point
{
public:
    double x;
    double y;
    bool   allPointsOnLine;
    Point(double x = 0, double y = 0)
    {
        this->x = x;
        this->y = y;
        this->allPointsOnLine = false;
    }
};

bool comparePointsFunc(const Point &p1, const Point &p2)
{
    return p1.x < p2.x;
}

class Line
{
public:
    Line(double a = 0, double b = 0, bool vertical = false)
    {
        this->a = a;
        this->b = b;
        this->vertical = vertical;
    }

    bool getY(double x, double& y)
    {
        if (vertical)
            return false;
        return a*x + b;
    }

    bool isVertical(double &x)
    {
        if (vertical)
            x = this->a;
        return vertical;
    }

    bool operator==(const Line &anotherLine)
    {
        if (this->vertical == true && anotherLine.vertical == true)
        {
            if (this->a == anotherLine.a)
                return true;
            return false;
        }

        if (this->vertical == true || anotherLine.vertical == true)
            return false;

        return this->a == anotherLine.a && this->b == anotherLine.b;
    }

    bool getCrossingPoint(const Line& anotherLine, Point &crossingPoint)
    {
        if (this->vertical == true && anotherLine.vertical == true)
        {
            if (this->a == anotherLine.a)
            {
                crossingPoint.allPointsOnLine = true;
                crossingPoint.x = this->a;
                return true;
            }
            return false;
        }

        if (this->vertical == true)
            return anotherLine.a * this->a + anotherLine.b;
        if (anotherLine.vertical == true)
            return this->a * anotherLine.a + this->b;

        if (this->a == anotherLine.a)
        {
            if (this->b == anotherLine.b)
            {
                crossingPoint.allPointsOnLine = true;
                crossingPoint.x = crossingPoint.y = 0;
                return true;
            }
            return false;
        }

        crossingPoint.x = (this->b - anotherLine.b) / (anotherLine.a - this->a);
        crossingPoint.y = this->a * crossingPoint.x + this->b;
        crossingPoint.allPointsOnLine = false;
        return true;
    }

private:
    double  a;
    double  b;
    bool    vertical;
};

class Solution
{
public:
    void Initialize(const vector<Line> &inputLines, double inputMaxX, double inputMaxY)
    {
        this->lines = inputLines;
        this->maxX = inputMaxX;
        this->maxY = inputMaxY;

        //We assume there are no duplicate lines
        //removeDuplicateLines();
        generatingCrossingPoints();
        sort(crossingPoints.begin(), crossingPoints.end(), comparePointsFunc);
    }

    int getLightAndShadowSegments(double x1, double x2)
    {
        if (x1 > x2)
        {
            double temp = x1;
            x1 = x2;
            x2 = temp;
        }

        int lineCount = getBoundaryPointsCount(x1);
        int lineCount2 = getBoundaryPointsCount(x2);
        if (lineCount2 > lineCount)
            lineCount = lineCount2;

        if (lineCount == 0)
            return 0;
        if (lineCount == 1)
            return 2;

        int crossingPointCount = getCrossingPointInRange(x1, x2);
        return lineCount + crossingPointCount + 1;
    }

private:
    vector<Line>    lines;
    vector<Point>   crossingPoints;
    double          maxX;
    double          maxY;

    void generatingCrossingPoints()
    {
        int n = lines.size();
        if (n < 2)
            return;

        crossingPoints.clear();

        Point pt;
        bool hasCrossing;

        for (int i = 0; i < n; i++)
        {
            for (int j = i + 1; j < n; j++)
            {
                hasCrossing = lines[i].getCrossingPoint(lines[j], pt);
                if (!hasCrossing)
                    continue;

                if (pt.x <= 0 || pt.x >= maxX || pt.y <= 0 || pt.y >= maxY)
                    continue;

                crossingPoints.push_back(pt);
            }
        }
    }

    int getCrossingPointInRange(double x1, double x2)
    {
        int n = crossingPoints.size();
        if (n == 0)
            return 0;

        int x1InsertPos = 0, x2InsertPos = 0;
        int start = 0, end = crossingPoints.size() - 1, mid = 0;
        while (start < end)
        {
            mid = (start + end) / 2;
            if (abs(crossingPoints[mid].x - x1) <= SAME_THRESHOLD)
            {
                start = mid + 1;
                continue;
            }

            if (x1 < crossingPoints[mid].x)
            {
                end = mid - 1;
                continue;
            }

            start = mid + 1;
        }

        if (start > end)
            x1InsertPos = start;
        else
        {
            if (abs(crossingPoints[start].x - x1) == SAME_THRESHOLD || crossingPoints[start].x < x1)
                x1InsertPos = start + 1;
            else
                x1InsertPos = start;
        }

        /************************************************************************/
        start = 0; end = crossingPoints.size() - 1; mid = 0;
        while (start < end)
        {
            mid = (start + end) / 2;
            if (abs(crossingPoints[mid].x - x1) <= SAME_THRESHOLD || x1 < crossingPoints[mid].x)
            {
                end = mid - 1;
                continue;
            }

            start = mid + 1;
        }

        if (start > end)
            x2InsertPos = start;
        else {
            if (abs(crossingPoints[start].x - x1) == SAME_THRESHOLD || crossingPoints[start].x > x1)
                x2InsertPos = start;
            else
                x2InsertPos = start + 1; 
        }

        return x2InsertPos - x1InsertPos;
    }

    int getVerticalLineCount(double x1, double x2)
    {
        int count = 0, n = lines.size();
        double verticalX = 0;
        for (int i = 0; i < n; i++)
        {
            if (lines[i].isVertical(verticalX) && verticalX > x1 && verticalX < x2)
                count++;
        }
        return count;
    }

    int getBoundaryPointsCount(double x)
    {
        int count = 0, n = lines.size();
        double y = 0;
        for (int i = 0; i < n; i++)
        {
            if (lines[i].isVertical(y))
                continue;

            lines[i].getY(x, y);
            if (y >= 0 && y <= maxY)
                count++;
        }

        return count;
    }
};


int _tmain(int argc, _TCHAR* argv[])
{
	return 0;
}

