#ifndef CTRL_MPDUTHREAD_H
#define CTRL_MPDUTHREAD_H

#include "ctrl_zpduthread.h"

class Ctrl_MpduThread : public Ctrl_CoreThread
{
    Q_OBJECT
    explicit Ctrl_MpduThread(QObject *parent = nullptr);
public:
    static Ctrl_MpduThread *bulid(QObject *parent = nullptr);

protected:
    void funSwitch(uchar *on, uchar *off, int f);
    void funClearEle(uchar *buf);
};

#endif // CTRL_MPDUTHREAD_H
