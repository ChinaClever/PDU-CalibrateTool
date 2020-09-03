#ifndef COL_CORETHREAD_H
#define COL_CORETHREAD_H

#include "ctrl_ipthread.h"

class Col_CoreThread : public QThread
{
    Q_OBJECT
public:
    explicit Col_CoreThread(QObject *parent = nullptr);

    virtual bool readPduData()=0;

protected:
    uchar *toInt(uchar *ptr, int line, uint *value);
    uchar *toChar(uchar *ptr, int line, uchar *value);
    uchar *toShort(uchar *ptr, int line, ushort *value);

    ushort getShort(uchar *ptr);
    uchar *getShort(uchar *ptr, int line, uchar *value);
    uchar *toOutputEle(uchar *ptr, int line, uint *value);
    uchar *toThreshold(uchar *ptr, int line, sThreshold &unit);

protected:
    sDataUnit *mData;
    Ad_Modbus *mModbus;
    sConfigItem *mItem;
};

#endif // COL_CORETHREAD_H
