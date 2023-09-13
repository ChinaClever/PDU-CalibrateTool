#ifndef CTRL_HEADERTHREAD_H
#define CTRL_HEADERTHREAD_H

#include "ctrl_jigthread.h"

class Ctrl_IpThread : public Ctrl_CoreThread
{
    Q_OBJECT
    explicit Ctrl_IpThread(QObject *parent = nullptr);
public:
    static Ctrl_IpThread *bulid(QObject *parent = nullptr);
    bool funSetDelayToZero(){return true;}
protected:
    void funSwitch(uchar *, uchar *){}
    void funClearEle(uchar *){}
    bool sentRtuCmd(ushort reg, ushort value);
};

#endif // CTRL_HEADERTHREAD_H
