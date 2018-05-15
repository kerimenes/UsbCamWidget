#include "mainwindow.h"
#include <QApplication>

#include <usbcamdriver.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    UsbCamDriver usb;
//    MainWindow w;
//    w.show();

    return a.exec();
}
