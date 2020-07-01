#ifndef AD_ADJUSTING_H
#define AD_ADJUSTING_H

#include "ad_autoid.h"

class Ad_Adjusting : public QThread
{
    Q_OBJECT
    explicit Ad_Adjusting(QObject *parent = nullptr);
public:
    static Ad_Adjusting *bulid(QObject *parent = nullptr);

    bool startAdjust();

protected:
    bool sentCmd();
    bool writeCmd(uchar fn, uchar line);
    bool transmit(uchar *buf, int len);

    bool readData();
    bool recvStatus(uchar *recv, int len);
    bool updateStatus(ushort status);

private:
    sConfigItem *mItem;
    Ad_Modbus *mModbus;
    sDataPacket *mPacket;
};

#endif // AD_ADJUSTING_H
