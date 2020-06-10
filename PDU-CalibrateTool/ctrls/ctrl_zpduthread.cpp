#include "ctrl_zpduthread.h"

Ctrl_ZpduThread::Ctrl_ZpduThread(QObject *parent) : Ctrl_CoreThread(parent)
{

}


Ctrl_ZpduThread *Ctrl_ZpduThread::bulid(QObject *parent)
{
    Ctrl_ZpduThread* sington = nullptr;
    if(sington == nullptr)
        sington = new Ctrl_ZpduThread(parent);
    return sington;
}


void Ctrl_ZpduThread::funSwitch(uchar *on, uchar *off, int f)
{
    int k = 5;
    static uchar cmd[68] = {0x7B, 0xC1, 0x00, 0xA2, 0xB2};

    for(int i=0; i<6; i++)  cmd[k++] = on[i];  //打开有效位
    for(int i=0; i<6; i++)  cmd[k++] = off[i];  //关闭有效位

    //预留位
    for(int i=0; i<3; i++)  cmd[k++] = 0xC7 + i;
    for(int i=0; i<3; i++)  cmd[k++] = 0xD7 + i;
    for(int i=0; i<39; i++)  cmd[k++] = 0x00;
    cmd[k++] = f; // 统一开关为1

    for(int i=0; i<3; i++)  cmd[k++] = 0x0E;
    cmd[k++] = 0x44;
    cmd[k] = mModbus->getXorNumber(cmd,sizeof(cmd)-1);
    writeSerial(cmd, sizeof(cmd));
}


void Ctrl_ZpduThread::funClearEle(uchar *buf)
{
    int k = 5;
    static uchar cmd[68] = {0x7B, 0xC1, 0x00, 0xA4, 0xB4};

    for(int i=0; i<6; i++) cmd[k++] = buf[i];
    for(int i=0; i<52; i++) cmd[k++] = 0x00;  //预留位
    for(int i=0; i<3; i++) cmd[k++] = 0x0E;
    cmd[k++] = 0x44;
    cmd[k] = mModbus->getXorNumber(cmd,sizeof(cmd)-1); //异或校验码
    writeSerial(cmd, sizeof(cmd));
}
