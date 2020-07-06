/*
 *
 *  Created on: 2020年10月1日
 *      Author: Lzy
 */
#include "ad_autoid.h"

Ad_AutoID::Ad_AutoID(QObject *parent) : QThread(parent)
{        
    mPacket = sDataPacket::bulid();
    mModbus = Ad_Modbus::bulid(this);
    mItem = Ad_Config::bulid()->item;
    mDevType = Ad_DevType::bulid();
}

Ad_AutoID *Ad_AutoID::bulid(QObject *parent)
{
    static Ad_AutoID* sington = nullptr;
    if(sington == nullptr)
        sington = new Ad_AutoID(parent);
    return sington;
}

void Ad_AutoID::initReadType(sRtuItem &it)
{
    it.addr = mItem->addr;
    it.fn = 0x03;
    it.reg = 0xA001;
    it.num = 2;
}

bool Ad_AutoID::analysDevType(uchar *buf, int len)
{
    if(len != 4) return false;

    uint id = 0;
    for(int i=0; i<len; ++i) {
        id *= 256;
        id += buf[i];
    }

    return mDevType->analysDevType(id);
}

bool Ad_AutoID::readDevType()
{    
    sRtuItem it;
    initReadType(it);

    uchar recv[8] = {0};
    mPacket->status = tr("正在识别模块类型！");
    int len = mModbus->rtuRead(&it, recv);

    bool ret = analysDevType(recv, len);
    if(ret) {
        mPacket->status = tr("识别模块成功！");
    } else {
        mPacket->pass = 2;
        mItem->step = Test_End;
        mPacket->status = tr("识别模块错误！");
    }
    ret = mModbus->delay(1);

    return ret;
}

