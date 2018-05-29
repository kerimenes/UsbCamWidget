#include "packetparse.h"
#include <QDebug>
#include <pcap/usb.h>

#define file "/home/kerim/myfs/codes/build_x86/UsbCamWidget/temp/usb_log.pcapng"
#define file2 "/home/kerim/myfs/codes/build_x86/UsbCamWidget/temp/tiwiga.pcapng"
#define file3 "/home/kerim/myfs/codes/build_x86/UsbCamWidget/temp/VideoCameraUsb.pcap.pcapng"
#define file4 "/home/kerim/myfs/codes/build_x86/UsbCamWidget/temp/pattern_generator.pcapng"

PacketParse::PacketParse(QObject *parent)
	:QObject(parent)
{
	openFile();
}

struct ftframe {
	int len;
	uchar data[1920 * 1080 * 2];
};

void my_packet_handler(u_char *args, const struct pcap_pkthdr *packet_header, const u_char *packet_body)
{
//	qDebug() << packet_header->caplen << packet_header->len << packet_body;
	struct _usb_header *usbptr;

	usbptr = (struct _usb_header *) packet_body;

	if (!usbptr->data_len)
		return;

	const uchar *uvchdr = packet_body + 64;
	int hle = uvchdr[0];
	int bfh = uvchdr[1];
	static ftframe fr;
	if (hle == 12) {
//		qDebug() << "start" << fr.len << 1920 * 1080 * 2;
		fr.len = usbptr->data_len - 12;
	} else
		fr.len += usbptr->data_len;

	return;
}
int PacketParse::openFile()
{
	char errbuf[1024];
	pcap_t *handle;
	handle = pcap_open_offline(file, errbuf);
	if (handle == NULL)
		return -1;

	pcap_loop(handle, 0, my_packet_handler, NULL);
	return 0;
}

