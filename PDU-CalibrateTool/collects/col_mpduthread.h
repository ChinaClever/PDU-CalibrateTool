#ifndef COL_MPDUTHREAD_H
#define COL_MPDUTHREAD_H

#include "col_zpduthread.h"

class Col_MPduThread : public Col_CoreThread
{
    Q_OBJECT
    explicit Col_MPduThread(QObject *parent = nullptr);
public:
    static Col_MPduThread *bulid(QObject *parent = nullptr);

    bool readPduData();
protected:
    bool recvMpduVolCur(uchar *recv, int);
    bool getMpduVolCur();

    bool recvMpduEle(uchar *recv, int);
    int getMpduEle();
};

#endif // COL_MPDUTHREAD_H
