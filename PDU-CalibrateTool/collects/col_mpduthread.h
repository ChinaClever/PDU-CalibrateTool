#ifndef COL_MPDUTHREAD_H
#define COL_MPDUTHREAD_H

#include "col_zpduthread.h"

class Col_MpduThread : public Col_CoreThread
{
    Q_OBJECT
    explicit Col_MpduThread(QObject *parent = nullptr);
public:
    static Col_MpduThread *bulid(QObject *parent = nullptr);

    bool readPduData();
protected:
    bool recvMpduVolCur(uchar *recv, int);
    bool getMpduVolCur();

    bool recvMpduEle(uchar *recv, int len);
    int getMpduEle();
};

#endif // COL_MPDUTHREAD_H
