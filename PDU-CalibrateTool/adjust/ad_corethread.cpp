/*
 *
 *  Created on: 2020年10月1日
 *      Author: Lzy
 */
#include "ad_corethread.h"
#include "dbvalue.h"
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
        mItem->step = Tset_Collect;
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

bool Ad_CoreThread::initThread(int v)
{
    bool ret = false;
    if(mItem->si_led) {
        ret = initLedSi();
    } else {
        if(mSource){
            mPacket->status = tr("给标准源上电中！");
            ret = mSource->setVol(v, 4);
            ret = mAutoID->readDevType();
        }
        if(!ret) ret = readDevInfo();
    }

    return ret;
}

void Ad_CoreThread::collectData()
{
    bool ret = initThread(220);
    if(ret) mPacket->status = tr("正在读取设备数据");
    else {mItem->step = Test_End; return;}

    Col_CoreThread *th = mResult->initThread();
    while(mItem->step != Test_Over) {
        ret = th->readPduData(); delay(2);
        if(ret){
            mResult->resTgData();
            mPacket->pass = Test_Success;
            mPacket->status = tr("正在读取设备数据");
        } else {
            mPacket->pass = Test_Fail;
            mPacket->status = tr("读取设备数据错误！");
        }
    }
    mPacket->status = tr("读取设备数据停止！");
}

void Ad_CoreThread::verifyResult()
{
    bool ret = initThread(200);
    if(ret) {
        mPacket->status = tr("自动验证开始");
        mResult->resEnter();
    }
}

void Ad_CoreThread::writeLog()
{
    Db_Tran db;
    sLogItem it;
    sValueItem val;

    val.dev = it.dev = mPacket->dev_type.split("_").first();
    val.op = it.op = user_land_name();
    val.user = it.user = mItem->user;
    val.sn = it.sn = mPacket->sn;

    mItem->cnt.all += 1;
    if(mPacket->pass == Test_Success) {
        val.result = it.result = tr("通过");
        mItem->cnt.ok += 1;
        if(mItem->cnt.num > 0) {
            mItem->cnt.num -= 1;
            if(!mItem->cnt.num) mItem->user.clear();
        }
        QString valStr;
        mAutoID->readDevValue(mDt->lines , valStr);
        val.value = valStr;
        DbValue::bulid()->insertItem(val);
    } else {
        mItem->cnt.err += 1;
        it.result = tr("失败：%1").arg(mPacket->status);
    }

    mModbus->writeLogs();
    Ad_Config::bulid()->writeCnt();

    if(it.sn.isEmpty()) return;
    DbLogs::bulid()->insertItem(it);
}

bool Ad_CoreThread::initSource()
{
    int sec = 15; bool ret = false;
    mPacket->status = tr("已启动校准！连接标准源");
    mSource = mResult->initStandSource();
    if(mSource) mSource->initFunSlot();
    if(mSource) {
        if((mDt->devType>APDU)||(mDt->specs==Transformer)) sec = 0;
        mPacket->status = tr("标准源上电中"); ret = mSource->setVol(220, sec);
        mPacket->status = tr("标准源设置电流！"); ret = mSource->setCur(60, 5);
    } else {
        mItem->step = Test_End;
    }

    return ret;
}

bool Ad_CoreThread::readDevInfo()
{
    bool ret = initSource();   
    if(mSource && ret) {
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
        ret = mSource->setVol(220, 0);
        if(AC == mDt->ac) {
            mPacket->status = tr("标准源设置电流！");
            if(ret) ret = mSource->setCur(60, 4);
        } } else return ret;

    Col_CoreThread *th = mResult->initThread();
    if(th && ret) {
        for(int i=0; i<5; i++) {
            mPacket->status = tr("读取设备数据");
            if(i) mPacket->status += tr(" 第%1次").arg(i);
            ret = th->readPduData(); if(ret) break; else if(!delay(3)) break;
        }
    }

    if(ret) {
        if(mDt->lines == mItem->si_line) {
            mPacket->status = tr("等待设备稳定"); ret = delay(6);
        } else {
            mPacket->status = tr("设备相数不对 %1").arg(mDt->lines);
            mPacket->pass = Test_Fail; ret = false;
        }
    }

    return ret;
}

void Ad_CoreThread::resDev()
{
    mPacket->status = tr("正在重启设备！");
    if(mPacket->devType->devType < RPDU_Mc) {
        if(mPacket->devType->specs == Mn) {
            mSource->setCur(0, 1);
        }
    }
    mSource->setVol(0, 5);
}

void Ad_CoreThread::workDown()
{
    bool ret = true;
    if(mItem->si_led) {
        ret = initLedSi(); if(!ret) return;
        ret = mLedSi->startAdjust(mSource);
    } else {
        ret = readDevInfo(); if(!ret) return;
        ret = mAdjust->startAdjust(mSource);
    }

    if(mItem->step == Test_vert) {
        resDev();
        mPacket->status = tr("开始自动验证");
        ret = mResult->resEnter();
    } else if(mItem->step == Test_End) {
        Col_CoreThread *th = mResult->initThread();
        th->readPduData();
    }
}

void Ad_CoreThread::run()
{
    if(!isRun) {
        isRun = true;
        mPacket->pass = 0;
        mPacket->sn.clear();
        mPacket->data->size = 0;
        mPacket->clear();
        mJig->open();

        switch (mItem->step) {
        case Test_Start: workDown(); break;
        case Tset_Collect: collectData(); break;
        case Test_vert: verifyResult(); break;
        }

        isRun = false; mItem->step = Test_End;
        if(mSource) mSource->powerDown();
        mJig->down(); writeLog();
    } else {
        qDebug() << "AdjustCoreThread run err" << isRun;
    }
}


