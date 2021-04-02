/*
 *
 *  Created on: 2020年10月1日
 *      Author: Lzy
 */
#include "ctrl_mpduthread.h"

Ctrl_MpduThread::Ctrl_MpduThread(QObject *parent) : Ctrl_CoreThread(parent)
{

}


Ctrl_MpduThread *Ctrl_MpduThread::bulid(QObject *parent)
{
    Ctrl_MpduThread* sington = nullptr;
    if(sington == nullptr)
        sington = new Ctrl_MpduThread(parent);
    return sington;
}


void Ctrl_MpduThread::funSwitch(uchar *on, uchar *off)
{
    uchar cmd[] = {0x7B, 0xC1, 0x01, 0x15, 0xA1,
                   0xFF, 0xFF, 0xFF, 0xC7, 0xC8, 0xC9,
                   0x00, 0x00, 0x00, 0xD7, 0xD8, 0xD9,
                   0x00, 0x01, 0x00, 0xCC};

    for(int i=0; i<3; ++i) {
        cmd[5+i] = on[i];
        cmd[11+i] = off[i];
    }

    cmd[18] = cmd[2] = mItem->addr;
    cmd[20] = mModbus->getXorNumber(cmd,sizeof(cmd)-1);
    mModbus->writeSerial(cmd, sizeof(cmd));
}


void Ctrl_MpduThread::funClearEle(uchar *buf)
{
    static uchar cmd[] = {0x7B, 0xC1, 0x01, 0x15, 0xD1,
                          0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00,
                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                          0x00, 0x00, 0x00, 0xCC};

    for(int i=0; i<3; i++) cmd[5+i] = buf[i];
    cmd[2] = mItem->addr;
    cmd[20] = mModbus->getXorNumber(cmd,sizeof(cmd)-1);
    mModbus->writeSerial(cmd, sizeof(cmd));
}


bool Ctrl_MpduThread::factorySet()
{
    static uchar cmd[] = {0x7B, 0xA2, 0x17, 0x00, 0x00, 0x00,
                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                          0x00, 0x00, 0x00, 0x00, 0xCC};

    cmd[22] = mModbus->getXorNumber(cmd,sizeof(cmd)-1);
    return mModbus->writeSerial(cmd, sizeof(cmd));
}
