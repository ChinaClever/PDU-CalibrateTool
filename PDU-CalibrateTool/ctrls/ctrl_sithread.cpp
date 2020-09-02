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
    sRtuSetItem it;
    it.addr = mItem->addr;
    it.fn = 0x10;
    it.reg = 0x1013;
    it.data = 0xFF00;
    mModbus->rtuWrite(&it);
}

bool Ctrl_SiThread::sentRtuCmd(ushort reg, ushort value)
{
    sRtuSetItem it;
    it.addr = mItem->addr;
    it.fn = 0x10;
    it.reg = reg;
    it.data = value;

    return mModbus->rtuWrite(&it);
}


bool Ctrl_SiThread::setAcTh()
{
    bool ret = true;
    ushort reg = 0x1002;

    for(int i=0; i<mData->size; ++i) {
        ret = sentRtuCmd(reg++, mItem->cTh.vol_max); if(!ret) return ret;
        ret = sentRtuCmd(reg++, mItem->cTh.vol_min); if(!ret) return ret;
    }

    reg = 0x1008;
    for(int i=0; i<mData->size; ++i) {
        ret = sentRtuCmd(reg++, mItem->cTh.cur_max/10); if(!ret) return ret;
        ret = sentRtuCmd(reg++, mItem->cTh.cur_min/10); if(!ret) return ret;
    }

    return ret;
}

bool Ctrl_SiThread::setDcTh()
{
    bool ret = true;
    ushort reg = 0x1014;

    ret = sentRtuCmd(reg++, mItem->cTh.vol_max); if(!ret) return ret;
    ret = sentRtuCmd(reg++, mItem->cTh.vol_min); if(!ret) return ret;
    ret = sentRtuCmd(reg++, mItem->cTh.cur_max/10); if(!ret) return ret;
    ret = sentRtuCmd(reg++, mItem->cTh.cur_min/10); if(!ret) return ret;

    return ret;
}




bool Ctrl_SiThread::setThreshold()
{
    bool ret = true;
    if(mItem->cTh.type > 0) {
        sDevType *devType = sDataPacket::bulid()->devType;
        if(DC == devType->ac) {
            ret = setDcTh();
        } else {
            ret = setAcTh();
        }

        mPacket->status = tr("出厂阈值设置");
        mModbus->appendLogItem(ret);
    }

    return ret;
}

// 表示行业标准 Modbus RTU 模式
bool Ctrl_SiThread::setModel()
{
    sRtuSetItem it;
    it.addr = mItem->addr;
    it.fn = 0x06;
    it.reg = 0x1019;
    it.data = 1;

    bool ret = mModbus->rtuWrite(&it);
    mPacket->status = tr("切换成行业标准模式");
    mModbus->appendLogItem(ret);

    return ret;
}

bool Ctrl_SiThread::factorySet()
{
    bool ret = setThreshold();
    if(ret) {
        if(mItem->cTh.ele_clear > 0) funClearEle(nullptr);
        if(mItem->cTh.si_mod > 0) ret = setModel();
    }

    return ret;
}
