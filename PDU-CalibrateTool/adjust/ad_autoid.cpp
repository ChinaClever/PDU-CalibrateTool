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
    mDevType = mPacket->devType;
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


QString Ad_AutoID::devTypeToStr(sDevType *dt)
{
    QString str;

    switch (dt->devType) {
    case 1: str = "ZPDU "; break;
    case 2: str = "MPDU "; break;
    case 3: str = "RPDU "; break;
    }

    if(dt->ac == 2) {
        str += tr("直流 ");
    } else {
        str += tr("交流 ");
    }

    char ch = 'A'-1 + dt->series;
    str += ch + tr("系列 ");

    str += tr("相数 %1").arg(dt->lines);
    str += tr("输出位/回路 %1").arg(dt->outputs);

    if(1 == dt->specs) {
        str += tr("互感器 ");
    } else {
        str += tr("锰铜 ");
    }

    str += tr("V_%1").arg(dt->version);
    return str;
}


bool Ad_AutoID::analysDevType(uchar *buf, int len, sDevType *dt)
{
    if(len != 4) return false;

    dt->devType = buf[0];
    dt->ac = buf[1] >> 4;
    dt->series = buf[1] & 0x0F;

    dt->lines = buf[2] >> 4;
    dt->outputs = buf[2] & 0x0F;

    dt->specs = buf[3] >> 4;
    dt->version = buf[3] & 0x0F;

    mPacket->dev_type = devTypeToStr(dt);

    return true;
}

bool Ad_AutoID::readDevType()
{    
    sRtuItem it;
    initReadType(it);

    uchar recv[8] = {0};
    mPacket->status = tr("正在识别模块类型！");    
    int len = mModbus->rtuRead(&it, recv);
    bool ret = analysDevType(recv, len, mDevType);
    if(ret) {
        mPacket->status = tr("识别模块成功！");
    } else {
        mItem->step = Test_End;
        mPacket->status = tr("识别模块错误！");
    }
    ret = mModbus->delay(1);

    return ret;
}

