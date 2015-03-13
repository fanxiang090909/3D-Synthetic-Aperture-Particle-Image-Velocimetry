/*
====================================================================================

File: generategrid.cpp
Description: Algorithm to generate the list of points at which to calculate the
    velocity whether a mask has been loaded or not.
Copyright (C) 2010  OpenPIV (http://www.openpiv.net)

Contributors to this code:
Zachary Taylor

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

====================================================================================
*/

#include "generategrid.h"

#include <QColor>
#include <QImage>
#include <iostream>
#include <QPoint>
#include <QList>
#include <QRect>

#include "settings.h"

/*QList<QPoint> generateGrid(QImage *maskData, Settings *settings)
{
    QList<QPoint> grid;

    int i, j, m, n, count;

    int cAlpha;
    QColor color;
    // 感兴趣测量区域区域就是按图像的测量范围开始考虑。三个方向的坐标，从0-nx,0-ny,0-nz


    int roiWidth;   // x轴方向
    int roiHeight;  // x轴方向
    int roiDepth;   // z轴方向，或叫roiLength
    //QRect rect = settings->roi();

    int xSpacing = settings->deltaX();
    int ySpacing = settings->deltaY();
    int zSpacing = settings->deltaZ();

    int _intLengthX = settings->intLengthX();
    int _intLengthY = settings->intLengthY();
    int _intLengthZ = settings->intLengthZ();

    // If any portion of any interrogation window has a value which is not transparent then
    // that interrogation window is not included in the list to be computed.
    for (i = rect.top(); i <= (rect.bottom() - _intLengthY); i = i + ySpacing)
    {
        for (j = rect.left(); j <= (rect.right() - _intLengthX); j = j + xSpacing)
        {
            if (!maskData->isNull())
            {
                count = 0;
                for (m = i; m < (i + _intLengthY); m++)
                {
                    for (n = j; n < (j + _intLengthX); n++)
                    {
                        cAlpha = color.fromRgba(maskData->pixel(n,m)).alpha();
                        if (cAlpha != 0) count++;
                    }
                }
                if (count == 0) grid.append(QPoint(j,i));
            }
            else
            {
                grid.append(QPoint(j,i));
            }
        }
    }

    return grid;
}*/

std::list<Grid3DPoint*>* generateGrid(Settings *settings)
{
    std::list<Grid3DPoint*>* grid = new std::list<Grid3DPoint*>();

    int i, j, k;

    int roiWidth = settings->getROIWidth();   // x轴方向
    int roiHeight = settings->getROIHeight();  // y轴方向
    int roiDepth = settings->getROIDepth();   // z轴方向，或叫roiLength
    //QRect rect = settings->roi();

    int xSpacing = settings->deltaX();
    int ySpacing = settings->deltaY();
    int zSpacing = settings->deltaZ();

    int _intLengthX = settings->intLengthX();
    int _intLengthY = settings->intLengthY();
    int _intLengthZ = settings->intLengthZ();

    // 中心向四周计算，形成链表

    // Assigning the entire image to be calculated assuming no mask
    for (k = 0; k <= (roiDepth - _intLengthZ); k = k + zSpacing)
    {
        for (i = 0; i <= (roiHeight - _intLengthY); i = i + ySpacing)
        {
            for (j = 0; j <= (roiWidth - _intLengthX); j = j + xSpacing)
            {
                Grid3DPoint * pointPointer = new Grid3DPoint;
                pointPointer->xpos = j;
                pointPointer->ypos = i;
                pointPointer->zpos = k;
                pointPointer->hasCompute = false;
                grid->push_back(pointPointer);
            }
        }
    }

    /*
    // 加入第一个点
    j = (roiWidth - _intLengthX) / 2;
    i = (roiHeight - _intLengthY) / 2;
    k = (roiDepth - _intLengthZ) / 2;
    Grid3DPoint * pointPointer1 = new Grid3DPoint;
    pointPointer1->xpos = j;
    pointPointer1->ypos = i;
    pointPointer1->zpos = k;
    pointPointer1->hasCompute = false;
    Grid3DPoint & gridpoint = *pointPointer1;
    grid.push_back(gridpoint);

    int tempj,tempi,tempk;

    int times = 0;

    // 还有格子标志点没有加入list
    while((j < 0 || j > roiWidth -_intLengthX) && (i < 0 || i > roiHeight -_intLengthY)
          && (k < 0 || k > roiDepth -_intLengthZ))
    {
        tempk = k;
        tempi = i;
        tempj = j;
        while (tempk >= k - (times*2+1) * zSpacing)
        {
            // 最上边或最下边
            if (tempk == k || tempk == k - (times*2+1) * zSpacing)
                ;
            // 中间
            while (tempi >= i - (times*2+1) * ySpacing)
            {
                if (tempi == i || tempi == i - (times*2+1) * ySpacing)

                while (tempj >= j - (times*2+1) * xSpacing)
                {

                    Grid3DPoint * pointPointer2 = new Grid3DPoint;
                    pointPointer2->xpos = j;
                    pointPointer2->ypos = i;
                    pointPointer2->zpos = k;
                    pointPointer2->hasCompute = false;
                    Grid3DPoint & gridpoint = *pointPointer2;
                    grid.push_back(gridpoint);

                    tempj += xSpacing;
                }
                tempi += ySpacing;
            }
            tempk += zSpacing;
        }

        k = k + zSpacing;
        j = j + xSpacing;
        i = i + ySpacing;
        times++;
    }*/

    return grid;
}
