/*
 *
 *  Created on: 2020年10月1日
 *      Author: Lzy
 */
#include "ctrl_rpduthread.h"

Ctrl_RpduThread::Ctrl_RpduThread(QObject *parent) : Ctrl_CoreThread(parent)
{

}

Ctrl_RpduThread *Ctrl_RpduThread::bulid(QObject *parent)
{
    Ctrl_RpduThread* sington = nullptr;
    if(sington == nullptr)
        sington = new Ctrl_RpduThread(parent);
    return sington;
}


void Ctrl_RpduThread::funSwitch(uchar *on, uchar *off, int f)
{
    uchar cmd[] = {0x7B, 0xA1, 0x10,
                   0xFF, 0xFF, 0xFF, 0xC7, 0xC8, 0xC9,
                   0x00, 0x00, 0x00, 0xD7, 0xD8, 0xD9,
                   0xCC};

    for(int i=0; i<3; ++i) {
        cmd[3+i] = on[i];
        cmd[9+i] = off[i];
    }

    cmd[15] = mModbus->getXorNumber(cmd,sizeof(cmd)-1);
    mModbus->writeSerial(cmd, sizeof(cmd));
}


void Ctrl_RpduThread::funClearEle(uchar *buf)
{
    static uchar cmd[] = {0x7B, 0xD1, 0x00, 0x10,
                          0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00,
                          0x00, 0x00, 0x00, 0x00, 0x00, 0xCC};

    for(int i=0; i<3; i++) cmd[4+i] = buf[i];
    cmd[15] = mModbus->getXorNumber(cmd,sizeof(cmd)-1);
    mModbus->writeSerial(cmd, sizeof(cmd));
}
