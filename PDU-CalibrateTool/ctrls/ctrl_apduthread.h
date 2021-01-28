#ifndef CTRL_APDUTHREAD_H
#define CTRL_APDUTHREAD_H
#include "ctrl_mpduthread.h"

class Ctrl_ApduThread : public Ctrl_CoreThread
{
    Q_OBJECT
    explicit Ctrl_ApduThread(QObject *parent = nullptr);
public:
    static Ctrl_ApduThread *bulid(QObject *parent = nullptr);

protected:
    void funClearEle(uchar *buf);
    void funSwitch(uchar *, uchar *){}
};

#endif // CTRL_APDUTHREAD_H
