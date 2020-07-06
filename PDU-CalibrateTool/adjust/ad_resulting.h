#ifndef AD_RESULTING_H
#define AD_RESULTING_H

#include "sn_managethread.h"
#include "yc_standsource.h"

class Ad_Resulting : public QThread
{
    Q_OBJECT
    explicit Ad_Resulting(QObject *parent = nullptr);
public:
    static Ad_Resulting *bulid(QObject *parent = nullptr);

    bool resEnter();
    Col_CoreThread *initThread();
    void resTgData() {resTgData(mPacket->tg);}

protected:
    bool curErrRange(int exValue, int cur);
    bool curTgCheck(int exValue);
    bool curRangeByID(int i, int exValue);
    bool volErrRange();

    void resTgData(sTgObjData *tg);
    bool outputSwCtrl(int exValue);

    bool workResult(bool res);

    bool outputCurById(int k, int exValue);
    bool sumCurCheck(int exValue);
    bool workDown(int exValue);
    bool delay(int s) {return mModbus->delay(s);}
    bool outputCurCheck(int exValue);

    bool outputAllCheck(int exValue);
    bool outputAllCurCheck(int exValue);

private:
    sDataUnit *mData;
    Ad_Modbus *mModbus;
    sConfigItem *mItem;
    sDataPacket *mPacket;
    YC_StandSource *mSource;

    Col_CoreThread *mCollect;
    Ctrl_CoreThread *mCtrl;
};

#endif // AD_RESULTING_H
