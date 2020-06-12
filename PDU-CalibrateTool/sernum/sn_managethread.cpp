/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "sn_managethread.h"

SN_ManageThread::SN_ManageThread(QObject *parent) : QThread(parent)
{
    mPacket = sDataPacket::bulid();
    mModbus = Ad_Modbus::bulid(this);
    mItem = AdjustConfig::bulid()->item;
}

SN_ManageThread *SN_ManageThread::bulid(QObject *parent)
{
    SN_ManageThread* sington = nullptr;
    if(sington == nullptr)
        sington = new SN_ManageThread(parent);
    return sington;
}

void SN_ManageThread::initReadCmd(sRtuItem &item)
{
    item.addr = mItem->addr;
    item.fn = 0x03;
    item.reg = 0xA001;
    item.len = 0x09;
}

bool SN_ManageThread::checkSn(uchar *sn, int len)
{
    bool ret = false;
    if(len == 9) {
        uchar exor = mModbus->getXorNumber(sn, len-1) % 10;
        if(exor == sn[len-1]) {
            ret = false;
        }
    }
    return ret;
}

bool SN_ManageThread::analySn(uchar *sn, int len, sSnItem &it)
{
    uchar *ptr = sn;
    it.devType[0] = *ptr++;
    it.devType[1] = *ptr++;

    bool ret = checkSn(sn, len);
    if(ret) {
        it.date[0] = *ptr++;
        it.date[1] = *ptr++;
        it.date[2] = *ptr++;
        it.num = (*ptr++) << 8;
        it.num += *ptr++;

        it.pc = *ptr++;
        it.exor = *ptr++;
    }

    return ret;
}

QString SN_ManageThread::toSnStr(sSnItem &it)
{
    it.sn = QString("%1%2%3%4%5%6%7%8")
            .arg((char)it.devType[0])
            .arg((char)it.devType[1])
            .arg(it.date[0], 2, 10, QLatin1Char('0'))
            .arg(it.date[1], 2, 10, QLatin1Char('0'))
            .arg(it.date[3], 2, 10, QLatin1Char('0'))
            .arg(it.num, 4, 10, QLatin1Char('0'))
            .arg((char)it.pc)
            .arg((char)it.exor);

    return it.sn;
}

bool SN_ManageThread::readSn(sSnItem &itSn)
{
    sRtuItem itRtu;
    bool ret = false;

    initReadCmd(itRtu);
    uchar buf[32] = {0};
    int len = mModbus->rtuRead(&itRtu, buf);
    if(len > 0) {
        ret =  analySn(buf, len, itSn);
        if(ret) {
            toSnStr(itSn);
        }
    }

    return ret;
}

void SN_ManageThread::initWriteCmd(sRtuSetItem &item, uchar *data, int len)
{
    item.addr = mItem->addr;
    item.fn = 0x03;
    item.reg = 0xA001;
    item.len = len;
    for(int i=0; i<len; ++i) {
        item.data[i] = data[i];
    }
}

void SN_ManageThread::createSn(sSnItem &it)
{
    QDate date = QDate::currentDate();
    it.date[0] = date.year() % 100;
    it.date[1] = date.month();
    it.date[2] = date.day();

    it.num = ++(mItem->currentNum);
    it.pc = mItem->pcNum;
    toSnStr(it);
}

int SN_ManageThread::toSnData(sSnItem &it, uchar *data)
{
    int k = 0;
    data[k++] = it.devType[0];
    data[k++] = it.devType[1];

    data[k++] = it.date[0];
    data[k++] = it.date[1];
    data[k++] = it.date[2];

    data[k++] = it.num / 256;
    data[k++] = it.num % 256;

    data[k++] = it.pc;

    uchar exor = mModbus->getXorNumber(data, 8) % 10;
    data[k++] = it.exor = exor;

    return 9;
}

bool SN_ManageThread::writeSn(sSnItem &itSn)
{
    createSn(itSn);
    uchar buf[32] = {0};
    int len = toSnData(itSn, buf);

    sRtuSetItem itRtu;
    initWriteCmd(itRtu, buf, len);
    mModbus->delay(1);

    return mModbus->rtuWrite(&itRtu);
}

void SN_ManageThread::writeStatus(bool ret)
{
    if(ret) {
        mPacket->status = tr("已写入序列号：\n%1").arg(mSnItem.sn);
    } else {
        mPacket->status = tr("序列号写入失败：\n%1").arg(mSnItem.sn);
        mItem->currentNum -= 1;
    }
}

bool SN_ManageThread::snEnter()
{
    mSnItem.sn.clear();

    bool ret = readSn(mSnItem);
    if(ret) {
        mPacket->status = tr("已读到序列号：\n%1").arg(mSnItem.sn);
    } else {
        ret = writeSn(mSnItem);
        writeStatus(ret);
    }
    mItem->sn = mSnItem.sn;
    ret = mModbus->delay(1);

    return ret;
}
