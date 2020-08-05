/*
 *
 *  Created on: 2020年10月1日
 *      Author: Lzy
 */
#include "col_ipthread.h"

#define IP_LINE_NUM 3
#define IP_RTU_ONE_LEN 49  // 单相数据长度
#define IP_RTU_THREE_LEN 58  // 三相数据长度

Col_IpThread::Col_IpThread(QObject *parent) : Col_CoreThread(parent)
{

}

Col_IpThread *Col_IpThread::bulid(QObject *parent)
{
    Col_IpThread* sington = nullptr;
    if(sington == nullptr)
        sington = new Col_IpThread(parent);
    return sington;
}

int Col_IpThread::recvLine(int len)
{
    int ret = 0;
    switch (len/2) {
    case IP_RTU_ONE_LEN: ret = 1; break;
    case IP_RTU_THREE_LEN: ret = 3; break;
    default: qDebug() << "IP rtu recv len Err!!" << len; break;
    }

    return ret;
}


void Col_IpThread::initRtuItem(sRtuItem &it)
{
    it.addr = mItem->addr;
    it.fn = 0x03;
    it.reg = 0;
    it.num = IP_RTU_ONE_LEN;//IP_RTU_THREE_LEN;

    uchar res = sDataPacket::bulid()->devType->series;
    if(3 == res) it.num = IP_RTU_THREE_LEN;  // V3
}


uchar *Col_IpThread::getSwitch(uchar *ptr, int line, uchar *value)
{
    ptr = getShort(ptr, line, value);
    for(int i=0; i<line; ++i) {
        if(value[i] == 2) value[i] = 1; else value[i] = 0;
    }

    return ptr;
}

/**
  * 功　能：读取电参数 数据
  * 入口参数：ptr -> 缓冲区
  * 出口参数：pkt -> 结构体
  * 返回值：12 正确
  */
int Col_IpThread::recvDataV1(uchar *ptr, sDataUnit *msg)
{
    uchar *ret = ptr;

    int line = IP_LINE_NUM;
    ptr =  toShort(ptr, line, msg->vol);
    ptr =  toShort(ptr, line, msg->cur);
    ptr =  toShort(ptr, line, msg->pow);
    ptr += (2*line); // 频率  //  ptr =  touChar(ptr, line, msg->hz);
    ptr =  toInt(ptr, line, msg->ele);
    ptr += 4 + 24 + 8; // 温度 湿度
    ptr += (2*2*line + 4); // 报警标志位

    ptr = getSwitch(ptr, 2, msg->sw);
    msg->size = getShort(ptr); ptr +=2;
    msg->version = getShort(ptr); ptr +=2;
    msg->br = getShort(ptr); ptr +=2;
    msg->reserve = getShort(ptr); ptr +=2;

    return ptr-ret;
}


/**
  * 功　能：读取电参数 数据
  * 入口参数：ptr -> 缓冲区
  * 出口参数：pkt -> 结构体
  * 返回值：12 正确
  */
int Col_IpThread::recvDataV3(uchar *ptr, sDataUnit *msg)
{
    uchar *ret = ptr;

    int line = IP_LINE_NUM;
    ptr =  toShort(ptr, line, msg->vol);
    ptr =  toShort(ptr, line, msg->cur);
    ptr =  getShort(ptr, line, msg->pf);
    ptr =  toShort(ptr, line, msg->pow);
    ptr =  toShort(ptr, line, msg->activePow);
    ptr =  toInt(ptr, line, msg->ele);

    msg->hz = getShort(ptr); ptr +=2;
    ptr = getSwitch(ptr, line, msg->sw); // 开关状态
    ptr += 4 + 12*2 + 2*4; // 温度 湿度
    ptr += (2*2*line + 2 + 2); // 报警

    msg->size = getShort(ptr); ptr +=2;
    msg->version = getShort(ptr); ptr +=2;
    msg->br = getShort(ptr); ptr +=2;

    ptr = toChar(ptr, 8, msg->ip);
    msg->reserve = getShort(ptr); ptr +=2;

    return ptr-ret;
}


bool Col_IpThread::recvPacket(uchar *buf, int len)
{
    bool ret = true;
    int line = recvLine(len);
    switch (line) {
    case 1: recvDataV1(buf, mData); break;
    case 3: recvDataV3(buf, mData); break;
    default: ret = false; break;
    }

    return ret;
}

bool Col_IpThread::readPduData()
{
    static sRtuItem it;
    static uchar recv[MODBUS_RTU_SIZE] = {0};

    mData->rate = 1;
    initRtuItem(it);

    int len = mModbus->rtuRead(&it, recv);
    return recvPacket(recv, len);
}
