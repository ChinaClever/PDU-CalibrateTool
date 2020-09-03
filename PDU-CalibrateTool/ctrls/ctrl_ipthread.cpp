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
    uchar len = 8;
    uchar data[20] = {0x01, 0x06, 0x10, 0x13, 0x00, 0xF0, 0x7C, 0x8B};
    data[0] = mItem->addr;

    ushort crc = mModbus->rtu_crc(data, len-2);
    data[6] = 0xff&crc; /*低8位*/
    data[7] = crc >> 8; /*高8位*/
    mModbus->delay(1);

    bool ret = false;
    len = mModbus->transmit(data, len, data, 1);
    if(len > 0) ret = true;

    mPacket->status = tr("出厂电能清除");
    mModbus->appendLogItem(ret);
}

void Ctrl_IpThread::initWriteCmd(sRtuSetItems &item)
{
    int k=0, len=12;

    item.addr = mItem->addr;
    item.fn = 0x06;
    item.reg = 0x1002;
    item.num = len;
    item.len = 2*len;

    for(int i=0; i<3; ++i) {
        item.data[k++] = mItem->cTh.vol_min >> 8;
        item.data[k++] = mItem->cTh.vol_min & 0xff;

        item.data[k++] = mItem->cTh.vol_max >> 8;
        item.data[k++] = mItem->cTh.vol_max & 0xff;
    }

    for(int i=0; i<3; ++i) {
        item.data[k++] = mItem->cTh.cur_min >> 8;
        item.data[k++] = mItem->cTh.cur_min & 0xff;

        item.data[k++] = mItem->cTh.cur_max >> 8;
        item.data[k++] = mItem->cTh.cur_max & 0xff;
    }
}

bool Ctrl_IpThread::setThreshold()
{
    bool ret = true;
    if(mItem->cTh.type > 0) {
        sRtuSetItems itRtu;
        initWriteCmd(itRtu);
        mModbus->delay(1);

        mPacket->status = tr("出厂阈值设置");
        ret = mModbus->rtuWrites(&itRtu);
        mModbus->appendLogItem(ret);
    }
    return ret;
}

void Ctrl_IpThread::setTime()
{
    int k = 0;
    sRtuSetItems it;
    it.addr = mItem->addr;
    it.fn = 0x10;
    it.reg =0x33;
    it.num = 6;
    it.len = 6;

    QDateTime time = QDateTime::currentDateTime();
    it.data[k++] = time.date().year() % 100;
    it.data[k++] = time.date().month();
    it.data[k++] = time.date().day();
    it.data[k++] = time.time().hour();
    it.data[k++] = time.time().minute();
    it.data[k++] = time.time().second();

    mPacket->status = tr("时间设置");
    bool ret = mModbus->rtuWrites(&it);
    mModbus->appendLogItem(ret);
    mModbus->delay(1);
}

bool Ctrl_IpThread::inputMacAddr(uchar *buf)
{
    bool ret = false;
    char *ptr = mItem->cTh.mac_addr;
    int len = strlen(ptr);
    if(len) {
        Ad_MacAddr *mac = Ad_MacAddr::bulid();
        len = mac->macToChar(ptr, buf);
        if(len > 0) {
            ret = true;
            if(mItem->cTh.mac_clear > 0) {
                mModbus->writeMac(ptr);
                mac->macAdd(ptr, ptr);
            } else {
                ptr[0] = 0;
            }
        }
    }

    return ret;
}

void Ctrl_IpThread::setMacAddr()
{
    sRtuSetItems it;
    it.addr = mItem->addr;
    it.fn = 0x10;
    it.reg =0x33;
    it.num = 6;
    it.len = 6;

    bool res = inputMacAddr(it.data);
    if(res){
        mPacket->status = tr("出厂MAC设置");
        bool ret = mModbus->rtuWrites(&it);
        mModbus->appendLogItem(ret);
        mModbus->delay(1);
    }
}

// 表示行业标准 Modbus RTU 模式
bool Ctrl_IpThread::setModel()
{
    uchar len = 8;
    uchar buf[20] = {0x01, 0x06, 0x10, 0x19, 0x00, 0x01, 0x5C, 0xCD};
    buf[0] = mItem->addr;

    ushort crc = mModbus->rtu_crc(buf, len-2);
    buf[6] = 0xff&crc; /*低8位*/
    buf[7] = crc >> 8; /*高8位*/
    mModbus->delay(1);

    bool ret = false;
    len = mModbus->transmit(buf, len, buf, 1);
    if(len > 0) ret = true;

    mPacket->status = tr("出厂模式切换");
    mModbus->appendLogItem(ret);

    return ret;
}

bool Ctrl_IpThread::setclearLog()
{
    uchar len = 8;
    uchar buf[20] = {0x01, 0x06, 0x10, 0x19, 0x00, 0x01, 0x5C, 0xCD};
    buf[0] = mItem->addr;

    ushort crc = mModbus->rtu_crc(buf, len-2);
    buf[6] = 0xff&crc; /*低8位*/
    buf[7] = crc >> 8; /*高8位*/
    mModbus->delay(1);

    bool ret = false;
    len = mModbus->transmit(buf, len, buf, 1);
    if(len > 0) ret = true;

    mPacket->status = tr("出厂模式切换");
    mModbus->appendLogItem(ret);

    return ret;
}


bool Ctrl_IpThread::factorySet()
{
    bool ret = setThreshold();
    if(ret) {
        if(mItem->cTh.time_set > 0) setTime(); // 设置时间
        if(mItem->cTh.ele_clear > 0) funClearEle(nullptr);
        if(mItem->cTh.mac_set > 0)  setMacAddr();
        if(mItem->cTh.log_clear > 0)  setclearLog();
        if(mItem->cTh.ip_mod > 0) ret = setModel();
    }

    return ret;
}
