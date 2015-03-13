#-------------------------------------------------
#
# Project created by QtCreator 2013-05-24T16:18:57
#
#-------------------------------------------------

QT       += core gui

TARGET = opencv_cut_image
TEMPLATE = app

INCLUDEPATH+=D:\OpenCV2.4\mingwopencv\install\include
INCLUDEPATH+=D:\OpenCV2.4\mingwopencv\install\include\opencv
LIBS+=D:\OpenCV2.4\mingwopencv\install\lib\libopencv_core240.dll.a
LIBS+=D:\OpenCV2.4\mingwopencv\install\lib\libopencv_highgui240.dll.a
LIBS+=D:\OpenCV2.4\mingwopencv\install\lib\libopencv_imgproc240.dll.a

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
