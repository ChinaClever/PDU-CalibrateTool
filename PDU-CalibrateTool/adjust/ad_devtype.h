#ifndef AD_DEVTYPE_H
#define AD_DEVTYPE_H
#include "ad_modbus.h"

class Ad_DevType
{
    Ad_DevType();
public:
    static Ad_DevType *bulid();

    bool analysDevType(uint id);

protected:
    QString zpduStrs(uint id);
    void zpduDevType(uint id, sDevType *dt);

private:
    sDevType *mDt;
    sDataPacket *mPacket;
};

#endif // AD_DEVTYPE_H
