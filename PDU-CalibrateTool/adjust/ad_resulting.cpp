/*
 *
 *  Created on: 2020年10月1日
 *      Author: Lzy
 */
#include "ad_resulting.h"
#define AD_CUR_RATE 100

Ad_Resulting::Ad_Resulting(QObject *parent) : QThread(parent)
{
    mPacket =sDataPacket::bulid();
    mModbus = Ad_Modbus::bulid(this);
    mItem = Ad_Config::bulid()->item;
    mData = mPacket->data;
    mSource = nullptr;
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

    if((cur >= min) && (cur <= max )) {
        ret =  true;
    }
    resTgData();

    return ret;
}

bool Ad_Resulting::powErrRange(int exValue, int cur)
{
    bool ret = false;
    int min = exValue - mItem->powErr * 100;
    int max = exValue + mItem->powErr * 100;

    if((cur >= min) && (cur <= max )) {
        ret =  true;
    }
    resTgData();

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

    if(mData->rate < 10) cur *= 10;
    return curErrRange(exValue, cur);
}


bool Ad_Resulting::powRangeByID(int i, int exValue)
{
    int pow = mData->pow[i];
    if(mData->rate < 10) pow *= 10;

    exValue = mItem->vol * exValue/AD_CUR_RATE;
    mPacket->status = tr("校验数据: 期望功率%1Kw 第%2位 功率").arg(exValue/1000.0).arg(i+1);
    bool ret = powErrRange(exValue, pow);
    if(ret) {
        mData->powed[i] = mData->pow[i];
        mData->status[i] = Test_Success;
        mPacket->status += tr("正常");
    } else {
        mPacket->status += tr("错误");
        mData->status[i] = Test_Fail;
        ret = false;
    }

    return ret;
}


bool Ad_Resulting::curRangeByID(int i, int exValue)
{
    int cur = mData->cur[i];
    if(mData->rate < 10) cur *= 10;

    mPacket->status = tr("校验数据: 期望电流%1A 第%2位 电流").arg(exValue/AD_CUR_RATE).arg(i+1);
    bool ret = curErrRange(exValue, cur);
    if(ret) {
        mPacket->status += tr("正常");
        mData->cured[i] = mData->cur[i];
        ret = powRangeByID(exValue, cur);
    } else {
        mPacket->status += tr("错误");
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
            mPacket->status = tr("电压 %1 错误").arg(i+1);
            mModbus->appendLogItem(ret);
        }
    }

    return ret;
}


void Ad_Resulting::resTgData(sTgObjData *tg)
{
    tg->cur = tg->pow = 0;
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
        mModbus->appendLogItem(ret);
        if(!ret) delay(2); else break;
    }

    return ret;
}

bool Ad_Resulting::outputSwCtrl(int exValue)
{
    bool ret = false;
    for(int k=0; k<mData->size; ++k) {
        mPacket->status = tr("校验数据 期望电流%1A 第%2输出位 ").arg(exValue/AD_CUR_RATE).arg(k+1);
        mCtrl->openOnlySwitch(k); if(!delay(10)) break;
        ret = outputCurById(k, exValue);
        if(ret) {
            ret = delay(3); if(!ret) break;
        } else {
            //mPacket->status = tr("错误");
            break;
        }
    }

    return ret;
}

bool Ad_Resulting::workResult(bool res)
{
    if(res) {
        mPacket->pass = Test_Success;
        mPacket->status = tr("校准成功!");
    } else {
        mPacket->pass = Test_Fail;
    }
    mModbus->appendLogItem(res);

    return res;
}

/**
 * @brief 总电流验证
 */
bool Ad_Resulting::sumCurCheck(int exValue)
{
    bool ret = true;

    for(int i=0; i<4; ++i) {
        mPacket->status = tr("校验数据: 期望电流%1A 第%2次").arg(exValue/AD_CUR_RATE).arg(i+1);
        mCollect->readPduData();
        ret = curTgCheck(exValue);
        if(ret) break; else if(!delay(2)) break;
    }

    return ret;
}

