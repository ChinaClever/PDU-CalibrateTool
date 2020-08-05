/*
 *
 *  Created on: 2020年10月1日
 *      Author: Lzy
 */
#include "col_sithread.h"

#define SI_RTU_DC_LEN 0x1B  // 直流数据长度
#define SI_RTU_THREE_LEN 0x51  // 三相数据长度//单相时，L2和L3表示C1和C2

Col_SiThread::Col_SiThread(QObject *parent) : Col_CoreThread(parent)
{

}

Col_SiThread *Col_SiThread::bulid(QObject *parent)
{
    Col_SiThread* sington = nullptr;
    if(sington == nullptr)
        sington = new Col_SiThread(parent);
    return sington;
}

void Col_SiThread::initRtuItem(sRtuItem &it)
{
    it.addr = mItem->addr;
    it.fn = 0x03;
    it.reg = 0;
    it.num = SI_RTU_THREE_LEN;
    uchar res = sDataPacket::bulid()->devType->ac;
    if(DC == res) it.num = SI_RTU_DC_LEN;
}

/**
  * 功　能：长度 校验
  * 入口参数：buf -> 缓冲区  len -> 长度
  * 返回值：0 正确
  */
int Col_SiThread::recvLine(int len)
{
    int ret = 0;
    switch (len) {
    case SI_RTU_DC_LEN: ret = 1; break;
    case SI_RTU_THREE_LEN: ret = 3; break;
    default: qDebug() << "SI rtu recv len Err!!" << len; break;
    }

    return ret;
}

int Col_SiThread::recvDcData(uchar *ptr, int line, sDataUnit *msg)
{
    ptr = toShort(ptr, line, msg->vol);
    ptr = toShort(ptr, line, msg->cur);
    ptr = toShort(ptr, line, msg->pow);
    ptr = toInt(ptr, line, msg->ele);
    ptr += 8 + 2; // 阈值 湿湿度

    msg->br = *(ptr++); // 波特率
    ptr = toShort(ptr, line, msg->activePow);
    ptr = toChar(ptr, line, msg->pf); // 功率因数
    ptr = toChar(ptr, line, msg->sw); // 开关状态
    msg->size = *(ptr++);
    msg->hz = *(ptr++);

    if(msg->size == 0) {
        msg->size = 1;
    }

    return SI_RTU_DC_LEN;
}


/**
  * 功　能：读取电参数 数据
  * 入口参数：ptr -> 缓冲区
  * 出口参数：pkt -> 结构体
  * 返回值：12 正确
  */
int Col_SiThread::recvAcData(uchar *ptr, int line, sDataUnit *msg)
{
    ptr = toShort(ptr, line, msg->vol);
    ptr = toShort(ptr, line, msg->cur);
    ptr = toShort(ptr, line, msg->pow);
    ptr = toInt(ptr, line, msg->ele);
    ptr += 24 + 6; // 阈值 湿湿度

    msg->size = *(ptr++);
    msg->br = *(ptr++); // 波特率
    ptr = toShort(ptr, line, msg->activePow);
    ptr = toChar(ptr, line, msg->pf); // 功率因数
    ptr = toChar(ptr, line, msg->sw); // 开关状态
    msg->hz = *(ptr++);

    if(msg->size > 1) {
        msg->size = 3;
    }

    return SI_RTU_THREE_LEN;
}

bool Col_SiThread::recvPacket(uchar *buf, int len)
{
    bool ret = true;
    int line = recvLine(len);
    switch (line) {
    case 1: recvDcData(buf, line, mData); break;
    case 3: recvAcData(buf, line, mData); break;
    default: ret = false; break;
    }

    return ret;
}

bool Col_SiThread::readPduData()
{
    static sRtuItem it;
    static uchar recv[MODBUS_RTU_SIZE] = {0};

    mData->rate = 1;
    initRtuItem(it);

    int len = mModbus->rtuRead(&it, recv);
    return recvPacket(recv, len);
}
