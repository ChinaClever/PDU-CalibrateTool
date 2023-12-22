#ifndef AD_RESULTING_H
#define AD_RESULTING_H

#include "sn_managethread.h"
#include "yc_dc107.h"
#include "udpsendsocket.h"

class Ad_Resulting : public QThread
{
    Q_OBJECT
    explicit Ad_Resulting(QObject *parent = nullptr);
public:
    static Ad_Resulting *bulid(QObject *parent = nullptr);

    bool resEnter();
    Col_CoreThread *initThread();
    YC_StandSource *initStandSource();
    void resTgData() {resTgData(mPacket->tg);}
    bool curByIDOtherZero(int k, int exValue,QVector<int> vec);

protected:
    bool powErrRange(int exValue, int cur);
    bool curErrRange(int exValue, int cur);

    bool curTgCheck(int exValue);
    bool curRangeByID(int i, int exValue, int cnt);
    bool powRangeByID(int i, int exValue, int cnt);
    bool volErrRange();
    bool volErrRangeByID(int i);

    int outputIdCheck(int k);
    void resTgData(sTgObjData *tg);
    bool outputSwCtrl(int exValue);

    bool workResult(bool res);

    bool outputCurById(int k, int exValue);
    bool sumCurCheck(int exValue);
    bool workDown(int exValue);
    bool delay(int s) {return mModbus->delay(s);}
    bool outputCurCheck(int exValue);

    bool eachCurCheck(int exValue, int cnt);
    bool eachCurEnter(int exValue);

    bool noLoadEnter();
    bool noLoadCurFun();
    bool noLoadCurCheck(int cnt);

    bool powerOn();

private:
    sDataUnit *mData;
    Ad_Modbus *mModbus;
    sConfigItem *mItem;
    sDataPacket *mPacket;
    YC_StandSource *mSource;

    Col_CoreThread *mCollect;
    Ctrl_CoreThread *mCtrl;
    sProgress *mPro;
    sDataPacket *log;
};

#endif // AD_RESULTING_H
