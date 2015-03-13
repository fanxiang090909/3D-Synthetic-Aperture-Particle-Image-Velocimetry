#include "preprocess_widget.h"
#include "ui_preprocess_widget.h"

preprocess_widget::preprocess_widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::preprocess_widget)
{
    ui->setupUi(this);

    //connect(ui->reconstruct_3dpoints_button,click(),ui->progressBar->);
}

preprocess_widget::~preprocess_widget()
{
    delete ui;
}

void preprocess_widget::on_reconstruct_3dpoints_button_clicked()
{
    ui->progressBar->setValue(0);
}
