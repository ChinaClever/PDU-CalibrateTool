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

bool Ctrl_SiThread::setAcTh()
{
    bool ret = true;
    ushort reg = 0x1002;

    for(int i=0; i<mData->size; ++i) {

        ushort value = mItem->cTh.vol_max;
        if(mData->volTh.max[i] != value) {
            mPacket->status = tr("L%1 电压最大值设置 %2V").arg(i+1).arg(value);
            ret = sentRtuCmd(reg++, value);
            if(!ret) return ret;
        } else reg++;

        value = mItem->cTh.vol_min;
        if(mData->volTh.min[i] != value) {
            mPacket->status = tr("L%1 电压最小值设置 %2V").arg(i+1).arg(value);
            ret = sentRtuCmd(reg++, value);
            if(!ret) return ret;
        } else reg++;
    }

    reg = 0x1008;
    for(int i=0; i<mData->size; ++i) {
        ushort value = mItem->cTh.cur_max;
        if(mData->curTh.max[0] != value) {
            mPacket->status = tr("L%1 电流最大值设置 %2A").arg(i+1).arg(value/10);
            ret = sentRtuCmd(reg++, value);
            if(!ret) return ret;
        } else reg++;

        value = mItem->cTh.cur_min;
        if(mData->curTh.min[0] != value) {
            mPacket->status = tr("L%1 电流最小值设置 %2A").arg(i+1).arg(value/10);
            ret = sentRtuCmd(reg++, value);
            if(!ret) return ret;
        } else reg++;
    }

    return ret;
}

bool Ctrl_SiThread::setDcTh()
{
    bool ret = true;
    ushort reg = 0x1014;

    ushort value = mItem->cTh.vol_max;
    if(mData->volTh.max[0] != value) {
        mPacket->status = tr("电压最大值设置 %1V").arg(value);
        ret = sentRtuCmd(reg++, value);
        if(!ret) return ret;
    } else reg++;

    value = mItem->cTh.vol_min;
    if(mData->volTh.min[0] != value) {
        mPacket->status = tr("电压最小值设置 %1V").arg(value);
        ret = sentRtuCmd(reg++, value);
        if(!ret) return ret;
    } else reg++;

    value = mItem->cTh.cur_max;
    if(mData->curTh.max[0] != value) {
        mPacket->status = tr("电流最大值设置 %1A").arg(value/10);
        ret = sentRtuCmd(reg++, value);
        if(!ret) return ret;
    } else reg++;

    value = mItem->cTh.cur_min;
    if(mData->curTh.min[0] != value) {
        mPacket->status = tr("电流最小值设置 %1A").arg(value/10);
        ret = sentRtuCmd(reg++, value);
        if(!ret) return ret;
    } else reg++;

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

        if(ret) mPacket->status = tr("出厂阈值设置");
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

    mPacket->status = tr("切换成行业标准模式");
    bool ret = mModbus->rtuWrite(&it);
    mModbus->appendLogItem(ret);

    return ret;
}

bool Ctrl_SiThread::factorySet()
{
    bool ret = true;
    if(!mItem->cTh.repair_en) {
        ret = setThreshold();
        if(ret) {
            // funClearEle(nullptr);
            if(mItem->cTh.si_mod > 0) ret = setModel();
        }
    }

    return ret;
}
