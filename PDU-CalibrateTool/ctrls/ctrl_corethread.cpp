#include "ctrl_corethread.h"

Ctrl_CoreThread::Ctrl_CoreThread(QObject *parent) : QThread(parent)
{
    mData = sDataPacket::bulid()->data;
    mItem = AdjustConfig::bulid()->item;
    mModbus = Ad_Modbus::bulid(this);
}


void Ctrl_CoreThread::openAllSwitch()
{
    uchar on[8], off[8];
    for(int i=0; i<6; i++)  on[i] = 0xFF;  //打开有效位
    for(int i=0; i<6; i++)  off[i] = 0x00;  //关闭有效位

    funSwitch(on, off, 1);
}

void Ctrl_CoreThread::setBitControl(int id, uchar *buf)
{
    int index = (mData->size * (mItem->addr-1) +id) / 8;
    int key = (mData->size * (mItem->addr-1) +id) % 8 ;
    buf[index] |= 0x80 >> key;
}

void Ctrl_CoreThread::openOutputSwitch(int id)
{
    uchar on[8], off[8];
    for(int i=0; i<6; i++)  on[i] = 0x00;  //打开有效位
    for(int i=0; i<6; i++)  off[i] = 0x00;  //关闭有效位

    if(id > 0) {
        setBitControl(id, on);
        funSwitch(on, off, 0);
    }
}

void Ctrl_CoreThread::openOnlySwitch(int id)
{
    openOutputSwitch(id);
    sleep(1);  // delay(1);
    closeOtherSwitch(id);
}

void Ctrl_CoreThread::closeOtherSwitch(int id)
{
    uchar on[8], off[8];
    for(int i=0; i<6; i++)  on[i] = 0x00;  //打开有效位
    for(int i=0; i<6; i++)  off[i] = 0x00;  //关闭有效位

    if(id > 0) {
        setBitControl(id, off);
        for(int i=0; i<6; i++)  off[i] = ~off[i];
        funSwitch(on, off, 0);
    }
}

void Ctrl_CoreThread::closeAllSwitch()
{
    uchar on[8], off[8];
    for(int i=0; i<6; i++)  on[i] = 0x00;  //打开有效位
    for(int i=0; i<6; i++)  off[i] = 0xff;  //关闭有效位

    funSwitch(on, off, 1);
}


void Ctrl_CoreThread::closeOutputSwitch(int id)
{
    uchar on[8], off[8];
    for(int i=0; i<6; i++)  on[i] = 0x00;  //打开有效位
    for(int i=0; i<6; i++)  off[i] = 0x00;  //关闭有效位

    if(id > 0) {
        setBitControl(id, off);
        funSwitch(on, off, 0);
    }
}


void Ctrl_CoreThread::clearAllEle()
{
    uchar cmd[8];
    for(int i=0; i<6; i++) cmd[i] = 0xFF;
    funClearEle(cmd);
}



void Ctrl_CoreThread::setClearEle(int id)
{
    uchar cmd[8];
    for(int i=0; i<6; i++) cmd[i] = 0;

    setBitControl(id, cmd);
    funClearEle(cmd);
}

