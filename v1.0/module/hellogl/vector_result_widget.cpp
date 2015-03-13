#include "vector_result_widget.h"
#include "ui_vector_result_widget.h"

vector_result_widget::vector_result_widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::vector_result_widget)
{
    ui->setupUi(this);

    glWidget = new GLWidget;

    //connect(glWidget, SIGNAL(xRotationChanged(int)), glWidget, SLOT(setXRotation(int)));
    //connect(glWidget, SIGNAL(yRotationChanged(int)), glWidget, SLOT(setYRotation(int)));
    //connect(glWidget, SIGNAL(zRotationChanged(int)), glWidget, SLOT(setZRotation(int)));

    connect(ui->orthoSlider, SIGNAL(valueChanged(int)), glWidget, SLOT(setViewPointLength(int)));
    connect(glWidget, SIGNAL(viewPointLengthChanged(int)), ui->orthoSlider, SLOT(setValue(int)));

    ui->gridLayout->addWidget(glWidget, 0, 0, 0);


    pivdata = new PivData(5, 5, 5);
    pivdata->read(1,QString("B00001.dat"),500);


    /*PivPointData pivpoint1;
    pivpoint1.x = 1;
    pivpoint1.y = 1;
    pivpoint1.z = 1;
    pivpoint1.u = 2;
    pivpoint1.v = 1;
    pivpoint1.w = 2;
    pivpoint1.snr = 1.0;

    PivPointData pivpoint2;
    pivpoint2.x = 2;
    pivpoint2.y = 1;
    pivpoint2.z = 1;
    pivpoint2.u = 4;
    pivpoint2.v = 1;
    pivpoint2.w = 2;
    pivpoint2.snr = 1.0;

    PivPointData pivpoint3;
    pivpoint3.x = 3;
    pivpoint3.y = 2;
    pivpoint3.z = 1;
    pivpoint3.u = 1;
    pivpoint3.v = 1;
    pivpoint3.w = 2;
    pivpoint3.snr = 1.0;

    PivPointData pivpoint4;
    pivpoint4.x = 4;
    pivpoint4.y = 4;
    pivpoint4.z = 1;
    pivpoint4.u = 1;
    pivpoint4.v = 1;
    pivpoint4.w = 1;
    pivpoint4.snr = 1.0;

    PivPointData pivpoint5;
    pivpoint5.x = 3;
    pivpoint5.y = 4;
    pivpoint5.z = 2;
    pivpoint5.u = 2;
    pivpoint5.v = 5;
    pivpoint5.w = 6;
    pivpoint5.snr = 1.0;

    QList<PivPointData> _data = QList<PivPointData>();
    _data.append(pivpoint1);
    _data.append(pivpoint2);
    _data.append(pivpoint3);
    _data.append(pivpoint4);
    _data.append(pivpoint5);

    pivdata->setList(_data);
*/
    glWidget->setPivData(pivdata);

    glWidget->updateGL();

    //w.updateGL();

}

vector_result_widget::~vector_result_widget()
{
    delete pivdata;
    delete glWidget;
    delete ui;
}
