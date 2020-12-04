/*
 *
 *  Created on: 2020年10月1日
 *      Author: Lzy
 */
#include "ctrl_ipthread.h"

Ctrl_IpThread::Ctrl_IpThread(QObject *parent) : Ctrl_CoreThread(parent)
{

}

Ctrl_IpThread *Ctrl_IpThread::bulid(QObject *parent)
{
    Ctrl_IpThread* sington = nullptr;
    if(sington == nullptr)
        sington = new Ctrl_IpThread(parent);
    return sington;
}

void Ctrl_IpThread::funSwitch(uchar *on, uchar *off, int f)
{

}

void Ctrl_IpThread::funClearEle(uchar *buf)
{   
}


bool Ctrl_IpThread::sentRtuCmd(ushort reg, ushort value)
{
    sRtuSetItem it;
    it.addr = mItem->addr;
    if(mPacket->devType->version == 3) {
        it.fn = 0x06; //V3 写 0x06
    } else{
        it.fn = 0x10;//V1 写 0x10
    }

    it.reg = reg;
    it.data = value;

    mModbus->delay(1);
    return mModbus->rtuWrite(&it);
}


