/*
====================================================================================

File: pivengine.cpp
Description: The base class for current and future correlation methods.  The class
    is designed to be thread safe and future developers are encouraged to build on
    this class by inheritance and by reimplementing the crossCorrelate function.
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


//#include <iostream>

//#include <QList>
//#include <QPoint>
#include <QMutex>

// Subpixel interpolators
//#include "gaussiansubpixel.h"
#include <list>

#include "pivengine.h"

#include "settings.h"
#include "frame_3dpoint.h"

#include <qDebug>

using namespace std;

PivEngine::PivEngine(Settings *settingsPass, std::list<Grid3DPoint*>* gridPass)
{
    // Initialization of local private variables
    _intLengthX = settingsPass->intLengthX();
    _intLengthY = settingsPass->intLengthY();
    _intLengthZ = settingsPass->intLengthZ();

    _width = settingsPass->getROIWidth();
    _height = settingsPass->getROIHeight();
    _depth = settingsPass->getROIDepth();

    _detector = settingsPass->detector();
    _cmap = new double [_intLengthX*2*_intLengthY*2+1];
    cmapCreated = true;
    _grid = gridPass;

    //writer = NULL;
    //file = NULLs;
    //setWriter("D:\\velocity.txt");
}

PivEngine::~PivEngine()
{
    // If the correlation map has been created, need to delete it
    if (cmapCreated)
    {
        delete [] _cmap;
        cmapCreated = false;
    }
    // If the images have been created, need to delete them
    if (imagesCreated)
    {
        //delete _imageA;
        //delete _imageB;
        imagesCreated = false;
    }
}

bool PivEngine::loadDataImages()
{

}

/* --------- Access functions ----------*/
//MetaData PivEngine::data() { return _data; }
int PivEngine::intLengthX() { return _intLengthX; }
int PivEngine::intLengthY() { return _intLengthY; }
int PivEngine::intLengthZ() { return _intLengthZ; }


//QList<QPoint> PivEngine::grid() { return _grid; }
std::list<Grid3DPoint*>* PivEngine::grid() { return _grid; }

bool PivEngine::imagesAvailable() { return imagesCreated; }
//ImageData* PivEngine::imageA() { return _imageA; }
//ImageData* PivEngine::imageB() { return _imageB; }
//double PivEngine::meanImageA() { return _meanImageA; }
//double PivEngine::meanImageB() { return _meanImageB; }

double* PivEngine::cmap() { return _cmap; }
/* --------- Access functions ----------*/

PivData* PivEngine::operator ()(/*MetaData dataPass*/)
{
    PivData *pivData;
    pivData = new PivData;

    // First step is to load the images into memory
    //_data = dataPass;
    //loadImages();

    PivPointData pointData;
    QList<PivPointData> _dataList;

    std::list<Grid3DPoint*>::iterator gridIter;

    //this->setWriter("D:\\velocity.txt");
    /*************************/
    QFile file("D:\\velocity.txt");

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return NULL;

    QTextStream writer(&file);

    writer << "TITLE = \"B00001\"\n";
    writer << "VARIABLES = \"x\", \"y\", \"z\", \"Vx\", \"Vy\", \"Vz\", \"NCC\"\n";
    writer << "ZONE T=\"Frame 0\", I=   , J=   , K=   \n";
    //out << "GRID LENGTH = 20, WIN_DELTA = 12 CAND_WIN_R = 20\n";
    /*************************/

    // 大量修改
    // 同时将计算结果设置成预测速度，方便每个格子的crossCorrelate计算中的遍历
    for (gridIter = _grid->begin(); gridIter != _grid->end(); gridIter++)
    {
        // Second step is to find the mean of each interrogation window
        //_meanImageA = this->imageMean(_imageA,_grid.value(i).ry(),_grid.value(i).rx());
        //_meanImageB = this->imageMean(_imageB,_grid.value(i).ry(),_grid.value(i).rx());

        // Third step is to calculate the displacement (velocity) between the two windows
        pointData = this->velocity((*gridIter)->xpos, (*gridIter)->ypos, (*gridIter)->zpos);

        // The minimum average intensity between the two images in the image pair is recorded
        //if (_meanImageA > _meanImageB) pointData.intensity = _meanImageB;
        //else pointData.intensity = _meanImageA;

        //writer << pointData.x;
        //writer << pointData.x << " " << pointData.y << " " << pointData.z << " " <<
        //             pointData.u << " " << pointData.v << " " << pointData.w << " " << pointData.ncc << "\n";

        writer << pointData.x << " " << pointData.y << " " << pointData.z << " " <<
                     10 << " " << 10 << " " << 10 << " " << pointData.ncc << "\n";


        // Finally the data is appended to the list of data
        _dataList.append(pointData);

    }
    //this->closeWriter();
    file.close();

    // Cleaning up
    pivData->setList(_dataList);
    //delete _imageA;
    //delete _imageB;
    imagesCreated = false;
    qDebug() << "pivengine::operator end";
    // Returning the data list
    return pivData;
}

