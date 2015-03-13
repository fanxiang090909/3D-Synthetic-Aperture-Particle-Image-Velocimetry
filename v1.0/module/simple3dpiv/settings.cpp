#include <QSize>
#include <QColor>
#include <cmath>
#include <iostream>
#include "settings.h"
//#include "filteroptions.h"
#include "generategrid.h"


Settings::Settings()
{
    // Processing
    _deltaX = 12;
    _deltaY = 12;
    _deltaZ = 12;
    _intLengthX = 48;
    _intLengthY = 48;
    _intLengthZ = 48;
    _imageSize.setWidth(-1);
    _imageSize.setHeight(-1);
    _isMask = false;    
    _processor = _3DPIV::NCCCorrelator;
    _detector = _3DPIV::GaussianSubPixel;
    //_roi = QRect(0,0,0,0);
    roiSet = false;

    // Vectors
    _vectorColourUnfiltered = QColor(Qt::cyan);
    _vectorColourFiltered = QColor(Qt::red);
    _vectorScale = 5.0;
    _vectorSub = 0.0;

    // Session
    _expName = "3DPIV";

    // Output
    _outputFolder = "";
    _outputFormat = _3DPIV::Text;

    // Batch
    _batchFilter = false;
    _batchMask = false;
    _batchShowImage = false;
    _batchShowVectors = false;
    _batchThreading = true;
}

Settings::~Settings()
{
}

/* ----------Processing -------------*/

int Settings::deltaX() { return _deltaX; }
void Settings::setDeltaX(int deltaXPass)
{
    xSpin = deltaXPass;
    changeDeltaX();
}

int Settings::deltaY() { return _deltaY; }
void Settings::setDeltaY(int deltaYPass)
{
    ySpin = deltaYPass;
    changeDeltaY();
}

int Settings::deltaZ() { return _deltaZ; }
void Settings::setDeltaZ(int deltaZPass)
{
    zSpin = deltaZPass;
    changeDeltaZ();
}

void Settings::changeDeltaX()
{
    _deltaX = int(double(_intLengthX) * double(100-xSpin) / 100);
    emit(processSettingsChanged());
}

void Settings::changeDeltaY()
{
    _deltaY = int(double(_intLengthY) * double(100-ySpin) / 100);
    emit(processSettingsChanged());
}

void Settings::changeDeltaZ()
{
    _deltaZ = int(double(_intLengthZ) * double(100-zSpin) / 100);
    emit(processSettingsChanged());
}

int Settings::intLengthX() { return _intLengthX; }
void Settings::setIntLengthX(int intLengthXPass)
{
    _intLengthX = int(pow(2.0,double(4+intLengthXPass)));
    changeDeltaX();
}

int Settings::intLengthY() { return _intLengthY; }
void Settings::setIntLengthY(int intLengthYPass)
{
    _intLengthY = int(pow(2.0,double(4+intLengthYPass)));
    changeDeltaY();
}

int Settings::intLengthZ() { return _intLengthZ; }
void Settings::setIntLengthZ(int intLengthZPass)
{
    _intLengthZ = int(pow(2.0,double(4+intLengthZPass)));
    changeDeltaZ();
}


int Settings::processor() { return _processor; }
void Settings::setProcessor(int processPass) { _processor = processPass; }

int Settings::detector() { return _detector; }
void Settings::setDetector(int detectPass) { _detector = detectPass; }

bool Settings::isMask() { return _isMask; }
void Settings::setIsMask(bool isMaskPass)
{
    _batchMask = isMaskPass;
    _isMask = isMaskPass;
    emit(imageSizeChanged());
}

int Settings::getROIWidth() {    return roiWidth;   }
void Settings::setRoiWidth(int roiWidthPass)
{
    roiWidth = roiWidthPass;
}

int Settings::getROIDepth() {    return roiDepth;   }
void Settings::setRoiDepth(int roiDepthPass)
{
    roiDepth = roiDepthPass;
}

int Settings::getROIHeight() {  return roiHeight;   }
void Settings::setRoiHeight(int roiHeightPass)
{
    roiHeight = roiHeightPass;
}

/*-------Session--------------*/


/*-------Batch Processing-------*/

bool Settings::batchFilter() { return _batchFilter; }
void Settings::setBatchFilter(bool batchFilterPass) { _batchFilter = batchFilterPass; }

bool Settings::batchMask() { return _batchMask; }
void Settings::setBatchMask(bool batchMaskPass) { _batchMask = batchMaskPass; }

bool Settings::batchShowImage() { return _batchShowImage; }
void Settings::setBatchShowImage(bool batchShowImagePass) { _batchShowImage = batchShowImagePass; }

bool Settings::batchShowVectors() { return _batchShowVectors; }
void Settings::setBatchShowVectors(bool batchShowVectorsPass) { _batchShowVectors = batchShowVectorsPass; }

bool Settings::batchThreading() { return _batchThreading; }
void Settings::setBatchThreading(bool batchThreadingPass) { _batchThreading = batchThreadingPass; }

/*-------- Vectors ------------*/

QColor Settings::vectorColourFiltered() { return _vectorColourFiltered; }
void Settings::setVectorColourFiltered(QColor vectorColourPass)
{
    _vectorColourFiltered = vectorColourPass;
    emit(vectorSettingChanged());
}
QColor Settings::vectorColourUnfiltered() { return _vectorColourUnfiltered; }
void Settings::setVectorColourUnfiltered(QColor vectorColourPass)
{
    _vectorColourUnfiltered = vectorColourPass;
    emit(vectorSettingChanged());
}
double Settings::vectorScale() { return _vectorScale; }
void Settings::setVectorScale(double vectorScalePass)
{
    _vectorScale = vectorScalePass;
    emit(vectorSettingChanged());
}

double Settings::vectorSub() { return _vectorSub; }
void Settings::setVectorSub(double vectorSubPass)
{
    _vectorSub = vectorSubPass;
    emit(vectorSettingChanged());
}

/* ---------Filter-----------*/

/*FilterOptions Settings::filterOptions() { return _filterOptions; }
void Settings::setFilterOptions(FilterOptions filterOptionsPass)
{
    _filterOptions = filterOptionsPass;
}*/

/* ---------Output-----------*/

QString Settings::expName() { return _expName; }
void Settings::setExpName(QString expNamePass)
{
    _expName = expNamePass;
}
QString Settings::outputFolder() { return _outputFolder; }
void Settings::setOutputFolder(QString outputFolderPass)
{
    _outputFolder = outputFolderPass;
}
int Settings::outputFormat() { return _outputFormat; }
void Settings::setOutputFormat(int outputFormatPass)
{
    _outputFormat = outputFormatPass;
}


/***************************************/
/***************************************/
/////////////generate grid///////////////
std::list<Grid3DPoint*>* Settings::gridList()
{
    if (isMask()) _gridList = generateGrid(this);
    else _gridList = generateGrid(this);
    return _gridList;
}
