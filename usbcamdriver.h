#ifndef USBCAMDRIVER_H
#define USBCAMDRIVER_H

#include <QObject>
#include <QThread>

class UsbCamDriver : public QObject
{
	Q_OBJECT
public:
	explicit UsbCamDriver(QObject *parent = 0);

	int initDevice();
	QImage convertYUVtoRGB(void *data, int width, int height);
signals:

public slots:
};

#endif // USBCAMDRIVER_H
