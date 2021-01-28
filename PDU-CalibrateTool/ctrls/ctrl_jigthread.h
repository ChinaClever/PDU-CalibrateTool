#ifndef CTRL_JIGTHREAD_H
#define CTRL_JIGTHREAD_H

#include "ctrl_sithread.h"

class Ctrl_JigThread : public QObject
{
    Q_OBJECT
    explicit Ctrl_JigThread(QObject *parent = nullptr);
public:
    static Ctrl_JigThread *bulid(QObject *parent = nullptr);

    bool open();
    bool down();

protected:
    bool openAllSwitch();
    bool closeAllSwitch();
    bool writeSerial(quint8 *cmd, int len);
    bool funSwitch(uchar *on, uchar *off);

protected slots:
    void initSerialSlot();

private:
    SerialPort *mSerial;
};

#endif // CTRL_JIGTHREAD_H
