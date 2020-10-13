/*
 *
 *  Created on: 2020年10月1日
 *      Author: Lzy
 */
#include "ad_modbus.h"

extern QString user_land_name();

Ad_Modbus::Ad_Modbus(QObject *parent) : QThread(parent)
{
    mSerial = nullptr;
    mItem = Ad_Config::bulid()->item;
}


Ad_Modbus *Ad_Modbus::bulid(QObject *parent)
{
    static Ad_Modbus* sington = nullptr;
    if(sington == nullptr)
        sington = new Ad_Modbus(parent);
    return sington;
}

bool Ad_Modbus::delay(int s)
{
    bool ret = true;
    for(int i=0; i<10*s; ++i) {
        if(mItem->step != Test_Over) {
            msleep(100);
        } else {
            ret = false;
            break;
        }
    }

    return ret;
}

bool Ad_Modbus::writeMac(const QString &mac)
{
    sMacItem it;

    sDataPacket *packet = sDataPacket::bulid();
    it.dev = packet->dev_type.split("_").first();
    it.op = user_land_name();
    it.user = mItem->user;
    it.sn = packet->sn;
    it.mac = mac;

    return DbMacs::bulid()->insertItem(it);
}

bool Ad_Modbus::appendLogItem(bool pass)
{
    sStateItem it;

    sDataPacket *packet = sDataPacket::bulid();
    it.dev = packet->dev_type.split("_").first();
    it.op = user_land_name();
    it.user = mItem->user;
    it.sn = packet->sn;
    if(pass) {
        it.result = tr("通过");
    } else {
        it.result = tr("失败");
    }
    it.memo = packet->status;

    bool ret = true;
    if(it.sn.size()) {
        mLogItems << it;
    } else {
        ret = false;
    }

    return ret;
}

void Ad_Modbus::writeLogs()
{
    Db_Tran db;
    for(int i=0; i<mLogItems.size(); ++i) {
        DbStates::bulid()->insertItem(mLogItems.at(i));
    }
    mLogItems.clear();
}

void Ad_Modbus::initSerial()
{
    if(!mSerial) {
        mSerial = mItem->serial;
    }
}

int Ad_Modbus::readSerial(quint8 *cmd, int secs)
{
    int rtn = 0;
    initSerial();
    for(int i=0; i<=secs; ++i) {
        if(mItem->step) {
            rtn = mSerial->read(cmd, 1);
            if(rtn > 0) break;
        }
    }

    return rtn;
}

bool Ad_Modbus::writeSerial(quint8 *cmd, int len)
{
    initSerial();
    bool ret = mSerial->isOpened();
    if(ret) {
        mSerial->write(cmd, len);
    } else {
        qDebug() << "AdjustCoreThread writeSerial err !" << ret;
    }

    return ret;
}

int Ad_Modbus::transmit(uchar *sent, int len, uchar *recv, int secs)
{
    int rtn = 0;
    bool ret = writeSerial(sent, len);
    if(ret) {
        rtn = readSerial(recv, secs);
    }

    return rtn;
}

bool Ad_Modbus::rtuRecvCrc(uchar *buf, int len)
{
    bool ret = true;
    int rtn = len-2; uchar *ptr = buf+rtn;
    if(rtn < 0) return false;

    ushort crc = (ptr[1]*256) + ptr[0]; // 获取校验码
    ushort res = rtu_crc(buf, rtn);
    if(crc != res) {
        ret = false;
        qDebug() << "Rtu Recv rtu recv crc Err!";
    }

    return ret;
}


bool Ad_Modbus::rtuRecvCrc(uchar *buf, int len, sRtuReplyItem *msg)
{
    bool ret = rtuRecvCrc(buf, len);
    if(ret) {
        int rtn = len-2; uchar *ptr = buf+rtn;
        msg->crc = (ptr[1]*256) + ptr[0]; // 获取校验码
    }

    return ret;
}

int Ad_Modbus::rtuRecvData(uchar *ptr, sRtuReplyItem *pkt)
{
    pkt->addr = *(ptr++);// 从机地址码
    pkt->fn = *(ptr++);  /*功能码*/
    pkt->len = *(ptr++); /*数据长度*/
    if(0 == pkt->len) pkt->len = *(ptr++); /*SI-PDU数据长度是二个字节*/
    if(pkt->len < MODBUS_RTU_SIZE) {
        for(int i=0; i<pkt->len; ++i) {
            pkt->data[i] = *(ptr++);
        }
    } else {
        pkt->len = 0;
    }

    return pkt->len;
}


int Ad_Modbus::rtuRead(sRtuItem *pkt, sRtuReplyItem *recv)
{
    static uchar sendBuf[64]={0}, recvBuf[256]={0};
    int rtn = rtu_sent_packet(pkt, sendBuf);
    rtn = transmit(sendBuf, rtn, recvBuf, 2);
    if(rtn > 0) {
        bool ret = rtuRecvCrc(recvBuf, rtn, recv);
        if(ret) {
            rtn = rtuRecvData(recvBuf, recv);
        } else {
            rtn = 0;
        }
    }

    return rtn;
}

int Ad_Modbus::rtuRead(sRtuItem *pkt, uchar *recv)
{
    static sRtuReplyItem item;
    int ret = rtuRead(pkt, &item);
    if(ret > 0) {
        for(int i=0; i<ret; ++i) {
            recv[i] = item.data[i];
        }
    }

    return ret;
}

