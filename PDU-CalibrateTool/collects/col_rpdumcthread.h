#ifndef COL_MCRPDUTHREAD_H
#define COL_MCRPDUTHREAD_H

#include "col_ipthread.h"

class Col_RpduMcThread : public Col_CoreThread
{
    Q_OBJECT
    explicit Col_RpduMcThread(QObject *parent = nullptr);
public:
    static Col_RpduMcThread *bulid(QObject *parent = nullptr);
    bool readPduData();

protected:
    void initRtuItem(sRtuItem &it);
    bool recvPacket(uchar *ptr, int len);

};

#endif // COL_MCRPDUTHREAD_H
