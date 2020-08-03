#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QSerialPort>
#include <QtCore>
#include "msgbox.h"

#define SERIAL_LEN  2*1024

class SerialPort : public QThread
{
    Q_OBJECT
public:
    explicit SerialPort(QObject *parent = nullptr);
    ~SerialPort();

    bool open(const QString &name,qint32 baudRate = QSerialPort::Baud19200);
    bool isOpened(){return isOpen;}
    bool close(void);

    void reflush();
    QStringList getList();
    QString getSerialName();
    bool isContains(const QString &name);

    int send(const QByteArray &array);
    int write(const QByteArray &array);
    int write(uchar *sent, int len);

    int read(QByteArray &array, int msecs=1);
    int read(uchar *recvstr, int msecs=1);

    int transmit(const QByteArray &witeArray, QByteArray &readArray, int msecs=1);
    int transmit(uchar *sent, int len, uchar *recv, int msecs=1);

    qint32 baudRate();
    bool setBaudRate(qint32 baudRate);
    bool loopTest();

signals:

private slots:
    void recvSlot();
    void writeSlot();
    void timeoutDone();

private:
    bool isOpen;
    QSerialPort  *mSerial;

    int mCount;
    QTimer *timer;
    QReadWriteLock  mRwLock;
    QByteArray mSerialData;
    QList<QByteArray> mWriteArrays;
};

#endif // SERIALPORT_H
