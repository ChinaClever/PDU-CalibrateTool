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
    QString  str = Json_Recv::bulid()->getDevTypeByID(id);

    return str;
}


int Ad_DevType::getDevType(const QString &str)
{
    int ret = 0;
    if(str.contains("ZPDU")) ret = ZPDU;
    if(str.contains("RPDU")) ret = RPDU;
    if(str.contains("MPDU")) {
        ret = MPDU;
        if(str.contains("位")) {
            mPacket->data->size = str.split("_").last().remove("位").toInt();
            mPacket->data->rate = 10;
        }
        if(str.contains("华为")) {
            mPacket->data->size = 8;
            mPacket->data->reserve = 6;
        }
    }

    if(str.contains("BM-PDU")) ret = BM_PDU;
    if(str.contains("SI-PDU")) ret = SI_PDU;
    if(str.contains("IP-PDU")) {
        ret = IP_PDU;
        if(str.contains("SNMPV3")) mDt->version = 3;
    }

    if(str.contains("断路器")) {
        ret = APDU;
        if(str.contains("位")) {
            mPacket->data->size = str.split("_").last().remove("位断路器").toInt();
            mPacket->data->rate = 10;
        }
    }

    return ret;
}

int Ad_DevType::getAcType(const QString &str)
{
    int ret = AC;
    if(str.contains("直流")) ret = DC;

    return ret;
}

int Ad_DevType::getColMode(const QString &str)
{
    int ret = Transformer;
    if(str.contains("锰铜")) ret = Mn;
    if(mDt->devType > RPDU) ret = Transformer;

    return ret;
}

int Ad_DevType::getLineNum(const QString &str)
{
    int ret = 1;
    if(str.contains("三相")) ret = 3;
    if(str.contains("两路")) ret = 2;

    return ret;
}


int Ad_DevType::getSerie(const QString &str)
{
    int ret = 0;
    if(str.contains("A系列")) ret = 1;
    if(str.contains("B系列")) ret = 2;
    if(str.contains("C系列")) ret = 3;
    if(str.contains("B/D系列")) ret = 4;

    return ret;
}

bool Ad_DevType::analysDevType(uint id)
{
    bool ret = true;
    QString str = getDevStr(id);
    if(str.size()) {
        mPacket->dev_type = str;
        mDt->devId = id;
        mDt->devType = getDevType(str);
        mDt->ac = getAcType(str);
        mDt->specs = getColMode(str);
        mDt->series = getSerie(str);
        mDt->lines = getLineNum(str);
    } else {
        ret = false;
    }

    return ret;
}