bool Ad_Resulting::eachCurCheck(int exValue)
{
    bool res = true;
    for(int k=0; k<mData->size; ++k) {
        bool ret = curRangeByID(k, exValue);        
        if(!ret) res = false;
    }
    return res;
}


bool Ad_Resulting::eachCurEnter(int exValue)
{
    bool ret = true;
    for(int i=0; i<4; ++i) {
        mPacket->status = tr("校验数据: 期望电流%1A 功率%2Kw 第%3次")
                .arg(exValue/AD_CUR_RATE).arg(mItem->vol*exValue/AD_CUR_RATE/1000.0).arg(i+1);

        mCollect->readPduData();
        ret = eachCurCheck(exValue);
        mModbus->appendLogItem(ret);
        if(ret) break; else if(!delay(2)) break;
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

YC_StandSource *Ad_Resulting::initStandSource()
{
    static YC_StandSource *source = YC_Ac92b::bulid(this);
    bool ret = source->handShake();
    if(!ret) {
        source = YC_Dc107::bulid(this);
        ret = source->handShake();
        if(!ret) {
            source = YC_Ac92b::bulid(this);
            ret = source->handShake();
            if(!ret) {
                source = nullptr;
                mPacket->status = tr("标准源通讯失败!");
                workResult(ret);
            }
        }
    }

    return source;
}

Col_CoreThread *Ad_Resulting::initThread()
{
    sDevType *dt = mPacket->devType;
    switch (dt->devType) {
    case ZPDU:
        mCollect = Col_ZpduThread::bulid(this);
        mCtrl = Ctrl_ZpduThread::bulid(this);
        break;

    case MPDU:
        mCollect = Col_MpduThread::bulid(this);
        mCtrl = Ctrl_MpduThread::bulid(this);
        break;

    case RPDU:
        mCollect = Col_RpduThread::bulid(this);
        mCtrl = Ctrl_RpduThread::bulid(this);
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
    case Sum: ret = sumCurCheck(exValue); break;
    case Mn: ret = outputCurCheck(exValue); break; // 输出位锰铜
    case Transformer: ret = eachCurEnter(exValue); break; // 输出位  互感器校验
    }

    return ret;
}

bool Ad_Resulting::noLoadCurCheck()
{
    bool res = true;
    for(int k=0; k<mData->size; ++k) {
        if(mData->cur[k] || mData->pow[k]) {
            res = false;
            mData->status[k] = Test_Fail;            
            mPacket->status = tr("校验数据: 空载电流A 第%1位 ").arg(k+1);
            if(mData->cur[k]) mPacket->status += tr("电流错误");
            if(mData->pow[k]) mPacket->status += tr("功率错误");
        } else {
            mData->status[k] = Test_Success;
        }
    }
    return res;
}


bool Ad_Resulting::noLoadCurFun()
{
    bool ret = true;
    for(int i=0; i<4; ++i) {
        mPacket->status = tr("校验数据: 空载电流 第%2次检查").arg(i+1);
        mCollect->readPduData();
        ret = noLoadCurCheck();
        mModbus->appendLogItem(ret);
        if(ret) break; else if(!delay(2)) break;
    }

    return ret;
}



bool Ad_Resulting::noLoadEnter()
{
    mCollect->readPduData();
    bool ret = volErrRange();
    if(ret) {
        mPacket->status = tr("验证电流：空载电流检查");
        mCtrl->openAllSwitch();
        ret = mSource->setCur(0);
        if(ret) ret = noLoadCurFun();
    }

    return ret;
}

bool Ad_Resulting::resEnter()
{
    mSource = initStandSource();    
    if(mSource) mSource->setVol(200); else return false;

    initThread(); delay(5);
    mItem->step = Test_vert;
    bool ret = noLoadEnter();
    if(ret) {
        for(int i=4; i<5; ++i) {
            int exCur = (i*2) * AD_CUR_RATE;
            mPacket->status = tr("验证电流：期望电流%1A").arg(exCur/AD_CUR_RATE);
            ret = mSource->setCur(exCur/10);
            if(ret) ret = workDown(exCur);
            if(!ret) break;
        }
    }

    return workResult(ret);
}

