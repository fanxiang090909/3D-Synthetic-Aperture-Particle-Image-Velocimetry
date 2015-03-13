#include <QtGui/QApplication>
#include "window_3dpiv.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    window_3dpiv w;
    w.show();

    return a.exec();
}
