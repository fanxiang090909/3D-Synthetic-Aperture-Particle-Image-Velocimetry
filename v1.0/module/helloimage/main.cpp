#include <QtGui/QApplication>

#include "mycvimage_widget.h"
#include "progressbar_widget.h"


#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <QDebug>

#include <vector> // STLµÄvector
#include <list> // STLµÄlist

#include <string>
#include <sstream>

#include "frame_3dpoint.h"

#include "reconstruction_3d_from_tomedata.h"

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    progressbar_widget *p_widget = new progressbar_widget();
    p_widget->show();


    /*std::string filen;
    std::stringstream ss;
    std::vector<string> *files = new std::vector<string>();
    for (int i = 5; i <= 9; i++)
    {
        if (i < 10)
            ss << "B0000" << i << ".tif";
        //else if (i < 100)
        //    ss << "B000" << i << ".tif";
        //else if (i < 1000)
        //    ss << "B00" << i << ".tif";

        filen = ss.str();
        ss.str("");
        (*files).push_back(filen.data());
    }
    string path = "D:\\SAPIV\\LaVison\\Package9\\Frame 0 images 005-200\\";

    Reconstruction3DFromTomeData * recon = new Reconstruction3DFromTomeData();

    recon->loadImageFiles(files, path);

    recon->initail3dPointsResultFile("D:\\resultpoints.txt");
*/
//    recon->RawImagesProcess();

//    recon->savePointsToFile("D:\\points.txt");

    int re = a.exec();

    return re;
}