//PivPointData PivEngine::velocity(int topLeftRow, int topLeftColumn)
//PivPointData PivEngine::velocity(int _intBxPass, int _intByPass, int _intBzPass)
PivPointData PivEngine::velocity(int _intAxPass, int _intAyPass, int _intAzPass)
{
    PivPointData pointData;

    // Performing the cross-correlation.  If the operation is unsuccessful the data are set to zeros.
    //if (crossCorrelate(topLeftRow, topLeftColumn))
    //if (crossCorrelate(_intBxPass, _intByPass, _intBzPass))
    if (crossCorrelate(_intAxPass, _intAyPass, _intAzPass, &pointData))
    {
        // Once the correlation map is computed, we estimate the displacement to sub-pixel accuracy
        switch (_detector)
        {
        // Gaussian sub-pixel estimation is currently the most widely used and robust estimator
        case _3DPIV::GaussianSubPixel:
            //pointData = gaussianSubPixel(_cmap,_intLengthX,_intLengthY);
            break;
        default:
            break;
        }
    }
    else
    {
        pointData.u = 0.0;
        pointData.v = 0.0;
        pointData.snr = 0.0;
    }

    // Data have not been filtered by this point
    pointData.filtered = false;
    /*pointData.x = _intAx + _intLengthX / 2;
    pointData.y = _intAy + _intLengthY / 2;
    pointData.z = _intAz + _intLengthZ / 2;
*/
    pointData.x = _intAxPass + _intLengthX / 2;
    pointData.y = _intAyPass + _intLengthY / 2;
    pointData.z = _intAzPass + _intLengthZ / 2;

    return pointData;
}

/*void PivEngine::loadImages()
{
    _imageA = new ImageData;
    _imageB = new ImageData;

    if (_imageA->read(_data.imageA()) && _imageB->read(_data.imageB())) imagesCreated = true;
    else
    {
        delete _imageA;
        delete _imageB;
        imagesCreated = false;
    }
}*/

//bool PivEngine::crossCorrelate(int topLeftRow, int topLeftColumn)
//bool PivEngine::crossCorrelate(int _intBx, int _intBy, int _intBz)
bool PivEngine::crossCorrelate(int _intAx, int _intAy, int _intAz, PivPointData* pointDataPass)
{
    /*  This is the only necssary function to change when this class is inherited.
        It is called from the PivEngine class and is expected to calculate the correlation
        map by access through this->cmap().
    */
    return false;
}

/*double PivEngine::imageMean(ImageData *image, int topLeftRow, int topLeftColumn)
{
    // Straightforward two-dimensional mean of the interrogation window beginning at
    // (x,y) = (topLeftColumn, topLeftRow)
    double mean = 0.0;
    int count = 0;
    double* data = image->buffer();
    int imageWidth = image->width();

    int imageI, imageJ;

    for (imageI = topLeftRow; imageI < (topLeftRow + _intLengthY); imageI++)
    {
        for (imageJ = topLeftColumn; imageJ < (topLeftColumn + _intLengthX); imageJ++)
        {
            mean += data[imageWidth*imageI + imageJ];
            count++;
        }
    }
    mean = mean / count;

    return mean;
}*/

/*bool PivEngine::setWriter(QString filename)
{

    // 处理文件
    this->file = QFile(filename);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    this->writer = QTextStream(&file);

    //qDebug() << "size:" << this->list().size();

    writer << "TITLE = \"B00001\"\n";
    writer << "VARIABLES = \"x\", \"y\", \"z\", \"Vx\", \"Vy\", \"Vz\", \"NCC\"\n";
    writer << "ZONE T=\"Frame 0\", I=   , J=   , K=   \n";
    //out << "GRID LENGTH = 20, WIN_DELTA = 12 CAND_WIN_R = 20\n";

    return true;
}

bool PivEngine::closeWriter()
{
    file.close();
}*/
