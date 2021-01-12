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

    mLedSi = Ad_LedSi::bulid(this);
    mModbus = Ad_Modbus::bulid(this);
    mAutoID = Ad_AutoID::bulid(this);
    mAdjust = Ad_Adjusting::bulid(this);
    mResult = Ad_Resulting::bulid(this);
    mSn = SN_ManageThread::bulid(this);
    mJig = Ctrl_JigThread::bulid(this);
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
    mPacket->status = tr("给标准源上电中！");
    Col_CoreThread *th = mResult->initThread();
    if(mSource && th) ret = mSource->setVol(200, 6);
    else {ret = readDevInfo(); return ret;}

    if(th) {
        for(int i=0; i<6; i++) {
            if(i) mPacket->status = tr("读取设备数据 %1").arg(i);
            ret = th->readPduData();
            if(ret) break; else if(!delay(3)) break;
            if(i>1 && i%2) th->changeBaudRate();
        }
    }

    if(mItem->step != Test_Over)
        if(!ret) ret = readDevInfo();

    return ret;
}

void Ad_CoreThread::collectData()
{
    bool ret = initThread();
    if(ret) mPacket->status = tr("正在读取设备数据");
    else {mItem->step = Test_End; return;}

    Col_CoreThread *th = mResult->initThread();
    while(mItem->step != Test_Over) {
        th->readPduData();
        mResult->resTgData();
        delay(2);
    }
}

void Ad_CoreThread::verifyResult()
{
    bool ret = initThread();
    if(ret) {
        mPacket->status = tr("自动验证开始");
        mResult->resEnter();
    }
    mItem->step = Test_End;  // 结束验证
}

void Ad_CoreThread::writeLog()
{
    Db_Tran db;
    sLogItem it;

    it.dev =mPacket->dev_type.split("_").first();
    it.op = user_land_name();
    it.user = mItem->user;
    it.sn = mPacket->sn;

    mItem->cnt.all += 1;
    if(mPacket->pass == Test_Success) {
        it.result = tr("通过");
        mItem->cnt.ok += 1;
    } else {
        mItem->cnt.err += 1;
        it.result = tr("失败：%1").arg(mPacket->status);
    }

    if(mItem->cnt.num > 0) {
        mItem->cnt.num -= 1;
        if(!mItem->cnt.num)
            mItem->user.clear();
    }

    Ad_Config::bulid()->writeCnt();
    if(it.sn.isEmpty()) return;
    DbLogs::bulid()->insertItem(it);
}

bool Ad_CoreThread::initSource()
{
    bool ret = false;
    mPacket->status = tr("已启动校准！连接标准源");
    mSource = mResult->initStandSource();
    if(mSource) {
        mPacket->status = tr("标准源上电中");
        ret = mSource->setVol(220, 6);

        mPacket->status = tr("等待设备启动完成！");
        ret = mModbus->delay(4);

        mPacket->status = tr("标准源设置电流！");
        if(ret) mSource->setCur(60, 5);
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

bool Ad_CoreThread::initLedSi()
{
    bool ret = false;
    mDt->devType = SI_PDU;
    mDt->ac = mItem->si_ac;
    mDt->specs = Transformer;
    mDt->lines = mItem->si_line;
    mPacket->dev_type = tr("SI/BM数码管");

    mPacket->status = tr("已启动校准！连接标准源");
    mSource = mResult->initStandSource();
    if(mSource) {
        mPacket->status = tr("标准源上电中");
        ret = mSource->setVol(220, 6);
        if(AC == mDt->ac) {
            mPacket->status = tr("标准源设置电流！");
            if(ret) mSource->setCur(60, 5);
        }
    } else return ret;

    Col_CoreThread *th = mResult->initThread();
    if(th) {
        for(int i=0; i<5; i++) {
            if(i) mPacket->status = tr("读取设备数据 %1").arg(i);
            ret = th->readPduData();
            if(ret) break; else if(!delay(3)) break;
        }
    }

    if(ret) {
        if(mDt->lines != mItem->si_line) {
            mPacket->status = tr("设备相数不对 %1").arg(mDt->lines);
            mPacket->pass = Test_Fail;
            ret = false;
        }
    }

    return ret;
}

void Ad_CoreThread::workDown()
{
    bool ret = true;
    mPacket->clear();
    if(mItem->si_led) {
        ret = initLedSi();
        if(ret) ret = mLedSi->startAdjust(mSource);
    } else {
        ret = readDevInfo(); if(!ret) return;
        ret = mAdjust->startAdjust(mSource);
    }

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
        mPacket->sn.clear();
        mPacket->data->size = 0;
        mJig->open();

        switch (mItem->step) {
        case Test_Start: workDown(); break;
        case Collect_Start: collectData(); break;
        case Test_vert: verifyResult(); break;
        }

        if(mSource) mSource->powerDown();
        mModbus->writeLogs();
        mJig->down();
        isRun = false;
    } else {
        qDebug() << "AdjustCoreThread run err" << isRun;
    }
}


