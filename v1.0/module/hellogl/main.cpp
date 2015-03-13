#include <QtGui/QApplication>
#include "glwidget.h"

#include "vector_result_widget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    vector_result_widget *vectorWidget
            = new vector_result_widget();

    vectorWidget->show();



    return a.exec();
}
