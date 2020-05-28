#ifndef ADJUSTCORETHREAD_H
#define ADJUSTCORETHREAD_H

#include "datapacket.h"
#include "adjustconfig.h"
#include "standardsource.h"

class AdjustCoreThread : public QThread
{
    Q_OBJECT
public:
    explicit AdjustCoreThread(QObject *parent = nullptr);
    ~AdjustCoreThread();

    virtual void openAllSwitch();
    virtual void closeAllSwitch();
    virtual void openOutputSwitch(int id);
    virtual void closeOutputSwitch(int id);

    virtual void clearAllEle();
    virtual void setClearEle(int id);

signals:

protected:
    void run();
    bool delay(int s);
    void workDown();
    void collectData();

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

    virtual bool readPduData()=0;

    bool curAllAdjust();
    bool curOneAdjust();
    bool volAdjust();

    bool dataAdjust();
    bool pduAdjust();
    void resTgData(sTgObjData *tg);
    void workResult(bool res);

    void changeSwitch();
    void setBitControl(int id, uchar *buf);
    virtual void funClearEle(uchar *buf)=0;
    virtual void funSwitch(uchar *on, uchar *off, int f)=0;

protected:
    bool isRun;
    sDataUnit *mData;
    sConfigItem *mItem;
    SerialPort *mSerial;
    sDataPacket *mPacket;
    StandardSource *mSource;
};

#endif // ADJUSTCORETHREAD_H
