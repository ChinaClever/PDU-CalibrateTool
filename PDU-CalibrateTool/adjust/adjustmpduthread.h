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

    void funSwitch(uchar *on, uchar *off);
    bool recvMpduVolCur(uchar *recv, int);
    bool getMpduVolCur();

    bool recvMpduEle(uchar *recv, int);
    int getMpduEle();
    bool readPduData();

    void funClearEle(uchar *buf);
};

#endif // ADJUSTMPDUTHREAD_H
