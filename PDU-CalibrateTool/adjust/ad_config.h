#ifndef ADJUSTCONFIG_H
#define ADJUSTCONFIG_H

#include "configbase.h"
#include "serialstatuswid.h"

enum {
    Test_Over, // 功能 终止
    Test_Start, // 开始
    Test_Ading,
    Test_vert, // 验证
    Tset_Collect, // 数据采集
    Test_End, // 完成

    Test_Info=0,
    Test_Success=1,
    Test_Fail=2,
};


/**
 * RTU传输统计结构体
 */
struct sCount
{
    uint num;
    uint all;
    uint ok;
    uint err;
};

struct sConfigItem
{
    sConfigItem() {step=0; vol=200; currentNum=0;addr=1;}
    SerialPort *serial; // 串口对象
    SerialPort *source; // 标准源
    SerialPort *jig; // 治具

    uchar step; // 步骤
    uchar addr;

    ushort vol;
    ushort volErr, curErr, powErr; // 电流误差

    int logCount;
    sCount cnt;

    bool si_led;
    uchar si_ac;
    uchar si_line;

    QString user; // 客户名称
    uchar pcNum;
    uchar aiMode;
    ushort currentNum; // 当天序号
    QString currentName; // 当前用户名称
    QTime startTime;
};


class Ad_Config : public ConfigBase
{
    Ad_Config();
public:
    static Ad_Config *bulid();
    sConfigItem *item;

    void setCurrentNum();
    int getAddr();
    void setAddr(int addr);

    void writeCnt();
    void writeErrData();

protected:
    bool getDate();
    void setDate();

    void initCurrentNum();
    void initErrData();
    void initCnt();
};

#endif // ADJUSTCONFIG_H
