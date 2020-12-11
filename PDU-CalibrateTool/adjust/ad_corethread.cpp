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
    mSource = nullptr;
    mPacket =sDataPacket::bulid();
    mItem = Ad_Config::bulid()->item;
    mDt = mPacket->devType;

    mModbus = Ad_Modbus::bulid(this);
    mAutoID = Ad_AutoID::bulid(this);
    mAdjust = Ad_Adjusting::bulid(this);
    mResult = Ad_Resulting::bulid(this);
    mSn = SN_ManageThread::bulid(this);
}

Ad_CoreThread::~Ad_CoreThread()
{
    if(isRun) {
        isRun = false;
        mPacket->status = tr("正在关闭标准源");
        if(mSource) mSource->powerDown();
        mItem->step = Test_Over;
    }
    wait();
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

bool Ad_CoreThread::initThread()
{
    bool ret = false;
    Col_CoreThread *th = mResult->initThread();
    if(th) {
        ret = th->readPduData();
        if(ret) ret = mSn->snEnter();//写入序列号
        else ret = readDevInfo();
    }

    return ret;
}

void Ad_CoreThread::collectData()
{
    mPacket->status = tr("数据采集");
    bool ret = initThread();
    if(!ret)  return;

    Col_CoreThread *th = mResult->initThread();
    while(mItem->step != Test_Over) {
        th->readPduData();
        mResult->resTgData();
        delay(2);
    }
}

void Ad_CoreThread::verifyResult()
{
    mPacket->status = tr("采集自动验证");
    bool ret = initThread();
    if(ret) {
        mResult->resEnter();
        mItem->step = Test_End;  // 结束验证
    }
}

void Ad_CoreThread::writeLog()
{
    Db_Tran db;
    sLogItem it;

    it.dev =mPacket->dev_type.split("_").first();
    it.op = user_land_name();
    it.user = mItem->user;
    it.sn = mPacket->sn;
    if(it.sn.isEmpty()) return;

    mItem->cnt.all += 1;
    if(mPacket->pass == Test_Success) {
        it.result = tr("通过");
        mItem->cnt.ok += 1;
    } else {
        mItem->cnt.err += 1;
        it.result = tr("失败：%1").arg(mPacket->status);
    }

    DbLogs::bulid()->insertItem(it);
    Ad_Config::bulid()->writeCnt();
}

bool Ad_CoreThread::initSource()
{
    bool ret = false;
    mPacket->status = tr("已启动校准！连接标准源");
    mSource = mResult->initStandSource();
    if(mSource) {
        mPacket->status = tr("标准源上电中");
        ret = mSource->setVol(220, 6);

        mPacket->status = tr("等待设备稳定！");
        ret = mModbus->delay(6);//IP-PDU三相启动慢
        if(ret) mSource->setCur(60, 0);
    } else {
        mItem->step = Test_End;
    }

    return ret;
}

bool Ad_CoreThread::readDevInfo()
{
    bool ret = initSource();
    if(mSource) {
        ret = mAutoID->readDevType();//读取设备类型
        if(ret){
            if(DC == mDt->ac) mSource->setCur(0, 0);
            ret = mSn->snEnter();//写入序列号
        }
        mModbus->appendLogItem(ret);  // 序列号操作成功，才能记录日志
    } else {
        mItem->step = Test_End;
    }

    return ret;
}

void Ad_CoreThread::workDown()
{
    mPacket->clear();
    bool ret = readDevInfo(); if(!ret) return;
    ret = mAdjust->startAdjust(mSource);
    if(ret) {
        mPacket->status = tr("开始自动验证");
        ret = mResult->resEnter();
    }
    writeLog(); msleep(20);  //记录校准设备校准成功还是校准失败
    mItem->step = Test_End;
}

void Ad_CoreThread::run()
{
    if(!isRun) {
        isRun = true;
        mPacket->pass = 0;

        switch (mItem->step) {
        case Test_Start: workDown(); break;
        case Collect_Start: collectData(); break;
        case Test_vert: verifyResult(); break;
        }

        if(mSource) mSource->powerDown();
        mModbus->writeLogs();
        isRun = false;
    } else {
        qDebug() << "AdjustCoreThread run err" << isRun;
    }
}


