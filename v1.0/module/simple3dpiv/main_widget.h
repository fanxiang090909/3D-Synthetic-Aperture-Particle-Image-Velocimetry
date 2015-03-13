#ifndef MAIN_WIDGET_H
#define MAIN_WIDGET_H

#include <QMainWindow>

namespace Ui {
    class main_widget;
}

class main_widget : public QMainWindow
{
    Q_OBJECT

public:
    explicit main_widget(QWidget *parent = 0);
    ~main_widget();

private:
    Ui::main_widget *ui;
};

#endif // MAIN_WIDGET_H
