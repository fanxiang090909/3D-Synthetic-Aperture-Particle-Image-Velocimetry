#include "main_widget.h"
#include "ui_main_widget.h"

main_widget::main_widget(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::main_widget)
{
    ui->setupUi(this);
}

main_widget::~main_widget()
{
    delete ui;
}
