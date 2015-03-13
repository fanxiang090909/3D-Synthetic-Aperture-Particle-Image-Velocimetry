#include "mycvimage_widget.h"
#include <QtGui/QPainter>
#include <QtCore/QPoint>

MyCVImageWidget::MyCVImageWidget(const IplImage *img, QWidget *parent) : QWidget(parent)
{
    qImg = new QImage(QSize(img->width, img->height), QImage::Format_RGB888);
    //qImg = new QImage(QSize(img->width, img->height), QImage::Format_MonoLSB);

    iplImg = cvCreateImageHeader(cvSize(img->width, img->height), IPL_DEPTH_8U ,3);
    //iplImg = cvCreateImageHeader(cvSize(img->width, img->height), IPL_DEPTH_8U ,1);

    iplImg->imageData = (char*)qImg->bits();

    if (img->origin == IPL_ORIGIN_TL)
    {
        cvCopy(img, iplImg, 0);
    }
    else
    {
        cvFlip(img, iplImg, 0);
    }
   // cvCvtColor(iplImg, iplImg, CV_BGRA2RGB);
    this->resize(img->width, img->height);
}

MyCVImageWidget::~MyCVImageWidget()
{
    cvReleaseImage(&iplImg);
    delete qImg;
}

void MyCVImageWidget::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.drawImage(QPoint(0, 0), *qImg);
}
