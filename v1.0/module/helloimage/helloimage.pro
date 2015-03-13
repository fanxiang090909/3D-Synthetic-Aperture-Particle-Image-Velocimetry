#-------------------------------------------------
#
# Project created by QtCreator 2013-05-13T15:54:45
#
#-------------------------------------------------

QT       += core gui

TARGET = helloimage
TEMPLATE = app

INCLUDEPATH+=D:\OpenCV2.4\mingwopencv\install\include
INCLUDEPATH+=D:\OpenCV2.4\mingwopencv\install\include\opencv
LIBS+=D:\OpenCV2.4\mingwopencv\install\lib\libopencv_core240.dll.a
LIBS+=D:\OpenCV2.4\mingwopencv\install\lib\libopencv_highgui240.dll.a
LIBS+=D:\OpenCV2.4\mingwopencv\install\lib\libopencv_imgproc240.dll.a


SOURCES += main.cpp \
    mycvimage_widget.cpp \
    frame_3dpoint.cpp \
    reconstruction_3dintensity.cpp \
    reconstruction_3d_from_tomedata.cpp \
    progressbar_widget.cpp

HEADERS  += \
    mycvimage_widget.h \
    frame_3dpoint.h \
    reconstruction_3dintensity.h \
    reconstruction_3d_from_tomedata.h \
    progressbar_widget.h

FORMS += \
    progressbar_widget.ui
