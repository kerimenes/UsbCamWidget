#ifndef PACKETPARSE_H
#define PACKETPARSE_H

#include <QObject>
#include <pcap.h>

class PacketParse : public QObject
{
	Q_OBJECT
public:
	PacketParse(QObject *parent = 0);
	int openFile();
};

#endif // PACKETPARSE_H
