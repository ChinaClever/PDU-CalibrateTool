#ifndef CTRL_RPDUTHREAD_H
#define CTRL_RPDUTHREAD_H

#include "ctrl_apduthread.h"

class Ctrl_RpduThread : public Ctrl_CoreThread
{
    Q_OBJECT
    explicit Ctrl_RpduThread(QObject *parent = nullptr);
public:
    static Ctrl_RpduThread *bulid(QObject *parent = nullptr);
    bool funSetDelayToZero(){return true;}

protected:
    void funSwitch(uchar *on, uchar *off);
    void funClearEle(uchar *buf);
};

#endif // CTRL_RPDUTHREAD_H
