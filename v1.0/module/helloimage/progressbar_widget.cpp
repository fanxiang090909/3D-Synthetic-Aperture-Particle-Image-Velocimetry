#include "progressbar_widget.h"
#include "ui_progressbar_widget.h"



#include <vector> // STLµÄvector
#include <list> // STLµÄlist

#include <string>
#include <sstream>

progressbar_widget::progressbar_widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::progressbar_widget)
{
    ui->setupUi(this);



    std::string filen;
    std::stringstream ss;
    std::vector<string> *files = new std::vector<string>();

    for (int i = 5; i <= 410; i++)
    {
        if (i < 10)
            ss << "B0000" << i << ".tif";
        else if (i < 100)
            ss << "B000" << i << ".tif";
        else if (i < 1000)
            ss << "B00" << i << ".tif";

        filen = ss.str();
        ss.str("");
        (*files).push_back(filen.data());
    }
    string path = "D:\\Frame 1 images 500_500_406(5-410)\\";

    recon = new Reconstruction3DFromTomeData();

    recon->loadImageFiles(files, path);

    recon->initail3dPointsResultFile("D:\\resultpoints.txt");

    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum((*files).size() - 1);

    QObject::connect(recon, SIGNAL(reconstructProgress(int)), ui->progressBar, SLOT(setValue(int)));
}

progressbar_widget::~progressbar_widget()
{
    delete ui;
}

void progressbar_widget::on_reconstruct_3dpoints_button_clicked()
{

    recon->RawImagesProcess();

    recon->savePointsToFile("D:\\points.txt");

}
