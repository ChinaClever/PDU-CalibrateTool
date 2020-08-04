#ifndef COL_SITHREAD_H
#define COL_SITHREAD_H

#include "col_mpduthread.h"

class Col_SiThread : public Col_CoreThread
{
    Q_OBJECT
    explicit Col_SiThread(QObject *parent = nullptr);
public:
    static Col_SiThread *bulid(QObject *parent = nullptr);
    bool readPduData();

protected:
    int recvLine(int len);
    void initRtuItem(sRtuItem &it);
    bool recvPacket(uchar *buf, int len);
    int recvAcData(uchar *ptr, int line, sDataUnit *msg);
    int recvDcData(uchar *ptr, int line, sDataUnit *msg);
};

#endif // COL_SITHREAD_H
