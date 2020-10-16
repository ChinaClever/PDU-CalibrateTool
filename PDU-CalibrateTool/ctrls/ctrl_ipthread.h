#ifndef CTRL_HEADERTHREAD_H
#define CTRL_HEADERTHREAD_H

#include "ctrl_sithread.h"

class Ctrl_IpThread : public Ctrl_CoreThread
{
    Q_OBJECT
    explicit Ctrl_IpThread(QObject *parent = nullptr);
public:
    static Ctrl_IpThread *bulid(QObject *parent = nullptr);

    bool factorySet();

protected:
    void funSwitch(uchar *on, uchar *off, int f);
    void funClearEle(uchar *buf);
    bool startProcess();
    bool updateMacAddr();
    bool checkNet();

    bool initDev();
    bool sentRtuCmd(ushort reg, ushort value);
    bool inputMacAddr(uchar *buf);
};

#endif // CTRL_HEADERTHREAD_H
