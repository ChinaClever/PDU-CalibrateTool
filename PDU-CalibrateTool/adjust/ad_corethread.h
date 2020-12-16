#ifndef AD_CORETHREAD_H
#define AD_CORETHREAD_H

#include "ad_ledsi.h"
#include "ad_resulting.h"

class Ad_CoreThread : public QThread
{
    Q_OBJECT
    explicit Ad_CoreThread(QObject *parent = nullptr);
public:
    static Ad_CoreThread *bulid(QObject *parent = nullptr);
    ~Ad_CoreThread();

    void startAdjust();
    void startCollect();
    void startResult();

protected:
    void run();
    bool initLedSi();
    bool initThread();
    bool initSource();

    void writeLog();
    void workDown();
    void collectData();
    void verifyResult();
    bool readDevInfo();
    bool delay(int s) {return mModbus->delay(s);}

private:
    bool isRun;
    sDevType *mDt;
    sDataPacket *mPacket;
    sConfigItem *mItem;
    Ad_Modbus *mModbus;
    Ad_AutoID *mAutoID;
    Ad_Adjusting *mAdjust;
    Ad_Resulting *mResult;
    YC_StandSource *mSource;
    SN_ManageThread *mSn;
    Ad_LedSi *mLedSi;
};

#endif // AD_CORETHREAD_H
