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

    void initWriteCmd(sRtuSetItems &item);
    bool setThreshold();
    bool inputMacAddr(uchar *buf);
    void setMacAddr();
    bool setModel();
    void setTime();
    bool setclearLog();
};

#endif // CTRL_HEADERTHREAD_H
