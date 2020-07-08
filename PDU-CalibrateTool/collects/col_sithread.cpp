/*
 *
 *  Created on: 2020年10月1日
 *      Author: Lzy
 */
#include "col_sithread.h"

#define SI_RTU_ONE_LEN 0x21  // 单相数据长度
#define SI_RTU_THREE_LEN 0x51  // 三相数据长度

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
}

uchar *Col_SiThread::toInt(uchar *ptr, int line, uint *value)
{
    for(int i=0; i<line; ++i) {
        value[i] =  (*ptr) * 256 + *(ptr+1);  ptr += 2; // 读取电能高8位
        value[i] <<= 8; // 左移8位
        value[i] +=  (*ptr) * 256 + *(ptr+1);  ptr += 2; // 读取电能底8位
    }

    return ptr;
}

uchar *Col_SiThread::toShort(uchar *ptr, int line, ushort *value)
{
    for(int i=0; i<line; ++i) {
        value[i] =  (*ptr) * 256 + *(ptr+1);  ptr += 2;
    }

    return ptr;
}

uchar *Col_SiThread::toChar(uchar *ptr, int line, uchar *value)
{
    for(int i=0; i<line; ++i) {
        value[i] =  *(ptr++); // 读取电压
    }

    return ptr;
}

uchar *Col_SiThread::toThreshold(uchar *ptr, int line, sThreshold &unit)
{
    for(int i=0; i<line; ++i) {
        ptr = toChar(ptr, 1, &unit.min[i]);
        ptr = toChar(ptr, 1, &unit.max[i]);
    }

    return ptr;
}

/**
  * 功　能：长度 校验
  * 入口参数：buf -> 缓冲区  len -> 长度
  * 返回值：0 正确
  */
int Col_SiThread::recvLine(int len)
{
    int ret = 0;

    if(len < SI_RTU_ONE_LEN) {
        ret = -1;
        qDebug() << "SI rtu recv Err: len too short!!" << len;
    } else if(len > SI_RTU_THREE_LEN + 7) {
        ret = -2;
        qDebug() << "SI rtu recv Err: len too long!!" << len;
    } else {
        if(len == SI_RTU_ONE_LEN) {
            ret = 1;
        } else if (len == SI_RTU_THREE_LEN) {
            ret = 3;
        } else  {
            ret = -3;
            qDebug() << "SI rtu recv len Err!!" << len;
        }
    }

    return ret;
}

/**
  * 功　能：读取电参数 数据
  * 入口参数：ptr -> 缓冲区
  * 出口参数：pkt -> 结构体
  * 返回值：12 正确
  */
int Col_SiThread::recvData(uchar *ptr, int line, sDataUnit *msg)
{
    int ret = SI_RTU_ONE_LEN;

    ptr = toShort(ptr, line, msg->vol);
    ptr = toShort(ptr, line, msg->cur);
    ptr = toShort(ptr, line, msg->pow);
    ptr = toInt(ptr, line, msg->ele);

    sThreshold th; // 无实际意义 为了解释数据 占位置
    ptr =  toThreshold(ptr, line, th);
    ptr =  toThreshold(ptr, line, th);

    // msg->tem.value[0] =  *(ptr++);// 温度
    // msg->hum.value[0] =  *(ptr++);// 湿度
    ptr =  toThreshold(ptr, line, th);
    if(line > 1) {
        // msg->tem.max[0] =  *(ptr++);
        // msg->tem.min[0] =  *(ptr++);
        ptr =  toThreshold(ptr, line, th);

        // msg->hum.max[0] =  *(ptr++);
        // msg->hum.min[0] =  *(ptr++);
        ptr =  toThreshold(ptr, line, th);

        msg->size =  *(ptr++);
    }

    msg->br =  *(ptr++); // 波特率
    ptr =  toShort(ptr, line, msg->activePow);
    ptr =  toChar(ptr, line, msg->pf); // 功率因数
    ptr =  toChar(ptr, line, msg->sw); // 开关状态

    if(line == 1) {
        msg->size =  *(ptr++);
    } else {
        ret = SI_RTU_THREE_LEN;
    }
    msg->size = line;

    return ret;
}

bool Col_SiThread::recvPacket(uchar *buf, int len)
{
    bool ret = true;
    int line = recvLine(len);
    if(line > 0) {
        recvData(buf, len, mData);
    } else {
        ret = false;
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
