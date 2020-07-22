#ifndef COL_RPDUTHREAD_H
#define COL_RPDUTHREAD_H

#include "col_rpdumcthread.h"

class Col_RpduThread : public Col_CoreThread
{
    Q_OBJECT
    explicit Col_RpduThread(QObject *parent = nullptr);
public:
    static Col_RpduThread *bulid(QObject *parent = nullptr);
    bool readPduData();

protected:
    bool recvVolCur(uchar *recv, int);
    bool getVolCur();

    bool recvEle(uchar *recv, int);
    int getEle();
};

#endif // COL_RPDUTHREAD_H
