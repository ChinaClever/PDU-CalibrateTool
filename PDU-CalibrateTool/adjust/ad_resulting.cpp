/*
 *
 *  Created on: 2020年10月1日
 *      Author: Lzy
 */
#include "ad_resulting.h"
#define AD_CUR_RATE 100
#include "datapacket.h"


Ad_Resulting::Ad_Resulting(QObject *parent) : QThread(parent)
{
    mPacket =sDataPacket::bulid();
    mSource =YC_Ac92b::bulid(this);;
    mModbus = Ad_Modbus::bulid(this);
    mPro = sDataPacket::bulid()->getPro();
    mItem = Ad_Config::bulid()->item;
    log = sDataPacket::bulid();
    mData = mPacket->data;
    mCollect= nullptr;
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

bool Ad_Resulting::powErrRange(int exValue, int pow)
{
    bool ret = false;
    int err = exValue * mItem->powErr/1000.0;
    int min = exValue - err;
    int max = exValue + err;

    if((pow >= min) && (pow <= max )) {
        ret =  true;
    } else {
        qDebug() << "pow Err Range" << pow << exValue << err;
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

    if(mData->rate < 10) cur *= 10;
    return curErrRange(exValue, cur);
}

bool Ad_Resulting::powRangeByID(int i, int exValue, int cnt)
{
    sDevType *dt = mPacket->devType;
    exValue = mItem->vol * exValue/AD_CUR_RATE;
    if(AC == dt->ac) exValue *= 0.5;
    mPacket->status = tr("期望功率%1kW 第%2位 功率").arg(exValue/1000.0).arg(i+1);

    int pow = mData->pow[i] / mData->rate;
    bool ret = powErrRange(exValue, pow);
    mData->powed[i] = mData->pow[i];
    if(ret) {
        mData->status[i] = Test_Success;
        mPacket->status += tr("正常");
    } else {
        if(cnt > 3) {
            mPacket->status += tr("错误");
            mData->status[i] = Test_Fail;
        }
        ret = false;
    }
    return ret;
}

bool Ad_Resulting::curRangeByID(int i, int exValue, int cnt)
{
    int cur = mData->cur[i];
    if(mData->rate < 10) cur *= 10;
    if(mPacket->devType->lines == 2 &&i == 0) exValue *= 2;
    mPacket->status = tr("期望电流%1A 第%2位 电流").arg(exValue/AD_CUR_RATE).arg(i+1);
    bool ret = curErrRange(exValue, cur);
    mData->cured[i] = mData->cur[i];
    if(ret) {
        if(mItem->si_led) return ret;
        ret = powRangeByID(i, exValue, cnt);
    } else {
        if(cnt > 2) {
            mPacket->status += tr("错误");
            mData->status[i] = Test_Fail;
        }
        ret = false;
    }
    return ret;
}

bool Ad_Resulting::curByIDOtherZero(int k, int exValue,QVector<int> vec)
{
    bool ret = false;
    for(int i=0; i<=5; ++i) {
        mCollect->readPduData();
        ret = curRangeByID(k, exValue, i);
        for(int j = 0 ; j < mData->size ; j++){
            bool flag = false;
            for(int m = 0 ; m < vec.size() ; m++){
                if(vec[m] == j ) {flag = true;break;}
            }
            if(!flag){
                int cur = mData->cur[j];
                if(cur){
                   mPacket->status += tr("校验:第%1位").arg(j+1);
                   mPacket->status += tr("电流有底数");
                   mData->status[j] = Test_Fail;
                   ret = false;
                   mData->cured[j] = mData->cur[j];
                }
            }
        }
        if(!ret) delay(2); else break;
    }
    mModbus->appendLogItem(ret);
    return ret;
}


bool Ad_Resulting::volErrRangeByID(int i)
{
    int min = mItem->vol - mItem->volErr;
    int max = mItem->vol + mItem->volErr;
    min *= mData->rate;
    max *= mData->rate;

    bool ret = true;
    int vol = mData->vol[i];
    mPacket->status = tr("期望电压200V，实际电压%1V 第%2位 电压").arg(vol/mData->rate).arg(i+1);
    if((vol >= min) && (vol <= max)) {
        mData->status[i] = Test_Success;
    } else {
        ret = false;
    }
    return ret;
}


bool Ad_Resulting::volErrRange()
{
    int k = 0;
    bool ret = mCollect->readPduData();
    for(int i=0; i<mData->size; ++i) {
        i = outputIdCheck(i);
        if(i < mData->size) {
            ret = volErrRangeByID(i);
            if(!ret) {
                if(k++ <= 6){
                    i = -1; delay(5);
                    mCollect->readPduData();
                } else {
                    ret = false;
                    mData->status[i] = Test_Fail;
                    mPacket->status = tr("检测到电压 %1 错误").arg(i+1);
                    mModbus->appendLogItem(ret);
                    break;
                }
            }
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
    for(int i=0; i<=5; ++i) {
        mCollect->readPduData();
        ret = curRangeByID(k, exValue, i);
        if(!ret) delay(2); else break;
    }
    mModbus->appendLogItem(ret);

    return ret;
}

bool Ad_Resulting::outputSwCtrl(int exValue)
{
    bool ret = false;
    for(int k=0; k<mData->size; ++k) {
        k = outputIdCheck(k);

        if(k < mData->size) {
            QVector<int> vec;
            if(mPacket->devType->devType == MPDU && mData->reserve == 6) {
                vec.push_back(2);
                vec.push_back(6);
            }
            vec.push_back(k);

            mPacket->status = tr("校验数据 期望电流%1A 第%2输出位 ").arg(exValue/AD_CUR_RATE).arg(k+1);
            mCtrl->openOutputSwitch(k); if(!delay(1)) break;
            mCtrl->openOnlySwitch(k); if(!delay(1)) break;
            mCtrl->closeOtherSwitch(k); if(!delay(1)) break;
            //ret = curByIDOtherZero(k, exValue ,vec);
            ret = outputCurById(k, exValue);
            if(ret) {
                ret = delay(1); if(!ret) break;
            } else {
                //mPacket->status = tr("错误");
                break;
            }
            QVector<int>().swap(vec);
        }
    }
    return ret;
}

bool Ad_Resulting::workResult(bool res)
{
    if(res && (mPacket->pass != Test_Fail)) {
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

    for(int i=0; i<5; ++i) {
        mPacket->status = tr("校验数据: 期望电流%1A 第%2次").arg(exValue/AD_CUR_RATE).arg(i+1);
        mCollect->readPduData();
        ret = curTgCheck(exValue);
        if(ret) break; else if(!delay(2)) break;
    }
    return ret;
}

int  Ad_Resulting::outputIdCheck(int k)
{
    if(mPacket->devType->series == 3) {
        if(k) k = mData->size - 1;
        if(mData->size == 4 && k > 0) k = mData->size;
    }

    if(mPacket->devType->devType == MPDU && mData->reserve == 6) {
        if(k==2||k==6) k++;//华为6位MPDU执行板跳过第3，7位
    }

    return k;
}

bool Ad_Resulting::eachCurCheck(int exValue, int cnt)
{
    bool res = true;
    for(int k=0; k<mData->size; ++k) {
        k = outputIdCheck(k);
        if(k < mData->size) {
            bool ret = curRangeByID(k, exValue, cnt);
            if(!ret) {res = false;return res;}
        }
    }
    return res;
}

bool Ad_Resulting::eachCurEnter(int exValue)
{
    bool ret = true;
    sDevType *dt = mPacket->devType;
    for(int i=0; i<5; ++i) {
        double value = mItem->vol*exValue/AD_CUR_RATE/1000.0;
        if(AC == dt->ac) value = value*0.5;
        mPacket->status = tr("校验数据: 期望电流%1A 功率%2kW").arg(exValue/AD_CUR_RATE).arg(value);
        if(i) mPacket->status += tr(" 第%1次").arg(i+1);
        else delay(5);

        mCollect->readPduData();
        ret = eachCurCheck(exValue, i);
        if(ret) break; else if(!delay(3)) break;
    }
    mModbus->appendLogItem(ret);
    return ret;
}

bool Ad_Resulting::outputCurCheck(int exValue)
{
    bool ret = outputSwCtrl(exValue);
    mCtrl->openAllSwitch();

    return ret;
}

YC_StandSource *Ad_Resulting::initStandSource()
{
    if(!mSource) mSource = YC_Ac92b::bulid(this);
    bool ret = mSource->handShake();
    if(!ret) {
        mSource = YC_Dc107::bulid(this);
        ret = mSource->handShake();
        if(!ret) {
            mSource = YC_Ac92b::bulid(this);
            ret = mSource->handShake();
            if(!ret) {
                mSource = nullptr;
                mPacket->status = tr("标准源通讯失败!");
                workResult(ret);
            }
        }
    }
    return mSource;
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

    case SI_PDU:
    case BM_PDU:
        mCollect = Col_SiThread::bulid(this);
        mCtrl = Ctrl_SiThread::bulid(this);
        break;

    case IP_PDU:
        mCollect = Col_IpThread::bulid(this);
        mCtrl = Ctrl_IpThread::bulid(this);
        break;

    case APDU:
        mCollect = Col_ApduThread::bulid(this);
        mCtrl = Ctrl_ApduThread::bulid(this);
        break;

    default:
        mCollect = nullptr;
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

bool Ad_Resulting::noLoadCurCheck(int cnt)
{
    bool res = true;
    for(int k=0; k<mData->size; ++k) {
        k = outputIdCheck(k);
        if(k < mData->size) {
            mData->powed[k] = mData->pow[k];
            mData->cured[k] = mData->cur[k];
            if(mData->cur[k] || mData->pow[k]) {
                res = false;
                if(cnt > 2) {
                    mData->status[k] = Test_Fail;
                    mPacket->status = tr("空载校验:第%1位").arg(k+1);
                    if(mData->cur[k]) mPacket->status += tr("电流有底数");
                    if(mData->pow[k]) mPacket->status += tr(" 功率有底数");
                }
            } else {
                mData->status[k] = Test_Success;
            }
        }
    }
    return res;
}

bool Ad_Resulting::noLoadCurFun()
{
    bool ret = true;
    for(int i=0; i<5; ++i) {
        if(i)mPacket->status = tr("空载校验: 第%1次检查").arg(i+1);
        else delay(3);

        mCollect->readPduData();
        ret = noLoadCurCheck(i);
        if(ret) break; else if(!delay(3)) break;
    }
    mModbus->appendLogItem(ret);
    return ret;
}

bool Ad_Resulting::noLoadEnter()
{
    mPacket->status = tr("空载验证：设置空载电流");
    bool ret = mSource->setCur(0, 5);
    if(ret) ret = noLoadCurFun();
    if(ret) {
        mCtrl->closeAllSwitch();
        ret = volErrRange();
        mCtrl->openAllSwitch();
    }
    return ret;
}

bool Ad_Resulting::powerOn()
{
    bool ret = false;
    mItem->step = Test_vert;
    if(!mSource) initStandSource();
    if(mSource) initThread(); else return ret;
    ret = mSource->setVol(200, 4);
    if(mPacket->devType->specs != Transformer) {
        for(int i=0; i<3; ++i) {
            ret = mCollect->readPduData();
            if(ret) break; else if(!delay(3+i)) break;
        }
        if(!ret) mPacket->status = tr("通讯协议不正确");
    }
    return ret;
}


bool Ad_Resulting::resEnter()
{
    bool ret = powerOn();
    if(ret) {
        for(int i=0; i<1; ++i) {
            int exCur = 0;
            switch (i) {
            case 0: exCur = 4; break;
            case 1: exCur = 8; break;
            }

            sleep(5);///////MPDU Pro新执行板
            mPacket->status = tr("验证电流：期望电流%1A").arg(exCur);
            ret = mSource->setCur(exCur*10, 3); mCtrl->factorySet();
            if(ret) ret = workDown(exCur*AD_CUR_RATE);
            if(!ret) break;
        }
        if(ret) {ret = noLoadEnter();}
    }

    sDevType *dt = mPacket->devType;
    for(int i=0; i<3; ++i) {
        if(ZPDU == dt->devType){ mCtrl->setDelayToZero();delay(5);}
    }
    mCtrl->openAllSwitch();
    delay(5);
    mCtrl->openAllSwitch();
    delay(5);
    mCtrl->clearAllEle();
    return workResult(ret);
}

