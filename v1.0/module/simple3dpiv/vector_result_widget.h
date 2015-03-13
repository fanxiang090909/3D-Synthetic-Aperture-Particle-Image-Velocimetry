#ifndef VECTOR_RESULT_WIDGET_H
#define VECTOR_RESULT_WIDGET_H

#include <QWidget>
#include "glwidget.h"

namespace Ui {
    class vector_result_widget;
}

class vector_result_widget : public QWidget
{
    Q_OBJECT

public:
    explicit vector_result_widget(QWidget *parent = 0);
    ~vector_result_widget();

    void setSettings(Settings* settingsPass);

private:
    Ui::vector_result_widget *ui;

    PivData * pivdata;

    GLWidget *glWidget;

};

#endif // VECTOR_RESULT_WIDGET_H
