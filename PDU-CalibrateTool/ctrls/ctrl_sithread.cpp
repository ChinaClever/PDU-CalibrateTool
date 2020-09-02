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
    sentRtuCmd(0x1013, 0xFF00);
}


int Ctrl_SiThread::initRtu(ushort reg, ushort value, uchar *buf)
{
    int k = 0;
    buf[k++] = mItem->addr;
    buf[k++] = 0x10;
    buf[k++] = reg >> 8;
    buf[k++] = reg & 0xFF;
    buf[k++] = value >> 8;
    buf[k++] = value & 0xFF;

    ushort crc = mModbus->rtu_crc(buf, k);
    buf[k++] = crc & 0xFF;
    buf[k++] = crc >> 8;

    return k;
}

bool Ctrl_SiThread::writeRtu(uchar *buf, int len)
{
    bool ret = false;
    uchar recv[32] = {0};
    len = mModbus->transmit(buf, len, recv, 2);
    if(len > 0) {
        if(recv[1] == buf[1])
            ret = true;
    }
    return ret;
}

bool Ctrl_SiThread::sentRtuCmd(ushort reg, ushort value)
{
    uchar buf[32] = {0};
    int len = initRtu(reg, value, buf);

    return writeRtu(buf, len);
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
    bool ret = false;
    uchar len = 8;
    uchar buf[20] = {0x01, 0x06, 0x10, 0x19, 0x00, 0x01, 0x5C, 0xCD};
    buf[0] = mItem->addr;

    ushort crc = mModbus->rtu_crc(buf, len-2);
    buf[6] = 0xff&crc; /*低8位*/
    buf[7] = crc >> 8; /*高8位*/
    mModbus->delay(1);
    len = mModbus->transmit(buf, len, buf, 1);
    if(len > 0) ret = true;

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
