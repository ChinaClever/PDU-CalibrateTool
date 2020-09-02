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
    uchar len = 8;
    uchar data[20] = {0x01, 0x06, 0x10, 0x13, 0x00, 0xF0, 0x7C, 0x8B};
    data[0] = mItem->addr;

    ushort crc = mModbus->rtu_crc(data, len-2);
    data[6] = 0xff&crc; /*低8位*/
    data[7] = crc >> 8; /*高8位*/
    mModbus->delay(1);

    mModbus->writeSerial(data, len);
}

void Ctrl_SiThread::initAcCmd(sRtuSetItem &item)
{
    int k=0, len=12;

    item.addr = mItem->addr;
    item.fn = 0x10;
    item.reg = 0x1002;
    item.num = len;
    item.len = len;

    for(int i=0; i<3; ++i) {
        item.data[k++] = mItem->cTh.vol_max;
        item.data[k++] = mItem->cTh.vol_min;
    }

    for(int i=0; i<3; ++i) {
        item.data[k++] = mItem->cTh.cur_max/10;
        item.data[k++] = mItem->cTh.cur_min/10;
    }
}

void Ctrl_SiThread::initDcCmd(sRtuSetItem &item)
{
    int k=0, len=4;

    item.addr = mItem->addr;
    item.fn = 0x10;
    item.reg = 0x1014;
    item.num = len;
    item.len = len;

    item.data[k++] = mItem->cTh.vol_max;
    item.data[k++] = mItem->cTh.vol_min;
    item.data[k++] = mItem->cTh.cur_max/10;
    item.data[k++] = mItem->cTh.cur_min/10;
}

void Ctrl_SiThread::initAcCmd(QList<sRtuSetItem>& item,unsigned short reg)
{
    int k=0;

    sRtuSetItem t;
    t.addr = mItem->addr;
    t.fn = 0x10;
    t.reg = reg;

    if(reg>=0x1002&&reg<=0x1007){
        if(reg%2==0)
        t.data[k++] = mItem->cTh.vol_max;
        else
        t.data[k++] = mItem->cTh.vol_min;
    }

    if(reg>=0x1008&&reg<=0x100D){
        if(reg%2==0)
        t.data[k++] = mItem->cTh.cur_max/10;
        else
        t.data[k++] = mItem->cTh.cur_min/10;
    }
    item.append(t);
}

void Ctrl_SiThread::initDcCmd(QList<sRtuSetItem>& item,unsigned short reg)
{
    int k=0;

    sRtuSetItem t;
    t.addr = mItem->addr;
    t.fn = 0x10;
    t.reg = reg;//0x1014;
    //t.num = len;
    //t.len = len;

    t.data[k++] = mItem->cTh.vol_max;
    t.data[k++] = mItem->cTh.vol_min;
    t.data[k++] = mItem->cTh.cur_max/10;
    t.data[k++] = mItem->cTh.cur_min/10;
    item.append(t);
}

void Ctrl_SiThread::initWriteCmd(QList<sRtuSetItem> &item,unsigned short reg)
{
    sDevType *devType = sDataPacket::bulid()->devType;
    if(DC == devType->ac) {
        initDcCmd(item,reg);
    } else {
        initAcCmd(item,reg);
    }
}

bool Ctrl_SiThread::setThreshold()
{
    bool ret = true;
    if(mItem->cTh.type > 0) {
        QList<sRtuSetItem> itRtu;
        sDevType *devType = sDataPacket::bulid()->devType;
        int len = 12;
        if(DC == devType->ac){
           len = 4;
        }
        for(int i = 0 ; i < len ; i++){
            initWriteCmd(itRtu);
        }
        mModbus->delay(1);
        for(int i = 0 ; i < len ; i++){
            ret = mModbus->rtuWrite(&itRtu);
            mModbus->delay(1);
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
