#ifndef AD_DEVTYPE_H
#define AD_DEVTYPE_H
#include "ad_macaddr.h"

enum eDevTypes {
    PDU=0,
    ZPDU,
    MPDU,
    RPDU,     // RPDU 执行板
    RPDU_Mc, // RPDU 主控
    SI_PDU,
    IP_PDU,
    BM_PDU,
    APDU,

    AC = 1, // 交流
    DC,     // 直流

    Sum=0,
    Transformer, // 互感器
    Mn    // 锰铜
};


class Ad_DevType
{
    Ad_DevType();
public:
    static Ad_DevType *bulid();
    bool analysDevType(uint id);

protected:
    QString getDevStr(uint id);
    int getDevType(const QString &str);
    int getAcType(const QString &str);
    int getColMode(const QString &str);
    int getLineNum(const QString &str);
    int getSerie(const QString &str);

private:
    sDevType *mDt;
    sDataPacket *mPacket;
};

#endif // AD_DEVTYPE_H
