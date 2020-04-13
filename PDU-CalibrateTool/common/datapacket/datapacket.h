#ifndef DATAPACKET_H
#define DATAPACKET_H
#include <QtCore>
#include <QColor>

#define PACK_ARRAY_SIZE 16

// 倍率定义
#define COM_RATE_VOL	10.0    // 电压
#define COM_RATE_CUR	10.0    // 电流
#define COM_RATE_POW	1000.0  // 功率
#define COM_RATE_ELE	10.0    // 电能
#define COM_RATE_PF     100.0   // 功率因数
#define COM_RATE_TEM	10.0    // 温度
#define COM_RATE_HUM	10.0    // 湿度


/**
 * 数据对象：包括电流，电压，功率，电能，开关状态，插接位名称
 */
struct sDataUnit
{
    sDataUnit() {size=0;}
    int size, rate;
    uint vol[PACK_ARRAY_SIZE]; // 电压
    uint cur[PACK_ARRAY_SIZE]; // 电流

    uint pow[PACK_ARRAY_SIZE]; // 功率
    uint ele[PACK_ARRAY_SIZE]; // 电能

    ushort pf[PACK_ARRAY_SIZE]; // 功率因数
    uchar sw[PACK_ARRAY_SIZE]; // 开关状态 0 表示未启用

    uchar hz; // 电压频率
    uint activePow[PACK_ARRAY_SIZE]; // 有功功率值
    uchar status[PACK_ARRAY_SIZE];
};

struct sTgObjData
{
    uint vol; // 电压
    uint cur;  // 电流
    uint pow; // 功率

    uint ele; // 电能
    uint pf; // 功率因数
    uint activePow; // 袖在功率
};


/**
 * 数据包
 */
class sDataPacket
{
    sDataPacket();
public:
    static sDataPacket *bulid();

    void clear();
    int tgCur();

    sTgObjData *tg; // 统计数据
    sDataUnit *data;
    QString status;
    int pass;
};



#endif // DATAPACKET_H
