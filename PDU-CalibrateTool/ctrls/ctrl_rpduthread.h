#ifndef CTRL_RPDUTHREAD_H
#define CTRL_RPDUTHREAD_H

#include "ctrl_mpduthread.h"

class Ctrl_RpduThread : public Ctrl_CoreThread
{
    Q_OBJECT
    explicit Ctrl_RpduThread(QObject *parent = nullptr);
public:
    static Ctrl_RpduThread *bulid(QObject *parent = nullptr);

protected:
    void funSwitch(uchar *on, uchar *off, int f);
    void funClearEle(uchar *buf);
};

#endif // CTRL_RPDUTHREAD_H
