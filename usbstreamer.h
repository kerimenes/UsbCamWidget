#ifndef USBSTREAMER_H
#define USBSTREAMER_H

#include <QObject>

#include <lmm/v4l2input.h>

class USBStreamer : public V4l2Input
{
	Q_OBJECT
public:
	explicit USBStreamer(QObject *parent = 0);

protected:
	virtual int openCamera();

signals:

public slots:
};

#endif // USBSTREAMER_H
