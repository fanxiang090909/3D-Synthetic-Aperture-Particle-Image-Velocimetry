#ifndef WINDOW_3DPIV_H
#define WINDOW_3DPIV_H

#include <QMainWindow>

namespace Ui {
    class window_3dpiv;
}

class window_3dpiv : public QMainWindow
{
    Q_OBJECT

public:
    explicit window_3dpiv(QWidget *parent = 0);
    ~window_3dpiv();

private:
    Ui::window_3dpiv *ui;
};

#endif // WINDOW_3DPIV_H
