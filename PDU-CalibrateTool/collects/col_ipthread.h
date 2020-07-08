#ifndef COL_IPTHREAD_H
#define COL_IPTHREAD_H

#include "col_sithread.h"

class Col_IpThread : public Col_SiThread
{
    Q_OBJECT
    explicit Col_IpThread(QObject *parent = nullptr);
public:
    static Col_IpThread *bulid(QObject *parent = nullptr);

protected:
    int recvLine(int len);
    void initRtuItem(sRtuItem &it);
    bool recvPacket(uchar *buf, int len);

    int recvData(uchar *ptr, sDataUnit *msg);
    int recvDataV3(uchar *ptr, sDataUnit *msg);
    ushort getShort(uchar *ptr);
    uchar *getShort(uchar *ptr, int line, uchar *value);
    uchar *getSwitch(uchar *ptr, int line, uchar *value);
};

#endif // COL_IPTHREAD_H
