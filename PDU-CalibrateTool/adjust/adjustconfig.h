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

    Test_Result_Wid,
    Test_Items_Wid,
    Test_Datas_Wid
};

struct sConfigItem
{
    sConfigItem() {step=0;}
    SerialPort *serial; // 串口对象

    int step; // 步骤
    int devType; // 执行板类型
    int addr; // 地址
    int mode; // 校准模式
    int cur, curErr; // 电流误差
    int vol, volErr; // 电压误差
    int vert; // 电流校验方式  0 每个校验  1 总电流校验
};


class AdjustConfig : public ConfigBase
{
    AdjustConfig();
public:
    static AdjustConfig *bulid();
     sConfigItem *item;
};

#endif // ADJUSTCONFIG_H