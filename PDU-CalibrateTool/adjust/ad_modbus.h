#ifndef AD_MODBUS_H
#define AD_MODBUS_H

#include "datapacket.h"
#include "ad_config.h"
#include "dbstates.h"

#define MODBUS_RTU_SIZE 255

struct sRtuItem {
    sRtuItem():fn(3),reg(0),num(0){} // 下位机有问题
    uchar addr; // 表示从机地址码
    uchar fn;  // 表示功能码
    ushort reg; // 表示寄存器首地址
    ushort num; // 表示数据长度
    ushort crc; // 表示CRC校验
};

struct sRtuReplyItem {
    sRtuReplyItem():fn(3),len(0){} // 下位机有问题
    uchar addr; // 表示从机地址码
    uchar fn;  // 表示功能码
    uchar len; // 表示数据长度
    uchar data[MODBUS_RTU_SIZE]; // 表示寄存器首地址
    ushort crc; // 表示CRC校验
};

struct sRtuSetItem
{
    uchar addr;
    uchar fn;
    ushort reg;
    ushort data;
    ushort crc;
};

struct sRtuSetItems
{
    uchar addr;
    uchar fn;
    ushort reg;
    ushort num;
    uchar len;
    uchar data[MODBUS_RTU_SIZE];
    ushort crc;
};

struct sRtuSetReplyItem
{
    uchar addr;
    uchar fn;
    uchar err;
    ushort crc;
};


class Ad_Modbus : public QThread
{
    Q_OBJECT
    explicit Ad_Modbus(QObject *parent = nullptr);
public:
    static Ad_Modbus *bulid(QObject *parent = nullptr);

    int readSerial(quint8 *cmd, int secs);
    bool writeSerial(quint8 *cmd, int len);
    int transmit(uchar *sent, int len, uchar *recv, int secs);

    ushort rtu_crc(uchar *buf, int len);
    uchar getXorNumber(uchar *buf, int len);

    int rtuRead(sRtuItem *pkt, uchar *recv);
    int rtuRead(sRtuItem *pkt, sRtuReplyItem *recv);

    bool rtuWrite(sRtuSetItem *pkt);
    bool rtuWrites(sRtuSetItems *pkt);
    bool delay(int s);

    bool appendLogItem(bool pass);
    bool changeBaudRate();
    void writeLogs();

protected:
    void initSerial();
    ushort calccrc (ushort crc, uchar crcbuf);
    int rtu_sent_packet(sRtuItem *pkt, uchar *ptr);
    int rtuRecvData(uchar *ptr,  sRtuReplyItem *pkt);

    bool rtuRecvCrc(uchar *buf, int len);
    bool rtuRecvCrc(uchar *buf, int len, sRtuReplyItem *msg);

    int rtu_write_packet(sRtuSetItem *pkt, uchar *ptr);
    int rtu_write_packets(sRtuSetItems *pkt, uchar *ptr);

private:
    sConfigItem *mItem;
    SerialPort *mSerial;
    QList<sStateItem> mLogItems;
};

#endif // AD_MODBUS_H
