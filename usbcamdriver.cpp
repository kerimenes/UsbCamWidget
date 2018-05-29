#include "usbcamdriver.h"
#include <libuvc/libuvc.h>
#include <libuvc/libuvc_config.h>

#include <QDebug>
#include <QElapsedTimer>
#include <QImage>
#include <QFile>

#include <unistd.h>
#include <opencv/cv.h>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;

QImage convertYUVtoRGB(void *data, int width, int height)
{
	unsigned char* pData = (unsigned char *) data;

	QImage m_RGB(width, height, QImage::Format_RGB888);

	for(int i = 0, j=0; i < width * height * 3; i+=6, j+=4)
	{
		unsigned char u = pData[j];
		unsigned char y = pData[j+1];
		unsigned char v = pData[j+2];
		//fprintf(stderr, "%d\n", v);

		m_RGB.bits()[i+2] = 1.0*y + 8 + 1.402*(v-128);               // r
		m_RGB.bits()[i+1] = 1.0*y - 0.34413*(u-128) - 0.71414*(v-128);   // g
		m_RGB.bits()[i] = 1.0*y + 1.772*(u-128) + 0;                            // b

		y = pData[j+3];
		m_RGB.bits()[i+5] = 1.0*y + 8 + 1.402*(v-128);               // r
		m_RGB.bits()[i+4] = 1.0*y - 0.34413*(u-128) - 0.71414*(v-128);   // g
		m_RGB.bits()[i+3] = 1.0*y + 1.772*(u-128) + 0;
	}

	return m_RGB;
}

unsigned char* cvtI422toRGB(void* data, int w, int h)
{
	unsigned char* yData = (unsigned char*) data;
	unsigned char* rgbData;
	rgbData = (unsigned char*)malloc(w * h * 3);

	for (int i = 0, j = 0; i < w * h * 3; i+=6, j+=4) {
		unsigned char y = yData[j];
		unsigned char u = yData[j+1];
		unsigned char v = yData[j+3];
		rgbData[i+0] = 1.0*y + 1.140*v; //R
		rgbData[i+1] = 1.0*y - 0.395*u - 0.581*v; //G
		rgbData[i+2] = 1.0*y + 2.032*u; //B

		y = yData[j+2];
		rgbData[i+3] = 1.0*y + 1.140*v; //R
		rgbData[i+4] = 1.0*y - 0.395*u - 0.581*v; //G
		rgbData[i+5] = 1.0*y + 2.032*u; //B
	}
	return rgbData;
}

void cb(uvc_frame_t *frame, void *)
{
	static QElapsedTimer timer;
	qDebug() << "cb" << timer.restart() << frame->data_bytes << frame->sequence << frame->step << frame->width
			 << frame->height << (uchar *)frame->data << frame->frame_format;

//	QFile f(QString("images/im%1.bin").arg(frame->sequence));
//	if(!f.open(QIODevice::WriteOnly | QIODevice::Unbuffered))
//		return;
//	f.write((const char*)frame->data, 1920 * 1080* 2);
//	f.close();

}

UsbCamDriver::UsbCamDriver(QObject *parent)
	: QObject(parent)
{
	initDevice();
}

int UsbCamDriver::initDevice()
{
	uvc_context_t *ctx;
	uvc_device_t *dev;
	uvc_device_handle_t *devh;
	uvc_stream_ctrl_t ctrl;
	uvc_error_t res;
	uvc_stream_handle_t *strmh;

	res = uvc_init(&ctx, NULL);
	if (res < 0) {
		uvc_perror(res, "uvc_init");
		return res;
	}

	res = uvc_find_device(
				ctx, &dev,
				0, 0, NULL); /* filter devices: vendor_id, product_id, "serial_num" */
	if (res < 0) {
		uvc_perror(res, "uvc_find_device");
	} else qDebug() << "Found Device";

	res = uvc_open(dev, &devh);
	if (res < 0) {
		uvc_perror(res, "uvc_open"); /* unable to open device */
	} else qDebug() << "Device opened";

	uvc_print_diag(devh, stderr);

	res = uvc_get_stream_ctrl_format_size(devh, &ctrl, UVC_FRAME_FORMAT_YUYV, 1920, 1080, 25);
	ctrl.bFormatIndex = 1;
	ctrl.bFrameIndex = 1;
	ctrl.dwFrameInterval = 400000;
	ctrl.wKeyFrameRate = 0;
	ctrl.wPFrameRate = 0;
	ctrl.wCompQuality = 0;
	ctrl.wCompWindowSize = 0;
	ctrl.wDelay = 0;
	ctrl.dwMaxVideoFrameSize = 41472000;
	ctrl.dwMaxPayloadTransferSize = 32768;//16384;
	ctrl.bInterfaceNumber = 1;
	uvc_print_stream_ctrl(&ctrl, stderr);
	void* data = 0;
	res = uvc_start_streaming(devh, &ctrl, cb, data, 0 );
	if (res < 0)
		qDebug() << "error";
	else {
		sleep(30);
	}
	qDebug() << res << "result";
	return 0;
}


