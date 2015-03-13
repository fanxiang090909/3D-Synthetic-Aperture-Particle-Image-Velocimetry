#include <QtGui/QApplication>
#include "mainwindow.h"
#include "cv.h"
#include "cxcore.h"
#include "highgui.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <QDebug>

using namespace std;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    IplImage* src;
    IplImage* dst;

    std::string filen;
    std::stringstream ss;
    std::vector<string> files = std::vector<string>();

    for (int i = 5; i <= 200; i++)
    {
        if (i < 10)
            ss << "B0000" << i << ".tif";
        else if (i < 100)
            ss << "B000" << i << ".tif";
        else if (i < 1000)
            ss << "B00" << i << ".tif";

        filen = ss.str();
        ss.str("");
        files.push_back(filen.data());
    }
    string path = "D:\\SAPIV\\LaVison\\Package9\\Frame 0 images 005-200\\";

    string outpath = "D:\\Frame 0 images 500_500_406(5-410)\\";

    string pathfile = "";

    int num = 0;
    for (int k = 0; k < files.size(); k++)
    {
        num++;
        qDebug() << "start" << num;
        pathfile = path + files[k];

        src = cvLoadImage(pathfile.data(), 1);//CV_LOAD_IMAGE_GRAYSCALE);

        cvSetImageROI(src, cvRect(0,0,500,500));

        dst = cvCreateImage(cvSize(500, 500), IPL_DEPTH_8U, src->nChannels);

        cvCopy(src, dst, 0);

        cvResetImageROI(src);

        pathfile = outpath + files[k];

        cvSaveImage(pathfile.data(),dst);

        cvReleaseImage(&src);
        cvReleaseImage(&dst);
        qDebug() << "end" << num;

    }

    MainWindow w;
    w.show();

    return a.exec();
}
