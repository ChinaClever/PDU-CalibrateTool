#ifndef CTRL_HEADERTHREAD_H
#define CTRL_HEADERTHREAD_H

#include "ctrl_rpduthread.h"

class Ctrl_HeaderThread : public Ctrl_CoreThread
{
    Q_OBJECT
    explicit Ctrl_HeaderThread(QObject *parent = nullptr);
public:
    static Ctrl_HeaderThread *bulid(QObject *parent = nullptr);

protected:
    void funSwitch(uchar *on, uchar *off, int f){}
    void funClearEle(uchar *buf){}
};

#endif // CTRL_HEADERTHREAD_H
