#ifndef AD_ADJUSTING_H
#define AD_ADJUSTING_H

#include "ad_autoid.h"
#include "yc_standsource.h"

class Ad_Adjusting : public QThread
{
    Q_OBJECT
    explicit Ad_Adjusting(QObject *parent = nullptr);
public:
    static Ad_Adjusting *bulid(QObject *parent = nullptr);
    bool startAdjust(YC_StandSource *source);

protected:
    bool sentCmd(YC_StandSource *source);
    bool writeCmd(uchar fn, uchar line);
    bool transmit(uchar *buf, int len);

    bool readData();
    bool waitDcRecv();
    int readSerial(uchar *recv, int sec=15);
    bool recvStatus(uchar *recv, int len);
    bool updateStatus(ushort status);
    bool overWork(const QString &str);

private:
    sConfigItem *mItem;
    Ad_Modbus *mModbus;
    sDataPacket *mPacket;
};

#endif // AD_ADJUSTING_H
