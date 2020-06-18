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
