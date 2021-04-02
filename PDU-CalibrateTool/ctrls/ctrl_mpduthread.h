#ifndef CTRL_MPDUTHREAD_H
#define CTRL_MPDUTHREAD_H

#include "ctrl_zpduthread.h"

class Ctrl_MpduThread : public Ctrl_CoreThread
{
    Q_OBJECT
    explicit Ctrl_MpduThread(QObject *parent = nullptr);
public:
    static Ctrl_MpduThread *bulid(QObject *parent = nullptr);
    bool factorySet();
protected:
    void funSwitch(uchar *on, uchar *off);
    void funClearEle(uchar *buf);
};

#endif // CTRL_MPDUTHREAD_H
