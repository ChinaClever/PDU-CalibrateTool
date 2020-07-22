/*
 *
 *  Created on: 2020年10月1日
 *      Author: Lzy
 */
#include "col_rpdumcthread.h"

Col_RpduMcThread::Col_RpduMcThread(QObject *parent) : Col_CoreThread(parent)
{

}

Col_RpduMcThread *Col_RpduMcThread::bulid(QObject *parent)
{
    Col_RpduMcThread* sington = nullptr;
    if(sington == nullptr)
        sington = new Col_RpduMcThread(parent);
    return sington;
}

void Col_RpduMcThread::initRtuItem(sRtuItem &it)
{
    it.addr = mItem->addr;
    it.fn = 0x03;
    it.reg = 0x20C;
    it.num = 6;
}

bool Col_RpduMcThread::recvPacket(uchar *ptr, int len)
{
    int line = 3;
    bool ret = true;
    if(len == 12) {
        ptr = toShort(ptr, line, mData->cur);
        ptr = toShort(ptr, line, mData->vol);

        for(int i=0; i<line; ++i) {
            mData->pow[i] = mData->cur[i] * mData->vol[i];
        }
    } else {
        ret = false;
    }

    return ret;
}

bool Col_RpduMcThread::readPduData()
{
    sRtuItem it;
    uchar recv[MODBUS_RTU_SIZE] = {0};

    mData->rate = 1;
    initRtuItem(it);
    int len = mModbus->rtuRead(&it, recv);
    return recvPacket(recv, len);
}
