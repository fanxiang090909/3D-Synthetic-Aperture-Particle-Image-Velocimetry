#include "window_3dpiv.h"
#include "ui_window_3dpiv.h"

window_3dpiv::window_3dpiv(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::window_3dpiv)
{
    ui->setupUi(this);
}

window_3dpiv::~window_3dpiv()
{
    delete ui;
}
