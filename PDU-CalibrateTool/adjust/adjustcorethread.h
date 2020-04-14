#ifndef ADJUSTCORETHREAD_H
#define ADJUSTCORETHREAD_H

#include "datapacket.h"
#include "adjustconfig.h"


class AdjustCoreThread : public QThread
{
    Q_OBJECT
public:
    explicit AdjustCoreThread(QObject *parent = nullptr);
    ~AdjustCoreThread();

signals:

protected:
    void run();
    bool delay(int s);
    void workDown();

    int readSerial(quint8 *cmd, int sec);
    bool writeSerial(quint8 *cmd, int len);
    int transmit(uchar *sent, int len, uchar *recv, int sec);

    void sendModeCmd();
    void sendGainCmd();
    void sendActivateCmd();

    virtual bool resActivationCmd()=0;
    virtual bool startActivationCmd()=0;
    bool resActivateVert(uchar *cmd, int len);
    quint8 getXorNumber(quint8 *data,int len);

    virtual int openAllSwitch()=0;
    virtual bool readPduData()=0;
    virtual void clearPduEle()=0;

    bool curAllAdjust();
    bool curOneAdjust();
    bool volAdjust();

    bool dataAdjust();
    bool pduAdjust();
    void resTgData(sTgObjData *tg);
    void workResult(bool res);


protected:
    bool isRun;
    sDataUnit *mData;
    sConfigItem *mItem;
    SerialPort *mSerial;
    sDataPacket *mPacket;
};

#endif // ADJUSTCORETHREAD_H
