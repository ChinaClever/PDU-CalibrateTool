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
    bool readData();
    bool recvStatus(uchar *buf, int len);
    bool updateStatus(uchar status);

private:
    sConfigItem *mItem;
    Ad_Modbus *mModbus;
    sDataPacket *mPacket;
};

#endif // AD_ADJUSTING_H
