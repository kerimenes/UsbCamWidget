#-------------------------------------------------
#
# Project created by QtCreator 2018-05-15T09:39:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = UsbCamWidget
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    usbcamdriver.cpp

HEADERS  += mainwindow.h \
    usbcamdriver.h

FORMS    += mainwindow.ui

INCLUDEPATH += /home/kerim/myfs/codes/libuvc/build2
LIBS += -L/home/kerim/myfs/codes/libuvc/build2

LIBS += -luvc

INCLUDEPATH += /usr/include/opencv
    LIBS += -lopencv_core \
        -lopencv_imgproc \
        -lopencv_highgui \
        -lopencv_ml \
        -lopencv_video \
        -lopencv_features2d \
        -lopencv_calib3d \
        -lopencv_objdetect \
        -lopencv_contrib \
        -lopencv_legacy \
        -lopencv_flann \
        -lopencv_nonfree
