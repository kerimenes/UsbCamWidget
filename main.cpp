#include "mainwindow.h"
#include <QApplication>

#include <usbcamdriver.h>
#include <packetparse.h>
#include <usbstreamer.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//	PacketParse packet;
//	UsbCamDriver usb;
//    MainWindow w;
//    w.show();

	USBStreamer streamer;
	streamer.start();

    return a.exec();
}