bool Ad_Modbus::rtuWrites(sRtuSetItems *pkt)
{
    bool ret = false;
    static uchar sendBuf[256]={0}, recvBuf[128]={0};
    int rtn = rtu_write_packets(pkt, sendBuf);
    rtn = transmit(sendBuf, rtn, recvBuf, 2);
    if(rtn > 0) {
        ret = rtuRecvCrc(recvBuf, rtn);
        if(ret) {
            uchar fn = recvBuf[1];
            if(fn < 0x80) { // 设置正常
                ret = true;
            }
        }
    }
    return ret;
}

bool Ad_Modbus::rtuWrite(sRtuSetItem *pkt)
{
    bool ret = false;
    static uchar sendBuf[256]={0}, recvBuf[128]={0};
    int rtn = rtu_write_packet(pkt, sendBuf);
    rtn = transmit(sendBuf, rtn, recvBuf, 2);
    if(rtn > 0) {
        ret = rtuRecvCrc(recvBuf, rtn);
        if(ret) {
            uchar fn = recvBuf[1];
            if(fn < 0x80) { // 设置正常
                ret = true;
            }
        }
    }
    return ret;
}

int Ad_Modbus::rtu_write_packet(sRtuSetItem *pkt, uchar *ptr)
{
    uchar *buf = ptr;
    *(ptr++) = pkt->addr;  /*地址码*/
    *(ptr++) = pkt->fn; /*功能码*/

    /*填入寄存器首地址*/
    *(ptr++) = ((pkt->reg) >> 8); /*高8位*/
    *(ptr++) = (0xff)&(pkt->reg); /*低8位*/

    *(ptr++) = ((pkt->data) >> 8);
    *(ptr++) = (0xff)&(pkt->data);

    /*填入CRC*/
    pkt->crc = rtu_crc(buf, 6);
    *(ptr++) = (0xff)&(pkt->crc); /*低8位*/
    *(ptr++) = ((pkt->crc) >> 8); /*高8位*/

    return 8;
}

int Ad_Modbus::rtu_write_packets(sRtuSetItems *pkt, uchar *ptr)
{
    uchar *buf = ptr;
    *(ptr++) = pkt->addr;  /*地址码*/
    *(ptr++) = pkt->fn; /*功能码*/

    /*填入寄存器首地址*/
    *(ptr++) = ((pkt->reg) >> 8); /*高8位*/
    *(ptr++) = (0xff)&(pkt->reg); /*低8位*/

    *(ptr++) = ((pkt->num) >> 8); /*长度高8位*/
    *(ptr++) = (0xff)&(pkt->num); /*低8位*/
    *(ptr++) = pkt->len;

    for(int i=0; i<pkt->len; ++i) {
        *(ptr++) = pkt->data[i];
    }

    /*填入CRC*/
    pkt->crc = rtu_crc(buf, 7+pkt->len);
    *(ptr++) = (0xff)&(pkt->crc); /*低8位*/
    *(ptr++) = ((pkt->crc) >> 8); /*高8位*/

    return 9 + pkt->len;
}



/**
  * 功　能：发送数据打包
  * 入口参数：pkt -> 发送结构体
  * 出口参数：ptr -> 缓冲区
  * 返回值：打包后的长度
  */
int Ad_Modbus::rtu_sent_packet(sRtuItem *pkt, uchar *ptr)
{
    uchar *buf = ptr;
    *(ptr++) = pkt->addr;  /*地址码*/
    *(ptr++) = pkt->fn; /*功能码*/

    /*填入寄存器首地址*/
    *(ptr++) = ((pkt->reg) >> 8); /*高8位*/
    *(ptr++) = (0xff)&(pkt->reg); /*低8位*/

    /*填入数据长度*/
    *(ptr++) = ((pkt->num) >> 8); /*长度高8位*/
    *(ptr++) = (0xff)&(pkt->num); /*低8位*/

    /*填入CRC*/
    pkt->crc = rtu_crc(buf, 6);
    *(ptr++) = (0xff)&(pkt->crc); /*低8位*/
    *(ptr++) = ((pkt->crc) >> 8); /*高8位*/

    return 8;
}

ushort Ad_Modbus::calccrc (ushort crc, uchar crcbuf)
{
    uchar x, kkk=0;
    crc = crc^crcbuf;
    for(x=0;x<8;x++)
    {
        kkk = crc&1;
        crc >>= 1;
        crc &= 0x7FFF;
        if(kkk == 1)
            crc = crc^0xa001;
        crc=crc&0xffff;
    }
    return crc;
}

/**
  * 功　能：CRC校验
  * 入口参数：buf -> 缓冲区  len -> 长度
  * 返回值：CRC
  */
ushort Ad_Modbus::rtu_crc(uchar *buf, int len)
{
    ushort crc = 0xffff;
    for(int i=0; i<len; i++)
        crc = calccrc(crc, buf[i]);
    return crc;
}

/**
  * 功　能：XOR和校验
  * 入口参数：buf -> 缓冲区  len -> 长度
  * 返回值：XOR
  */
uchar Ad_Modbus::getXorNumber(uchar *buf, int len)
{
    uchar xorsum = 0x00;
    for(int i=0; i<len; i++)
        xorsum ^= buf[i];
    return xorsum;
}


bool Ad_Modbus::changeBaudRate()
{
    int br = mSerial->baudRate();
    switch (br) {
    case 9600: br = 19200; break;
    case 19200: br = 9600; break;
    default: return false;
    }
    return mSerial->setBaudRate(br);;
}
