/*
 *
 *  Created on: 2020年10月1日
 *      Author: Lzy
 */
#include "col_corethread.h"

Col_CoreThread::Col_CoreThread(QObject *parent) : QThread(parent)
{
    mData = sDataPacket::bulid()->data;
    mItem = Ad_Config::bulid()->item;
    mModbus = Ad_Modbus::bulid(this);
}


uchar *Col_CoreThread::toInt(uchar *ptr, int line, uint *value)
{
    for(int i=0; i<line; ++i) {
        value[i] =  (*ptr) * 256 + *(ptr+1);  ptr += 2; // 读取电能高8位
        value[i] <<= 16; // 左移8位
        value[i] +=  (*ptr) * 256 + *(ptr+1);  ptr += 2; // 读取电能底8位
    }

    return ptr;
}

uchar *Col_CoreThread::toOutputEle(uchar *ptr, int line, uint *value)
{
    for(int i=0; i<line; ++i) {
        value[i] = (*ptr++) << 16;
        value[i] += (*ptr++) << 8;
        value[i] += *ptr++;
    }

    return ptr;
}

uchar *Col_CoreThread::toShort(uchar *ptr, int line, ushort *value)
{
    for(int i=0; i<line; ++i) {
        value[i] =  (*ptr) * 256 + *(ptr+1);  ptr += 2;
    }

    return ptr;
}

uchar *Col_CoreThread::toChar(uchar *ptr, int line, uchar *value)
{
    for(int i=0; i<line; ++i) {
        value[i] = *(ptr++); // 读取电压
    }

    return ptr;
}

uchar *Col_CoreThread::toThreshold(uchar *ptr, int line, sThreshold &unit)
{
    for(int i=0; i<line; ++i) {
        ptr = toChar(ptr, 1, &unit.min[i]);
        ptr = toChar(ptr, 1, &unit.max[i]);
    }

    return ptr;
}

ushort Col_CoreThread::getShort(uchar *ptr)
{
    return  (*ptr) * 256 + *(ptr+1);
}

uchar *Col_CoreThread::getShort(uchar *ptr, int line, uchar *value)
{
    for(int i=0; i<line; ++i) {
        value[i] = getShort(ptr); ptr += 2;
    }

    return  ptr;
}
