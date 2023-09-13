#ifndef AD_AUTOID_H
#define AD_AUTOID_H

#include "ad_devtype.h"

class Ad_AutoID : public QThread
{
    Q_OBJECT
    explicit Ad_AutoID(QObject *parent = nullptr);
public:
     static Ad_AutoID *bulid(QObject *parent = nullptr);

    bool readDevType();
    bool readDevValue(int size , QString & valStr);
protected:
    bool readDevId();
    void initReadType(sRtuItem &it);
    bool analysDevType(uchar *buf, int len);

    bool readDevVal(int size , QString & valStr);
    void initReadCalibrationsVal(sRtuItem &it , int size);
    bool analysDevCalibrationValue(uchar *buf, int len ,int size , QString &valStr);

private:
    sConfigItem *mItem;
    Ad_Modbus *mModbus;
    sDataPacket *mPacket;
    Ad_DevType *mDevType;
    sDevType *mDt;
};

#endif // AD_AUTOID_H
