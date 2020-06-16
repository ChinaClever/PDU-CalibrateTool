#ifndef ADJUSTCONFIG_H
#define ADJUSTCONFIG_H

#include "configbase.h"
#include "serialstatuswid.h"

enum {
    Test_Function, // 功能测试
    Test_Start, // 开始
    Test_vert, // 验证
    Test_End, // 完成
    Test_Over, // 终止

    Collect_Start,

    Test_Result_Wid,
    Test_Items_Wid,
    Test_Datas_Wid
};

struct sConfigItem
{
    sConfigItem() {step=0; vol=220;}
    SerialPort *serial; // 串口对象
    SerialPort *source; // 标准源

    uchar step; // 步骤
    uchar devType; // 执行板类型
    QString dev_type;

    uchar addr; // 地址
    uchar ac;
    uchar outputs; // 输出位数量
    uchar adMode; // 校准模式
    uchar vert; // 电流校验方式  0 每个校验  1 总电流校验
    uchar version;

    ushort vol;
    ushort volErr, curErr; // 电流误差
    int logCount;

    uchar pcNum;
    ushort currentNum;
    QString sn;
};


class Ad_Config : public ConfigBase
{
    Ad_Config();
public:
    static Ad_Config *bulid();
     sConfigItem *item;

};

#endif // ADJUSTCONFIG_H
