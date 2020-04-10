#ifndef ADJUSTCORETHREAD_H
#define ADJUSTCORETHREAD_H

#include "datapacket.h"
#include "adjustconfig.h"


class AdjustCoreThread : public QThread
{
    Q_OBJECT
    explicit AdjustCoreThread(QObject *parent = nullptr);
public:
    static AdjustCoreThread *bulid(QObject *parent = nullptr);
    ~AdjustCoreThread();

signals:

protected:
    void run();
    void delay(int s);
    void workDown();

    bool writeSerial(quint8 *cmd, int len);
    bool startActivationCmd();
    bool resActivationCmd();
    bool resActivateVert(uchar *cmd, int len);

public slots:

private:
    sDataUnit *mData;
    sConfigItem *mItem;
    SerialPort *mSerial;
};

#endif // ADJUSTCORETHREAD_H
