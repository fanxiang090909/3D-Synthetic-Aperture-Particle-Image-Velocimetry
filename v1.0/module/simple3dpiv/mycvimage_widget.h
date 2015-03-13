#ifndef MYCVIMAGEWIDGET_H
#define MYCVIMAGEWIDGET_H

#include <QWidget>
#include <QtGui/QPaintEvent>
#include <QtGui/QImage>
#include <QtCore/QTimer>
#include <opencv/cv.h>
#include <opencv/highgui.h>

class MyCVImageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MyCVImageWidget(const IplImage *img, QWidget *parent = 0);
    ~MyCVImageWidget();

protected:
    void paintEvent(QPaintEvent *e);

private:
    IplImage *iplImg;
    QImage *qImg;
};

#endif // MYCVIMAGEWIDGET_H
