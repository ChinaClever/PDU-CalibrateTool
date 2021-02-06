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
    mDt = mPacket->devType;
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
    if(len != 4) {
        mPacket->status = tr("通讯错误，未能正确识别执行板/表头 %1").arg(len);
        return false;
    }

    uint id = 0;
    for(int i=0; i<len; ++i) {
        id *= 256;  id += buf[i];
    }

    bool ret = mDevType->analysDevType(id);
    if(!ret){
        mPacket->status = QObject::tr("不支持此设备类型 ID是%1").arg(id);
    }

    return ret;
}

bool Ad_AutoID::readDevId()
{
    sRtuItem it;
    initReadType(it);

    int len = 0;
    uchar recv[8] = {0};
    for(int i=0; i<=6; ++i) {
        if(i>1) mPacket->status = tr("第%1次读取设备ID ").arg(i);
        len = mModbus->rtuRead(&it, recv);
        if(len) break; else if(!mModbus->delay(1)) break;
        if(i>2) mModbus->changeBaudRate(); mModbus->delay(1);
    }

    return analysDevType(recv, len);
}

bool Ad_AutoID::readDevType()
{
    mPacket->status = tr("正在识别模块类型！");
    bool ret = readDevId();
    if(ret) {
        mPacket->status = tr("识别模块成功！");
        if((IP_PDU == mDt->devType) && (DC == mDt->ac)){
            mPacket->status = tr("读取IP模块代号！");
            ret = mModbus->delay(7);
            ret = readDevId();
        }
    }else{
        mItem->step = Test_End;
        mPacket->pass = Test_Fail;
    }

    return ret;
}

