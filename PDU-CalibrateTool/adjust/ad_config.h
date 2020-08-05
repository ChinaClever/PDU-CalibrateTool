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



struct sConfigItem
{
    sConfigItem() {step=0; vol=200; currentNum=0;addr=1;}
    SerialPort *serial; // 串口对象
    SerialPort *source; // 标准源

    uchar step; // 步骤
    uchar addr;

    ushort vol;
    ushort volErr, curErr, powErr; // 电流误差
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
protected:
    bool getDate();
    void setDate();

    void initCurrentNum();
};

#endif // ADJUSTCONFIG_H
