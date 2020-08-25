#ifndef ADJUSTCONFIG_H
#define ADJUSTCONFIG_H

#include "configbase.h"
#include "serialstatuswid.h"

enum {
    Test_Over, // 功能 终止
    Test_Start, // 开始
    Test_Ading,
    Test_vert, // 验证
    Test_End, // 完成
    Collect_Start, // 数据采集

    Test_Success=1,
    Test_Fail,
};

struct sConfigThreshold {
    uchar type; // 0 不设置， 1 32安  2 16安， 3 63安  4 自定义
    ushort vol_min;
    ushort vol_max;
    ushort cur_min;
    ushort cur_max;

    uchar si_mod; // 1 表示行业标准 Modbus RTU 模式
    uchar ele_clear; // 电能清除
    uchar ip_mod; //  1 表示 Modbus版本
    uchar set_mac; // 设置MAC
    uchar mac_clear;  // MAC地址自动加一，清空
    char mac_addr[32]; //  MAC地址
};

struct sConfigItem
{
    sConfigItem() {step=0; vol=200; currentNum=0;addr=1;}
    SerialPort *serial; // 串口对象
    SerialPort *source; // 标准源

    uchar step; // 步骤
    uchar addr;

    ushort vol;
    ushort volErr, curErr, powErr; // 电流误差
    sConfigThreshold cTh;

    int logCount;

    uchar pcNum;
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
    QString initName();
    void setName(QString str);

    int getAddr();
    void setAddr(int addr);

    void writeErrData();
    void writeThreshold();
    void setMacAddr(const QString &name);

protected:
    bool getDate();
    void setDate();

    QString getMacAddr();
    void initCurrentNum();
    void initErrData();
    void initThreshold();
};

#endif // ADJUSTCONFIG_H
