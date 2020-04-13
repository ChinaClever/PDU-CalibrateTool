#ifndef ADJUSTMPDUTHREAD_H
#define ADJUSTMPDUTHREAD_H

#include "adjustcorethread.h"

class AdjustMpduThread : public AdjustCoreThread
{
    Q_OBJECT
    explicit AdjustMpduThread(QObject *parent = nullptr);
public:
    static AdjustMpduThread *bulid(QObject *parent = nullptr);

signals:

protected:
    bool resActivationCmd();
    bool startActivationCmd();
    int openSwitch();

    bool recvMpduVolCur(uchar *recv, int);
    bool getMpduVolCur();

    bool recvMpduEle(uchar *recv, int);
    void getMpduEle();
    bool readPduData();
};

#endif // ADJUSTMPDUTHREAD_H
