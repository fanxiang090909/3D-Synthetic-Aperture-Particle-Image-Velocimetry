#include <QtGui/QApplication>
#include "main_widget.h"
#include "vector_result_widget.h"

#include "settings.h"
#include "pivengine.h"
#include "pivthread.h"
#include "normalizedcrosscorrelator.h"

#include "processing.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    string pathfile_A = "D:\\Frame 0 images 500_500_406(5-410)\\";
    string pathfile_B = "D:\\Frame 1 images 500_500_406(5-410)\\";
    std::string filen;

    std::vector<string> filesA = std::vector<string>();
    std::vector<string> filesB = std::vector<string>();

    std::stringstream ss;

    //12¸öÉî¶È
    for (int i = 5; i <= 110; i++)
    {
        if (i < 10)
            ss << "B0000" << i << ".tif";
        else if (i < 100)
            ss << "B000" << i << ".tif";
        else if (i < 1000)
            ss << "B00" << i << ".tif";

        filen = ss.str();
        ss.str("");
        filesA.push_back(filen.data());
        filesB.push_back(filen.data());

    }

    Settings * settings = new Settings();
    settings->pathA = pathfile_A;
    settings->pathB = pathfile_B;
    settings->filesA = filesA;
    settings->filesB = filesB;

    settings->setRoiWidth(100);
    settings->setRoiHeight(100);
    settings->setRoiDepth(100);

    Processing* process = new Processing(settings);

    //process->initializeThreads();
    process->processCurrent();


    //main_widget w;
    //w.show();



    vector_result_widget ww;

    ww.setSettings(settings);
    ww.show();


    int re = a.exec();

    delete settings;
    //delete process;

    return re;
}
