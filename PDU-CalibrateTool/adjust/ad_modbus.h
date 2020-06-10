#ifndef AD_MODBUS_H
#define AD_MODBUS_H

#include "datapacket.h"
#include "adjustconfig.h"
#define MODBUS_RTU_SIZE 256

struct sRtuItem {
    sRtuItem():fn(3),reg(0),len(0){} // 下位机有问题
    uchar addr; // 表示从机地址码
    uchar fn;  // 表示功能码
    ushort reg; // 表示寄存器首地址
    ushort len; // 表示数据长度
    ushort crc; // 表示CRC校验
};

struct sRtuRecvItem {
    sRtuRecvItem():fn(3),len(0){} // 下位机有问题
    uchar addr; // 表示从机地址码
    uchar fn;  // 表示功能码
    ushort len; // 表示数据长度
    uchar data[MODBUS_RTU_SIZE]; // 表示寄存器首地址
    ushort crc; // 表示CRC校验
};


struct sRtuSetItem
{
    int addr;
    int reg;
    ushort value;
};


class Ad_Modbus : public QThread
{
    Q_OBJECT
    explicit Ad_Modbus(QObject *parent = nullptr);
public:
     static Ad_Modbus *bulid(QObject *parent = nullptr);

    int readSerial(quint8 *cmd, int sec);
    bool writeSerial(quint8 *cmd, int len);
    int transmit(uchar *sent, int len, uchar *recv, int sec);

    ushort rtu_crc(uchar *buf, int len);
    uchar getXorNumber(uchar *buf, int len);

    int rtuTrans(sRtuItem *pkt, uchar *recv);
    int rtuTrans(sRtuItem *pkt, sRtuRecvItem *recv);

protected:
    void initSerial();
    ushort calccrc (ushort crc, uchar crcbuf);
    int rtu_sent_packet(sRtuItem *pkt, uchar *ptr);
    int rtuRecvData(uchar *ptr,  sRtuRecvItem *pkt);
    bool rtuRecvCrc(uchar *buf, int len, sRtuRecvItem *msg);

private:
    sConfigItem *mItem;
    SerialPort *mSerial;
};

#endif // AD_MODBUS_H
