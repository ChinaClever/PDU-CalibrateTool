/*
 *
 *  Created on: 2020年10月1日
 *      Author: Lzy
 */
#include "ad_devtype.h"
#include "json_recv.h"

Ad_DevType::Ad_DevType()
{
    mPacket = sDataPacket::bulid();
    mDt = mPacket->devType;
}


Ad_DevType * Ad_DevType::bulid()
{
    static  Ad_DevType* sington = nullptr;
    if(sington == nullptr)
        sington = new  Ad_DevType();
    return sington;
}

QString Ad_DevType::getDevStr(uint id)
{
    QString str = "ZPDU_磁保持_互感器_C系列_三相_14位";
    //str = Json_Recv::bulid()->getDevTypeByID(id);

    return str;
}


int Ad_DevType::getDevType(const QString &str)
{
    int ret = 0;
    if(str.contains("ZPDU")) ret = 1;
    if(str.contains("MPDU")) ret = 2;
    if(str.contains("RPDU")) ret = 3;

    if(str.contains("SI-PDU")) ret = 4;
    if(str.contains("IP-PDU")) ret = 5;
    if(str.contains("BM-PDU")) ret = 6;

    return ret;
}

int Ad_DevType::getAcType(const QString &str)
{
    int ret = 1;
    if(str.contains("直流")) ret = 2;

    return ret;
}

int Ad_DevType::getColMode(const QString &str)
{
    int ret = 1;
    if(str.contains("锰铜")) ret = 2;

    return ret;
}

int Ad_DevType::getLineNum(const QString &str)
{
    int ret = 1;
    if(str.contains("三相")) ret = 3;

    return ret;
}

bool Ad_DevType::analysDevType(uint id)
{
    bool ret = true;
    QString str = getDevStr(id);
    if(str.size()) {
        mPacket->dev_type = str;
        mDt->devType = getDevType(str);
        mDt->ac = getAcType(str);
        mDt->specs = getColMode(str);
        mDt->lines = getLineNum(str);
    } else {
        ret = false;
    }

    return ret;
}


