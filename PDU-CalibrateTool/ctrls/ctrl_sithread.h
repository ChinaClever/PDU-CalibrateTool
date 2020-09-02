#ifndef CTRL_SITHREAD_H
#define CTRL_SITHREAD_H

#include "ctrl_rpduthread.h"

class Ctrl_SiThread : public Ctrl_CoreThread
{
    Q_OBJECT
    explicit Ctrl_SiThread(QObject *parent = nullptr);
public:
    static Ctrl_SiThread *bulid(QObject *parent = nullptr);

    bool factorySet();

protected:
    void funSwitch(uchar *on, uchar *off, int f);
    void funClearEle(uchar *buf);

    void initAcCmd(sRtuSetItem &item);
    void initDcCmd(sRtuSetItem &item);
    void initAcCmd(QList<sRtuSetItem>& item,unsigned short reg);
    void initDcCmd(QList<sRtuSetItem>& item,unsigned short reg);
    void initWriteCmd(QList<sRtuSetItem> &item, unsigned short reg);
    bool setThreshold();
    bool setModel();
};

#endif // CTRL_SITHREAD_H
