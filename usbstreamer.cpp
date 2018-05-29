#include "usbstreamer.h"
#include <QDebug>

#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include <QFile>


USBStreamer::USBStreamer(QObject *parent)
	: V4l2Input(parent)
{
	captureWidth = 1920;
	captureHeight = 1080;
	deviceName = "/dev/video1";
	nonBlockingIO = false;
}

int USBStreamer::openCamera()
{
	struct v4l2_capability cap;
	struct v4l2_format fmt;
	int width = captureWidth, height = captureHeight;
	int err = 0;
	qDebug() << nonBlockingIO;
	err = openDeviceNode();
	if (err)
		return err;
	qDebug() << "device opened";

	err = queryCapabilities(&cap);
	if (err)
		return err;
	qDebug() << "query " << cap.capabilities;


	//	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	//	if (ioctl(fd, VIDIOC_G_FMT, &fmt) == -1) {
	//		qDebug("Unable to get VIDIOC_G_FMT");
	//		err = EINVAL;
	//		return err;
	//	}
	//	fmt.fmt.pix.width = width;
	//	fmt.fmt.pix.height = height;
	//	fmt.fmt.pix.bytesperline = width * 2;
	//	fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
	//	fmt.fmt.pix.field = V4L2_FIELD_NONE;
	//	fmt.fmt.pix.sizeimage = 1920 * 1080 * 2;

	//	if (ioctl(fd, VIDIOC_S_FMT, &fmt) == -1) {
	//		qDebug("Unable to set VIDIOC_S_FMT");
	//		err = EINVAL;
	//		return err;
	//	}

	if (allocBuffers(4, V4L2_BUF_TYPE_VIDEO_CAPTURE) < 0) {
		qDebug("Unable to allocate capture driver buffers");
		err = ENOMEM;
		return err;
	}

	qDebug() << "start streaming :" << startStreaming();

	int frame_count = 100;
	while  (frame_count--) {
		for(;;) {
			fd_set fds;
			struct timeval tv;
			int r;

			FD_ZERO(&fds);
			FD_SET(fd, &fds);

			/* Timeout. */
			tv.tv_sec = 2;
			tv.tv_usec = 0;

			r = select(fd + 1, &fds, NULL, NULL, &tv);

//			if (-1 == r) {
//				if (EINTR == errno)
//					continue;
//			}

//			if (0 == r) {
//				fprintf(stderr, "select timeout\\n");
//				exit(EXIT_FAILURE);
//			}
			v4l2_buffer buf;
			buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
			buf.memory = V4L2_MEMORY_MMAP;
			if (ioctl(fd, VIDIOC_DQBUF, &buf) == -1) {
				if (errno != EAGAIN) {
					qDebug("VIDIOC_DQBUF failed with err %d", errno);
					break;
				}
				//			return NULL;
			}
			unsigned char *data = (unsigned char *)v4l2buf[buf.index];
			qDebug() << data[0] << buf.bytesused;
			qDebug() << data;
		}

	}

	return 0;
}
