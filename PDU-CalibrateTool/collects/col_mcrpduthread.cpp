#include "col_mcrpduthread.h"

Col_McRpduThread::Col_McRpduThread(QObject *parent) : Col_CoreThread(parent)
{

}

Col_McRpduThread *Col_McRpduThread::bulid(QObject *parent)
{
    Col_McRpduThread* sington = nullptr;
    if(sington == nullptr)
        sington = new Col_McRpduThread(parent);
    return sington;
}



void Col_McRpduThread::initRtuItem(sRtuItem &it)
{
    it.addr = mItem->addr;
    it.fn = 0x03;
    it.reg = 0x20C;
    it.num = 6;
}


bool Col_McRpduThread::recvPacket(uchar *ptr, int len)
{
    int line = 3;
    bool ret = true;
    if(len == 12) {
        ptr = toShort(ptr, line, mData->cur);
        ptr = toShort(ptr, line, mData->vol);
    } else {
        ret = false;
    }

    return ret;
}

bool Col_McRpduThread::readPduData()
{
    sRtuItem it;
    uchar recv[MODBUS_RTU_SIZE] = {0};

    mData->rate = 1;
    initRtuItem(it);
    int len = mModbus->rtuRead(&it, recv);
    return recvPacket(recv, len);
}
