#ifndef UDPSENDSOCKET_H
#define UDPSENDSOCKET_H

#include <QObject>
#include <QtCore>
#include <QtNetwork>
#include "json_pack.h"

class UdpSendSocket : public QThread
{
    Q_OBJECT
public:
    explicit UdpSendSocket(QObject *parent = 0);
    ~UdpSendSocket();

    bool initSocket(int port);
    int dataSend(void);

protected:


signals:

public slots:

private:
    bool isRun;
    QUdpSocket *mUdpSocket;
};

#endif // UDPSENDSOCKET_H
