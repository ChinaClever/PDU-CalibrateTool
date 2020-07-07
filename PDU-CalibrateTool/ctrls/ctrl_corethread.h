#ifndef CTRL_CORETHREAD_H
#define CTRL_CORETHREAD_H

#include "ad_adjusting.h"

class Ctrl_CoreThread : public QThread
{
    Q_OBJECT
public:
    explicit Ctrl_CoreThread(QObject *parent = nullptr);

    virtual void openAllSwitch();
    virtual void closeAllSwitch();
    virtual void openOutputSwitch(int id);
    virtual void closeOutputSwitch(int id);
    virtual void openOnlySwSlot(int id);
    virtual void closeOtherSwitch(int id);
    virtual void openOnlySwitch(int id);

    virtual void clearAllEle();
    virtual void setClearEle(int id);

protected:
    void setBitControl(int id, uchar *buf);
    bool delay(int s) {return  mModbus->delay(s);}

    virtual void funClearEle(uchar *buf)=0;
    virtual void funSwitch(uchar *on, uchar *off, int f)=0;

protected slots:
    void closeOtherSwitchSlot();

protected:
    int mIdClosed;
    sDataUnit *mData;
    sConfigItem *mItem;
    Ad_Modbus *mModbus;
};

#endif // AD_CORETHREAD_H
