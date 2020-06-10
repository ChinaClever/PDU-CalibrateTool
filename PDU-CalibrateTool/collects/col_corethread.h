#ifndef COL_CORETHREAD_H
#define COL_CORETHREAD_H

#include "ctrl_mpduthread.h"

class Col_CoreThread : public QThread
{
    Q_OBJECT
public:
    explicit Col_CoreThread(QObject *parent = nullptr);

    virtual bool readPduData()=0;

protected:
    sDataUnit *mData;
    Ad_Modbus *mModbus;
    sConfigItem *mItem;
};

#endif // COL_CORETHREAD_H
