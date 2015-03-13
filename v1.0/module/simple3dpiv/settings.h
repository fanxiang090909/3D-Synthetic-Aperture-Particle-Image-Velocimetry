#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QColor>
#include <QSize>
//#include <QList>
#include <QPoint>
#include <QRect>

#include <vector>
#include <string>
#include <list>
#include "frame_3dpoint.h"
//#include "filteroptions.h"

using namespace std;

namespace _3DPIV
{
    // Processors
    const int FFTCorrelator = 0;
    const int NCCCorrelator = 1;

    // Detectors
    const int GaussianSubPixel = 0;

    // Output
    const int Text = 0;
    const int HDF5 = 1;
};

class Settings : public QObject
{
    Q_OBJECT

    public:
        Settings();
        virtual ~Settings();

        // Processing
        int deltaX();       
        int deltaY();
        int deltaZ();
        int intLengthX();
        int intLengthY();
        int intLengthZ();
        int processor();
        int detector();
        QSize imageSize();
        void setImageSize(QSize imageSizePass);
        bool isMask();

        // 返回测量区域长宽及深度尺寸
        // QRect roi();
        int getROIWidth();   // x轴方向
        int getROIHeight();  // x轴方向
        int getROIDepth();   // z轴方向，或叫roiLength

        // Batch processing
        bool batchFilter();
        bool batchMask();
        bool batchShowImage();
        bool batchShowVectors();
        bool batchThreading();

        // Vectors
        QColor vectorColourFiltered();
        QColor vectorColourUnfiltered();
        double vectorScale();
        double vectorSub();

        // Filters
        //FilterOptions filterOptions();

        // Output
        QString expName();
        QString outputFolder();
        int outputFormat();

        /////////////generate grid///////////////
        std::list<Grid3DPoint*>* gridList();

        // files
        std::vector<string> filesA;
        std::vector<string> filesB;
        string pathA;
        string pathB;


    public slots:
        
        // Processing
        void setDeltaX(int deltaXPass);
        void setDeltaY(int deltaYPass);
        void setDeltaZ(int deltaZPass);
        void setIntLengthX(int intLengthXPass);
        void setIntLengthY(int intLengthYPass);
        void setIntLengthZ(int intLengthZPass);
        void setProcessor(int processPass);
        void setDetector(int detectPass);
        void setIsMask(bool isMaskPass);
        // void setRoi(QRect roiPass);
        void setRoiWidth(int roiWidthPass);
        void setRoiHeight(int roiHeightPass);
        void setRoiDepth(int roiDepthPass);

        // Batch processing
        void setBatchFilter(bool batchFilterPass);
        void setBatchMask(bool batchMaskPass);
        void setBatchShowImage(bool batchShowImagePass);
        void setBatchShowVectors(bool batchShowVectorsPass);
        void setBatchThreading(bool batchThreadingPass);

        // Vectors
        void setVectorColourFiltered(QColor vectorColourPass);
        void setVectorColourUnfiltered(QColor vectorColourPass);
        void setVectorScale(double vectorScalePass);
        void setVectorSub(double vectorSubPass);

        // Filters
        //void setFilterOptions(FilterOptions filterOptionsPass);

        // Output
        void setExpName(QString expNamePass);
        void setOutputFolder(QString outputFolderPass);
        void setOutputFormat(int outputFormatPass);

    signals:
        // Display
        void imageSizeChanged();

        // Processing
        void processSettingsChanged();

        // Vectors
        void vectorSettingChanged();

    protected:
        void changeDeltaX();
        void changeDeltaY();
        void changeDeltaZ();

    private:
        // Display
        QSize _imageSize;

        // Masking
        bool _isMask;
        //QRect _roi;
        //roi区域
        int roiWidth;   // x轴方向
        int roiHeight;  // x轴方向
        int roiDepth;   // z轴方向，或叫roiLength
        bool roiSet;

        // Processing
        int _deltaX, _deltaY, _deltaZ;
        int xSpin, ySpin, zSpin;
        int _intLengthX, _intLengthY, _intLengthZ;
        int _processor;
        int _detector;

        // Batch processing
        bool _batchFilter;
        bool _batchMask;
        bool _batchShowImage;
        bool _batchShowVectors;
        bool _batchThreading;

        // Vectors
        QColor _vectorColourFiltered;
        QColor _vectorColourUnfiltered;
        double _vectorScale;
        double _vectorSub;

        // Filters
        //FilterOptions _filterOptions;

        // Output
        QString _outputFolder;
        QString _expName;
        int _outputFormat;


        /////////////generate grid///////////////
        std::list<Grid3DPoint*> * _gridList;
};

#endif // SETTINGS_H
