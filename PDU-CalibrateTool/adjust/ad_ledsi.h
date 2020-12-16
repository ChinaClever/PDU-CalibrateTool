#ifndef AD_LEDSI_H
#define AD_LEDSI_H

#include "ad_adjusting.h"

class Ad_LedSi : public QThread
{
    Q_OBJECT
    explicit Ad_LedSi(QObject *parent = nullptr);
public:
    static Ad_LedSi *bulid(QObject *parent = nullptr);
    bool startAdjust(YC_StandSource *source);

protected:
    bool sentCmd();
    bool writeCmd(uchar fn, uchar line);
    bool transmit(uchar *buf, int len);

    bool writeDc();
    bool writeAc();

private:
    sConfigItem *mItem;
    Ad_Modbus *mModbus;
    sDataPacket *mPacket;
    YC_StandSource *mSource;
};

#endif // AD_LEDSI_H
