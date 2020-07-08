#ifndef COL_SITHREAD_H
#define COL_SITHREAD_H

#include "col_mpduthread.h"

class Col_SiThread : public Col_CoreThread
{
    Q_OBJECT
public:
    explicit Col_SiThread(QObject *parent = nullptr);
    static Col_SiThread *bulid(QObject *parent = nullptr);
    bool readPduData();

protected:
    virtual void initRtuItem(sRtuItem &it);
    virtual bool recvPacket(uchar *buf, int len);
    int recvData(uchar *ptr, int line, sDataUnit *msg);

    virtual int recvLine(int len);
    uchar *toInt(uchar *ptr, int line, uint *value);
    uchar *toChar(uchar *ptr, int line, uchar *value);
    uchar *toShort(uchar *ptr, int line, ushort *value);

    struct sThreshold { uchar min[3], max[3];};
    uchar *toThreshold(uchar *ptr, int line, sThreshold &unit);
};

#endif // COL_SITHREAD_H
