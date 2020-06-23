/*
 *
 *  Created on: 2020年10月1日
 *      Author: Lzy
 */
#include "ad_corethread.h"
#include "dblogs.h"
extern QString user_land_name();

Ad_CoreThread::Ad_CoreThread(QObject *parent) : QThread(parent)
{
    isRun = false;
    mPacket =sDataPacket::bulid();
    mItem = Ad_Config::bulid()->item;

    mModbus = Ad_Modbus::bulid(this);
    mAutoID = Ad_AutoID::bulid(this);
    mAdjust = Ad_Adjusting::bulid(this);
    mResult = Ad_Resulting::bulid(this);
    mSource = YC_StandSource::bulid(this);
    mSn = SN_ManageThread::bulid(this);
}

Ad_CoreThread *Ad_CoreThread::bulid(QObject *parent)
{
    static Ad_CoreThread* sington = nullptr;
    if(sington == nullptr)
        sington = new Ad_CoreThread(parent);
    return sington;
}

void Ad_CoreThread::startAdjust()
{
    if(!isRun) {
        mItem->step = Test_Start;
        start();
    }
}

void Ad_CoreThread::startCollect()
{
    if(!isRun) {
        mItem->step = Collect_Start;
        start();
    }
}

void Ad_CoreThread::startResult()
{
    if(!isRun) {
        mItem->step = Test_vert;
        start();
    }
}


void Ad_CoreThread::collectData()
{
    mPacket->status = tr("数据采集");
    mAutoID->readDevType();
    Col_CoreThread *th = mResult->initThread();

    while(mItem->step != Test_Over) {
        th->readPduData(); delay(2);
    }
}


void Ad_CoreThread::writeLog()
{
    sLogItem it;

    it.dev =mPacket->dev_type.split(" ").first();
    it.user = user_land_name();
    it.sn = mPacket->sn;
    if(mPacket->pass == 1) {
        it.status = tr("通过");
    } else {
         it.status = tr("失败");
    }
    DbLogs::bulid()->insertItem(it);
}


void Ad_CoreThread::workDown()
{
    mSource->powerReset();
    bool ret = delay(3); if(!ret) return;

    ret = mAutoID->readDevType();
    if(!ret) return;
    mSn->snEnter();

    mPacket->status = tr("开始校准");
    ret = mAdjust->startAdjust();
    if(ret){
        mPacket->status = tr("开始自动验证");
        ret = mResult->resEnter();
    }

    mSource->powerDown();
    writeLog();
}

void Ad_CoreThread::run()
{
    if(!isRun) {
        isRun = true;
        mPacket->clear();
        mPacket->pass = 0;

        switch (mItem->step) {
        case Test_Start: workDown(); break;
        case Collect_Start: collectData(); break;
        case Test_vert: mResult->resEnter(); break;
        }

        isRun = false;
    } else {
        qDebug() << "AdjustCoreThread run err" << isRun;
    }
}
