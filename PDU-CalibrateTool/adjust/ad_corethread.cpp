/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "ad_corethread.h"

Ad_CoreThread::Ad_CoreThread(QObject *parent) : QThread(parent)
{
    isRun = false;
    mPacket =sDataPacket::bulid();
    mItem = Ad_Config::bulid()->item;

    mModbus = Ad_Modbus::bulid(this);
    mAutoID = Ad_AutoID::bulid(this);
    mAdjust = Ad_Adjusting::bulid(this);
    mResult = Ad_Resulting::bulid(this);
    mSource = Ad_StandSource::bulid(this);
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

void Ad_CoreThread::collectData()
{
    mPacket->status = tr("数据采集");
    mAutoID->readDevType();
    Col_CoreThread *th = mResult->initThread();

    while(mItem->step != Test_Over) {
        th->readPduData(); delay(2);
    }
}

void Ad_CoreThread::workDown()
{
    mSource->powerReset();
    bool ret = delay(3); if(!ret) return;

    ret = mAutoID->readDevType();
    if(!ret) return;

    mPacket->status = tr("开始校准");
    ret = mAdjust->startAdjust();
    if(!ret) return;

    mPacket->status = tr("开始自动验证");
    ret = mResult->resEnter();

    mSource->powerDown();
}

void Ad_CoreThread::run()
{
    if(!isRun) {
        isRun = true;
        mPacket->clear();
        mPacket->pass = 0;

        if(Collect_Start == mItem->step) {
            collectData();
        } else {
            workDown();
        }
        isRun = false;
    } else {
        qDebug() << "AdjustCoreThread run err" << isRun;
    }
}
