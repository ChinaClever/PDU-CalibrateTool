/*
 *
 *  Created on: 2020年10月1日
 *      Author: Lzy
 */
#include "datapacket.h"
#include "ad_config.h"

sDataPacket::sDataPacket()
{
    tg = new sTgObjData;
    data = new sDataUnit;
    devType = new sDevType;
    // clear();
    devType->specs = 0;
    devType->devType = 0;
    pro = new sProgress();
    for(int i=0; i<DEV_NUM; ++i) {
        dev[i] = new sDevData;
        clear(i);
    }
}

void sDataPacket::clear(int id)
{
    sDevData *ptr = dev[id];
    ptr->devType.ac = 1;
    ptr->devType.devType = 0;
    ptr->devType.version = 0;
    uchar sp = devType->specs;
    uchar dt = devType->devType;
    memset(tg, 0, sizeof(sTgObjData));
    memset(data, 0, sizeof(sDataUnit));
    memset(devType, 0, sizeof(sDevType));

    devType->devType = dt;
    devType->specs = sp;
    dev_type.clear();
    status.clear();
    sn.clear();

    pro->step = 0;
    pro->result = 0;
    pro->pass.clear();
    pro->itPass.clear();
    pro->item.clear();
    pro->status.clear();
    pro->startTime = QTime::currentTime();

    pro->productType.clear();
    pro->productSN.clear();
    // pro->macAddress.clear();
    pro->clientName.clear();
    pro->testTime.clear();
    pro->softwareType = "PDU-CalibrateTool";
    pro->companyName = "clever";
    pro->protocolVersion = "V1.0";
    // pro->testStartTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    pro->testEndTime.clear();
    pro->no.clear();
    pro->itemName.clear();
    pro->uploadPass.clear();
    pro->softwareVersion.clear();
    pro->PCB_Code.clear();
}


sDataPacket *sDataPacket::bulid()
{
    static sDataPacket* sington = nullptr;
    if(sington == nullptr)
        sington = new sDataPacket();
    return sington;
}

int sDataPacket::tgCur()
{
    int ret = 0;
    for(int i=0; i<data->size; ++i) {
        ret += data->cur[i];
    }

    return ret;
}

void sDataPacket::init()
{
    clear();
    pro->step = Test_Over;
    pro->result = Test_Info;
    pro->startTime = QTime::currentTime();

    pro->softwareType = "PDU-CalibrateTool";
    pro->companyName = "clever";
    pro->protocolVersion = "V1.0";
    pro->testStartTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");

}

bool sDataPacket::updatePro(const QString &str, bool pass, int sec)
{
    pro->time = QTime::currentTime().toString("hh:mm:ss");
    pro->pass << pass;
    pro->itPass << pass;

    pro->item << str;
    pro->status << str;

    pro->uploadPass << pass;
    pro->itemName << str;

    if(pass) pass = delay(sec);
    else pro->result = Test_Fail;

    return pass;
}


bool sDataPacket::delay(int s)
{
    bool ret = true;
    for(int i=0; i<s; ++i) {
        if((pro->step < Test_Over)  || (pro->step > Test_End)){
            QThread::msleep(100);
        } else {
            ret = false;
            break;
        }
    }

    return ret;
}
