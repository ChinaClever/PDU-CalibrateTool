#ifndef DATAPACKET_H
#define DATAPACKET_H
#include <QtCore>
#include <QColor>

#define DEV_NUM 3
#define PACK_ARRAY_SIZE 16

// 倍率定义
#define COM_RATE_VOL	1.0    // 电压
#define COM_RATE_CUR	10.0    // 电流
#define COM_RATE_POW	1000.0  // 功率
#define COM_RATE_ELE	10.0    // 电能
#define COM_RATE_PF     100.0   // 功率因数
#define COM_RATE_TEM	10.0    // 温度
#define COM_RATE_HUM	10.0    // 湿度

struct sThreshold {
    ushort min[PACK_ARRAY_SIZE];
    ushort max[PACK_ARRAY_SIZE];
};

/**
 * 数据对象：包括电流，电压，功率，电能，开关状态，插接位名称
 */
struct sDataUnit
{
    sDataUnit() {size=0;}
    int size;
    double rate;
    sThreshold volTh;
    ushort vol[PACK_ARRAY_SIZE]; // 电压

    sThreshold curTh;
    ushort cur[PACK_ARRAY_SIZE]; // 电流
    ushort cured[PACK_ARRAY_SIZE]; // 电流
    ushort pow[PACK_ARRAY_SIZE]; // 功率
    ushort powed[PACK_ARRAY_SIZE]; // 功率
    uint ele[PACK_ARRAY_SIZE]; // 电能

    uchar pf[PACK_ARRAY_SIZE]; // 功率因数
    uchar sw[PACK_ARRAY_SIZE]; // 开关状态  0 表示未启用 1 表示开
    uchar tem;
    uchar hum;

    uchar hz; // 电压频率
    uchar br; // 波特率
    uchar version;
    uchar chipStatus;
    ushort activePow[PACK_ARRAY_SIZE]; // 有功功率值
    uchar status[PACK_ARRAY_SIZE];
    ushort reserve;
    uchar ip[18];
};

struct sTgObjData
{
    uint vol; // 电压
    uint cur;  // 电流
    uint pow; // 功率

    uint ele; // 电能
    uint pf; // 功率因数
    uint activePow; // 视在功率
};


struct sDevType
{
    sDevType() {devType=0;}

    uint devId;
    uchar devType; // 1 ZPDU执行板类型  2 MPDU执行板类型   3 RPDU执行板类型
                   // 4 SI-PDU  5 IP-PDU  6 BM-PDU
    uchar devBothType;

    uchar ac; // 1 交流 2 直流
    uchar series; // 1 A系列  2 B系列  3 C系列  4 D系列1
    uchar lines;
    uchar outputs;

    uchar specs; // 1 互感器  2锰铜
    uchar version; // 版本号
};

struct sDevData
{
    sDevData() {id=1; en=0;}

    uchar id, en;  // 设备号
    sDevType devType; //设备类型
    uchar offLine; //离线标志 > 0在线

    sTgObjData line; // 相数据
    // sEnvData env; // 环境数据

    uchar hz; // 电压频率
    ushort br;  // 00	表示波特率9600(00默认9600，01为4800，02为9600，03为19200，04为38400)
    ushort version;
    ushort reserve;
};

struct sProgress
{
    sProgress() {step=0;}

    uchar step; // 步骤
    QString time;
    uchar recordstep; // 记录启动步骤
    QList<bool> pass, itPass;
    QStringList status, item;

    uchar result;    // 最终结果
    QTime startTime;

    QString softwareType;
    QString companyName;
    QString protocolVersion;

    QString productType;
    QString productSN;
    QString macAddress;
    // uchar softwareVersion;
    QString softwareVersion;
    QString clientName;
    QString testTime;
    QString testStartTime;
    QString testEndTime;
    QStringList no, itemName;
    QList<bool> uploadPass;
    uchar uploadPassResult;

};

/**
 * 数据包
 */
class sDataPacket
{
    sDataPacket();
public:
    static sDataPacket *bulid();

    // void clear();
    int tgCur();

    sTgObjData *tg; // 统计数据
    sDataUnit *data;
    sDevType *devType;

    QString dev_type;
    QString sn;

    QString status;
    int pass;

    void init();
    sProgress *getPro() {return pro;}
    sDevData *getDev(int id=1) {return dev[id];}
    bool updatePro(const QString &str, bool pass=true, int sec=1);
    bool delay(int s=1);
    void clear(int id=1);
protected:

    sDevData *dev[DEV_NUM];
    sProgress *pro;
};



#endif // DATAPACKET_H
