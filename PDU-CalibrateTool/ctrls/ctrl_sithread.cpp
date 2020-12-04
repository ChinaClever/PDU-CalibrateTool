/*
 *
 *  Created on: 2020年10月1日
 *      Author: Lzy
 */
#include "ctrl_sithread.h"

Ctrl_SiThread::Ctrl_SiThread(QObject *parent) : Ctrl_CoreThread(parent)
{

}

Ctrl_SiThread *Ctrl_SiThread::bulid(QObject *parent)
{
    Ctrl_SiThread* sington = nullptr;
    if(sington == nullptr)
        sington = new Ctrl_SiThread(parent);
    return sington;
}

void Ctrl_SiThread::funSwitch(uchar *on, uchar *off, int f)
{

}

void Ctrl_SiThread::funClearEle(uchar *buf)
{
    mPacket->status = tr("清除电能");
    sentRtuCmd(0x1013, 0xFF00);
}

bool Ctrl_SiThread::sentRtuCmd(ushort reg, ushort value)
{
    sRtuSetItem it;
    it.addr = mItem->addr;
    it.fn = 0x10;
    it.reg = reg;
    it.data = value;

    mModbus->delay(1);
    return mModbus->rtuWrite(&it);
}

