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


bool Ad_AutoID::readDevValue(int size , QString & valStr)
{
    mPacket->status = tr("正在读取校准值！");
    bool ret = readDevVal(size, valStr);
    if(!ret){
        for(int i = 0 ; i < size ; ++i){
            valStr += "0";
            if(i != size - 1) valStr += "/";
        }
    }


    return ret;
}

bool Ad_AutoID::readDevVal(int size , QString & valStr)
{
    sRtuItem it;
    initReadCalibrationsVal(it , size);

    int len = 0;
    static uchar recv[255] = {0};
    for(int i=0; i<=6; ++i) {
        if(i>1) mPacket->status = tr("第%1次读取设备校准值 ").arg(i);
        len = mModbus->rtuRead(&it, recv);
        if(len==size*4) break; else if(!mModbus->delay(1)) break;
    }

    return analysDevCalibrationValue(recv, len , size , valStr);
}

void Ad_AutoID::initReadCalibrationsVal(sRtuItem &it , int size)
{
    it.addr = mItem->addr;
    it.fn = 0x03;
    it.reg = 0xA020;
    it.num = 2*size;
}

bool Ad_AutoID::analysDevCalibrationValue(uchar *buf, int len ,int size , QString &valStr)
{
    bool ret = false;
    QString str;
    if(len != size*4) {
        str = tr("读取设备校准值失败：返回长度为%1").arg(len);
        return ret;
    }
    str = tr("读取设备校准值成功");
    uint id = 0;
    ret = true;
    for(int i = 0; i < len; i += 4) {
        id = 0;
        id += buf[i]*256 + buf[i+1];
        id <<= 16;
        id += buf[i+2]*256 + buf[i+3];
        if(i != len - 4) valStr += QString::number(id)+"/";
    }

    return ret;
}
