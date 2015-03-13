#ifndef PROGRESSBAR_WIDGET_H
#define PROGRESSBAR_WIDGET_H

#include <QWidget>

#include "reconstruction_3d_from_tomedata.h"

namespace Ui {
    class progressbar_widget;
}

class progressbar_widget : public QWidget
{
    Q_OBJECT

public:
    explicit progressbar_widget(QWidget *parent = 0);
    ~progressbar_widget();

private slots:
    void on_reconstruct_3dpoints_button_clicked();

private:
    Ui::progressbar_widget *ui;

    Reconstruction3DFromTomeData * recon;
};

#endif // PROGRESSBAR_WIDGET_H
