#ifndef PREPROCESS_WIDGET_H
#define PREPROCESS_WIDGET_H

#include <QWidget>

namespace Ui {
    class preprocess_widget;
}

class preprocess_widget : public QWidget
{
    Q_OBJECT

public:
    explicit preprocess_widget(QWidget *parent = 0);
    ~preprocess_widget();

private slots:
    void on_reconstruct_3dpoints_button_clicked();

private:
    Ui::preprocess_widget *ui;
};

#endif // PREPROCESS_WIDGET_H
