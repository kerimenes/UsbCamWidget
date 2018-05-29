#-------------------------------------------------
#
# Project created by QtCreator 2018-05-15T09:39:16
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = UsbCamWidget
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    usbcamdriver.cpp \
    packetparse.cpp \
    usbstreamer.cpp

HEADERS  += mainwindow.h \
    usbcamdriver.h \
    packetparse.h \
    usbstreamer.h

FORMS    += mainwindow.ui

message($$OUT_PWD)
INCLUDEPATH += $$OUT_PWD/../usr/local/include
LIBS += $$OUT_PWD/../usr/local/lib/liblmm.a

INCLUDEPATH += /home/kerim/myfs/codes/libuvc/build2
LIBS += -L/home/kerim/myfs/codes/libuvc/build2

LIBS += -luvc -lpcap

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
