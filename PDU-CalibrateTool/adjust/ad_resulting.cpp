/*
 *
 *  Created on: 2020年10月1日
 *      Author: Lzy
 */
#include "ad_resulting.h"

Ad_Resulting::Ad_Resulting(QObject *parent) : QThread(parent)
{
    mPacket =sDataPacket::bulid();
    mModbus = Ad_Modbus::bulid(this);
    mItem = Ad_Config::bulid()->item;
    mSource = YC_StandSource::bulid(this);
    mData = mPacket->data;
}


Ad_Resulting *Ad_Resulting::bulid(QObject *parent)
{
    static Ad_Resulting* sington = nullptr;
    if(sington == nullptr)
        sington = new Ad_Resulting(parent);
    return sington;
}


bool Ad_Resulting::curErrRange(int exValue, int cur)
{
    bool ret = false;
    int min = exValue - mItem->curErr * 10;
    int max = exValue + mItem->curErr * 10;

    if((cur > min) && (cur < max )) {
        ret =  true;
    }

    return ret;
}


/**
 * @brief 电流统计，是否在误差范围
 * @return
 */
bool Ad_Resulting::curTgCheck(int exValue)
{
    int cur = 0;
    for(int i=0; i<mData->size; ++i) {
        cur += mData->cur[i];
    }

    return curErrRange(exValue, cur);
}


bool Ad_Resulting::curRangeByID(int i, int exValue)
{
    int cur = mData->cur[i] / mData->rate;
    bool ret = curErrRange(exValue, cur);
    if(ret) {
        mData->cured[i] = mData->cur[i];
        mData->status[i] = Test_Success;
    } else {
        mData->status[i] = Test_Fail;
        ret = false;
    }

    return ret;
}


bool Ad_Resulting::volErrRange()
{
    int min = mItem->vol - mItem->volErr;
    int max = mItem->vol + mItem->volErr;
    min *= mData->rate;
    max *= mData->rate;

    bool ret = true;
    for(int i=0; i<mData->size; ++i) {
        int vol = mData->vol[i];
        if((vol >= min) && (vol <= max)) {
            mData->status[i] = Test_Success;
        } else {
            ret = false;
            mData->status[i] = Test_Fail;
        }
    }

    return ret;
}


void Ad_Resulting::resTgData(sTgObjData *tg)
{
    tg->cur = 0;
    tg->pow = 0;

    for(int i=0; i<mData->size; ++i) {
        tg->cur += mData->cur[i];
        tg->pow += mData->pow[i];
    }    
}


bool Ad_Resulting::outputCurById(int k, int exValue)
{
    bool ret = false;
    for(int i=0; i<5; ++i) {
        mCollect->readPduData();
        ret = curRangeByID(k, exValue);
        if(ret)  break;
        delay(1);
    }

    return ret;
}

bool Ad_Resulting::outputSwCtrl(int exValue)
{
    bool ret = false;
    mCollect->readPduData();
    for(int k=0; k<mData->size; ++k) {
        mPacket->status = tr("校验数据 第%1输出位").arg(k+1);
        mCtrl->openOnlySwitch(k+1); delay(10);

        ret = outputCurById(k, exValue);
        if(!ret) break;
    }

    return ret;
}

bool Ad_Resulting::workResult(bool res)
{
    if(res) res = volErrRange();
    QString str = tr("校准失败!");
    if(res) {
        mPacket->pass = Test_Success;
        str = tr("校准成功!");
    } else {
        mPacket->pass = Test_Fail;
    }
    mPacket->status = str;

    mItem->step = Test_End;
    resTgData(mPacket->tg);
    delay(1);

    return res;
}

/**
 * @brief 总电流验证
 */
bool Ad_Resulting::sumCurCheck(int exValue)
{
    bool ret = true;

    for(int i=0; i<4; ++i) {
        mPacket->status = tr("校验数据: 第%1次").arg(i+1);
        mCollect->readPduData();

        ret = curTgCheck(exValue);
        if(ret) {
            break;
        } else {
            ret = delay(2);
            if(!ret) break;
        }
    }

    return ret;
}

bool Ad_Resulting::outputAllCheck(int exValue)
{
    bool res = true;
    for(int k=0; k<mData->size; ++k) {
        bool ret = curRangeByID(k, exValue);
        if(!ret) res = false;
    }
    return res;
}

bool Ad_Resulting::outputAllCurCheck(int exValue)
{
    bool ret = true;

    for(int i=0; i<4; ++i) {
        mPacket->status = tr("校验数据: 电流%1A 第%2次").arg(exValue/10).arg(i+1);
        mCollect->readPduData();
        ret = outputAllCheck(exValue);
        if(ret) {
            break;
        } else {
            if(!delay(2)) break;
        }
    }

    return ret;
}

bool Ad_Resulting::outputCurCheck(int exValue)
{
    bool ret = outputSwCtrl(exValue);
    if(ret) {
        mCtrl->openAllSwitch();
    }

    return ret;
}

Col_CoreThread *Ad_Resulting::initThread()
{
    sDevType *dt = mPacket->devType;
    switch (dt->devType) {
    case ZPDU:
        mCollect = Col_ZPduThread::bulid(this);
        mCtrl = Ctrl_ZpduThread::bulid(this);
        break;

    case MPDU:
        mCollect = Col_MPduThread::bulid(this);
        mCtrl = Ctrl_MpduThread::bulid(this);
        break;

    default:
        break;
    }

    return mCollect;
}

bool Ad_Resulting::workDown(int exValue)
{
    bool ret = false;
    sDevType *dt = mPacket->devType;
    switch (dt->specs) {
    case Transformer: ret = outputAllCurCheck(exValue); break; // 输出位  互感器校验
    case Mn: ret = outputCurCheck(exValue); break; // 输出位锰铜
    case 3: ret = sumCurCheck(exValue); break;
    }

    return ret;
}

bool Ad_Resulting::resEnter()
{
    bool ret = true;
    initThread();

    mItem->step = Test_vert;
    for(int i=0; i<3; ++i) {
        int exCur = (i*2 + 1) * 10;
        mSource->powerOn(exCur);
        mPacket->status = tr("验证电流：%1A").arg(exCur/10);
        ret = delay(10); if(!ret) return false;

        ret = workDown(exCur);
        if(!ret) break;
    }

    return workResult(ret);
}

