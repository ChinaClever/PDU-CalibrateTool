#ifndef ADJUSTZPDUTHREAD_H
#define ADJUSTZPDUTHREAD_H

#include "adjustmpduthread.h"

class AdjustZpduThread : public AdjustCoreThread
{
    Q_OBJECT
    explicit AdjustZpduThread(QObject *parent = nullptr);
public:
    static AdjustZpduThread *bulid(QObject *parent = nullptr);

signals:

protected:
    bool startActivationCmd();
    bool resActivationCmd();
    int openAllSwitch();

    bool recvZpduVolCur(uchar *recv, int len);
    bool getZpduVolCur();
    bool readPduData();
    void clearPduEle();
};

#endif // ADJUSTZPDUTHREAD_H
