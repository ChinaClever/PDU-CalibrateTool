#include "col_corethread.h"

Col_CoreThread::Col_CoreThread(QObject *parent) : QThread(parent)
{
    mData = sDataPacket::bulid()->data;
    mItem = AdjustConfig::bulid()->item;
    mModbus = Ad_Modbus::bulid(this);
}
