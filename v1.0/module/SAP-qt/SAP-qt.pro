#-------------------------------------------------
#
# Project created by QtCreator 2013-06-08T17:09:37
#
#-------------------------------------------------

QT       += core

QT       -= gui

INCLUDEPATH+=D:\OpenCV2.4\mingwopencv\install\include
INCLUDEPATH+=D:\OpenCV2.4\mingwopencv\install\include\opencv
LIBS+=D:\OpenCV2.4\mingwopencv\install\lib\libopencv_core240.dll.a
LIBS+=D:\OpenCV2.4\mingwopencv\install\lib\libopencv_highgui240.dll.a
LIBS+=D:\OpenCV2.4\mingwopencv\install\lib\libopencv_imgproc240.dll.a


TARGET = SAP-qt
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    projectimage.cpp \
    SAP.cpp

HEADERS += \
    projectimage.h

OTHER_FILES += \
    pic_p.txt \
    pic.txt \
    direct_p.txt \
    direct.txt \
    calibration_result_x.xml \
    calibration_result.xml
