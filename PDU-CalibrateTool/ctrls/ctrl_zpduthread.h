#ifndef CTRL_ZPDUTHREAD_H
#define CTRL_ZPDUTHREAD_H

#include "ctrl_corethread.h"

class Ctrl_ZpduThread : public Ctrl_CoreThread
{
    Q_OBJECT
    explicit Ctrl_ZpduThread(QObject *parent = nullptr);
public:
    static Ctrl_ZpduThread *bulid(QObject *parent = nullptr);

protected:
    void funSwitch(uchar *on, uchar *off);
    void funClearEle(uchar *buf);
};

#endif // CTRL_ZPDUTHREAD_H
