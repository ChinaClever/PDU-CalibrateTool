#ifndef COL_IPTHREAD_H
#define COL_IPTHREAD_H

#include "col_sithread.h"

class Col_IpThread : public Col_CoreThread
{
    Q_OBJECT
    explicit Col_IpThread(QObject *parent = nullptr);
public:
    static Col_IpThread *bulid(QObject *parent = nullptr);
    bool readPduData();

protected:
    int recvLine(int len);
    void initRtuItem(sRtuItem &it);
    bool recvPacket(uchar *buf, int len);

    int recvDataV1(uchar *ptr, sDataUnit *msg);
    int recvDataV3(uchar *ptr, sDataUnit *msg);
    uchar *getSwitch(uchar *ptr, int line, uchar *value);
};

#endif // COL_IPTHREAD_H
